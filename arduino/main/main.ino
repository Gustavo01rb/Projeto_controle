#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Thread.h>
#include "config.hpp"
#include "motor.hpp"
#include "d_sensor.hpp"

WebSocketsServer webSocket = WebSocketsServer(WEB_PORT); 
Motor*   motor;
DSensor* sensor;
Thread threadRpm;
int cont_rpm = 0;
int fz = 0;

void setup() {
  Serial.begin(SERIALSPEED);
  initConnnection();
  motor = new Motor(SERVO_PIN);
  sensor = new DSensor(TRIG_PIN, ECHO_PIN);
  threadRpm.setInterval(ONE_SECOND * 30);
  threadRpm.onRun(rpm);
}

void loop() {
  webSocket.loop();
  if(!motor->get_active()) return;
  int teste = sensor->get_distance_with_range(0,9);
  if (teste != -1){
    cont_rpm++;
  }
  motor->control(fz);
  if(threadRpm.shouldRun())
		threadRpm.run();
}

void rpm(){
  Serial.println("Passou 30 segundos");
  int rpm = cont_rpm * 2;
  int rpm_expected = map(motor->get_speed(), 0, 127, 0, maxRPM);
  Serial.print("O motor roda a ");
  Serial.print(rpm);
  Serial.print("rpm \nValor esperado: ");
  Serial.print(rpm_expected);
  Serial.println(" rpm");
  Serial.print("Erro: ");
  int err = rpm_expected - rpm;
  Serial.print(err);
  Serial.println(" rpm\n\n");
  fz = fuzzy(err);
  Serial.print(fz);
  Serial.println(" fuzzy\n\n");
  cont_rpm = 0;
  
}

// Funções Fuzzy
  double gaussian(const int x,const double const_param[2]){
    double k = const_param[0];
    double m = const_param[1];
    k = k/2;
    double expoent = (-1)*((x-m)*(x-m))/(k*k);
    return exp(expoent);
  }
  
  int fuzzy(int err){
    const double ant_param[3][2] = {{boundarieFuzzy,-boundarieFuzzy},{boundarieFuzzy,0},{boundarieFuzzy,boundarieFuzzy}};
    const double cons_param[3][2] = {{limitVoltage+1,-3*limitVoltage/4},{2*limitVoltage/3,0},{limitVoltage+1,3*limitVoltage/4}};
    double w[3];
    for(int i =0; i < 3 ; ++i){
        w[i] = gaussian(err,ant_param[i]);
    }
    double c[3][2*limitVoltage+1];
    double agreg_out [2*limitVoltage+1];
    for(int i =0;i<2*limitVoltage+1;++i){
        double yw1 = gaussian(i-limitVoltage,cons_param[0]);
        double yw2 = gaussian(i-limitVoltage,cons_param[1]);
        double yw3 = gaussian(i-limitVoltage,cons_param[2]);
        c[0][i] = fmin(w[0],yw1);
        c[1][i] = fmin(w[1],yw2);
        c[2][i] = fmin(w[2],yw3);
        double m = fmax(c[0][i],c[1][i]);
        agreg_out[i] =  fmax(m,c[2][i]);
    }
    double sumN=0;
    double sumD=0;
    for(int i =0;i < 2*limitVoltage+1;++i){ //Centroid
        sumN+=(i-limitVoltage)*agreg_out[i];
        sumD+=agreg_out[i];
    }
    float r = sumN/sumD; 

    return (int)map(r,-limitVoltage,limitVoltage,0,127);
  }

//Funções de conexão
  void initConnnection(){
    connectToWiFi();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
  }
  void connectToWiFi(){
    Serial.println("===========================================================================");
    Serial.println("NODEMCU iniciado.");
    Serial.println("Testando conexao com rede: ");
    try{
      WiFi.begin(SSID, SSID_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print("\tConectando a rede: '");
        Serial.print(SSID);
        Serial.print("' ...\n");
        delay(ONE_SECOND / 2);
      }
      Serial.println("-------------------------------------------------------------------------");
      Serial.print("NODEMCU conectado a rede : '");
      Serial.print(SSID);
      Serial.print("'.\n");
      Serial.print("\tEndereco de IP: ");
      Serial.print(WiFi.localIP());
      Serial.print("'.\n");
      Serial.println("===========================================================================");
    }catch(...){
      Serial.println("Erro: falha ao conectar a rede local. Verifique se os parâmetros SSID e SSID_PASSWORD estão corretos.");
    }
  }
  void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch (type) {
      case WStype_DISCONNECTED:{caseDisconnected();}break;
      case WStype_CONNECTED:{caseConnected(num);} break;
      case WStype_TEXT:{caseText(num, payload, lenght);}break;
    }

  }
  void caseDisconnected(){
    Serial.println("\n\n Dispositivo externo desconectado.\n\n");
  }
  void caseConnected(uint8_t num){
    Serial.print("Dispositivo externo conectado na porta: ");
    Serial.print(webSocket.remoteIP(num).toString());
    Serial.println();
    webSocket.sendTXT(num, "connected");
  }
  void caseText(uint8_t num, uint8_t * payload, size_t length){
    char aux;
    int cont = 0;
    String l_active = "";
    String l_speed = "";
    for(int i = 0; i < length; i++){ 
      aux = (char) payload[i];
      if (aux == ';'){cont++; continue;}
      if (cont == 0){l_active += aux; continue;}
      if (cont == 1){l_speed += aux; continue;}
    }
    bool respose_a;
    if(l_active == "true") respose_a = true;
    else respose_a = false;
    motor->receive_message(respose_a,l_speed.toInt());
    
  }