#pragma once

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <math.h>
#include "Arduino.h"

#define SERIALSPEED 9600
#define ONE_SECOND 1000
#define SSID "Gustavo"
#define SSID_PASSWORD "12345678"
#define WEB_PORT 81
#define SERVO_PIN D5
#define ECHO_PIN D7 
#define TRIG_PIN D6

#define maxRPM 300
#define boundarieFuzzy  maxRPM/2
#define limitVoltage  3
