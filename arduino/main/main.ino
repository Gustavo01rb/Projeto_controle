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
  int teste = sensor->get_distance_with_range(0,9);
  if (teste != -1){
    cont_rpm++;
    delayMicroseconds(3); 
  }
  motor->set_speed(122);
  if(threadRpm.shouldRun())
		threadRpm.run();
}

void rpm(){
  Serial.println("Passou 30 segundos");
  int rpm = cont_rpm * 2;
  Serial.print("O motor roda a ");
  Serial.print(rpm);
  Serial.println(" rpm\n\n");
  cont_rpm = 0;
  
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
    String l_stepsPerRevolution = "";
    String l_setSpeed = "";
    for(int i = 0; i < length; i++){ 
      aux = (char) payload[i];
      if (aux == ';'){ cont++; continue;}
      if (cont == 0){l_active += aux; continue;}
      if (cont == 1){l_stepsPerRevolution += aux; continue;}
      if (cont == 2){l_setSpeed += aux; continue;}
    }
    
  }