#include "motor.hpp"

Motor::Motor(int pin):pin(pin),speed(0){
  pinMode(pin, OUTPUT);
}

int  Motor::get_pin()const{
  return this->pin;  
}

int  Motor::get_speed()const{
  return this->speed;
}

void Motor::set_speed(int speed){
  if (speed > 255) this->speed = 255;
  if (speed < 0)   this->speed = 0;
  this->speed = speed;
  analogWrite(this->pin, this->speed);
}
void Motor::max_speed(){
  this->speed = 255;
  analogWrite(this->pin, this->speed);
}
void Motor::min_speed(){
  this->speed = 0;
  analogWrite(this->pin, this->speed);
}