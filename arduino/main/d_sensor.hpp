#pragma once
#include "Arduino.h"

class DSensor{
  private:
    const int trigPin;   
    const int echoPin;   
  
  public:
    DSensor(int trigPin, int echoPin );
    int get_distance();
    int get_distance_with_range(int min, int max);
};