#pragma once
#include "Arduino.h"

class Motor{
  private:
    const int pin;
    int speed;
  public:
    Motor(int pin);
    int get_pin() const;
    int get_speed() const;
    void set_speed(int speed);
    void max_speed();
    void min_speed();

};