#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Stepper.h> 
#include "config.hpp"

WebSocketsServer webSocket = WebSocketsServer(WEB_PORT); 
bool activeded_step_motor = false;
int stepsPerRevolution = 0;
int setSpeed = 0;

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
  if(l_active == "true") activeded_step_motor = true; 
  else activeded_step_motor = false;
  stepsPerRevolution = l_stepsPerRevolution.toInt();
  setSpeed = l_setSpeed.toInt();
}

void active_step_motor(){
    if(!activeded_step_motor) return;
    Stepper myStepper(stepsPerRevolution, STEP_M_IN1,STEP_M_IN2,STEP_M_IN3,STEP_M_IN4); 
    myStepper.setSpeed(setSpeed);
    myStepper.step(stepsPerRevolution);  
}

void setup() {
  Serial.begin(SERIALSPEED);
  connectToWiFi();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}



void loop() {
  webSocket.loop();
  active_step_motor();
}