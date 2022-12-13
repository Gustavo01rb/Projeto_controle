#include "d_sensor.hpp"

DSensor::DSensor(int trigPin, int echoPin ): trigPin(trigPin), echoPin(echoPin){
  pinMode(trigPin, OUTPUT);   
  pinMode(echoPin, INPUT);
}
int DSensor::get_distance(){
  digitalWrite(this->trigPin, LOW);  
  delayMicroseconds(1);  
  digitalWrite(this->trigPin, HIGH);  
  delayMicroseconds(5);  
  digitalWrite(this->trigPin, LOW);  
  int duration = pulseIn(this->echoPin, HIGH);  
  return duration*0.034/2; 
}
int DSensor::get_distance_with_range(int min, int max){
  int current_distance = this->get_distance();
  if (current_distance >= min && current_distance <= max)
    return current_distance;
  return -1;
}