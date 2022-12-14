#pragma once
#include "Arduino.h"

class Motor{
  private:
    const int pin;
    int speed;
    bool active;

  public:
    Motor(int pin);
    int get_pin() const;
    int get_speed() const;
    bool get_active() const;
    void set_speed(int speed);
    void max_speed();
    void min_speed();
    void receive_message(bool active, int speed);
    void control(int fix);

};