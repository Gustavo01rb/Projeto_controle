#include "motor.hpp"

Motor::Motor(int pin):pin(pin),speed(0),active(false){
  pinMode(pin, OUTPUT);
}

int  Motor::get_pin()const{
  return this->pin;  
}

bool Motor::get_active() const{
  return this->active;
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
void Motor::receive_message(bool active, int speed){
  this->active = active;
  if (this->active) this->set_speed(speed);
  else this->min_speed();
}
void Motor::control(int fix){
  int c_speed = this->speed + fix;
  if (c_speed > 255) c_speed = 255;
  if (c_speed < 0)   c_speed = 0;
  analogWrite(this->pin, c_speed);
}

