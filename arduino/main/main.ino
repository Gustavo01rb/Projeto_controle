#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include "config.hpp"
WebSocketsServer webSocket = WebSocketsServer(WEB_PORT); 


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

 const int trigPin = D6;   
 const int echoPin = D7;   
 long duration;  
 int distance; 
void setup() {
  Serial.begin(SERIALSPEED);
  connectToWiFi();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  pinMode(SERVO_PIN, OUTPUT);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output  
 pinMode(echoPin, INPUT);
}

void loop() {
  webSocket.loop();
 digitalWrite(trigPin, LOW);  
 delayMicroseconds(2);  
 digitalWrite(trigPin, HIGH);  
 delayMicroseconds(10);  
 digitalWrite(trigPin, LOW);  
 duration = pulseIn(echoPin, HIGH);  
 distance= duration*0.034/2;  
 if(distance <= 13){
  Serial.print("Distance: ");  
  Serial.println(distance);
 }  
  
  
  analogWrite(SERVO_PIN, 120);


  /*int valor_pwm = 0;  
  for (valor_pwm = 0; valor_pwm < 120; valor_pwm++){
    analogWrite(SERVO_PIN, valor_pwm);
    Serial.print("Valor setado:");
    Serial.println(valor_pwm);
    Serial.print("Valor lido:");
    yeld();
    Serial.println(analogRead(A0));
    delay(1000);

  }
  for (valor_pwm = 120; valor_pwm >= 0; valor_pwm--){
    analogWrite(SERVO_PIN, valor_pwm);
    Serial.println(valor_pwm);
    delay(100);

  }*/
}