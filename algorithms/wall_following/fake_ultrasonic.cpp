#include "ultrasonic.h"

// max distance detectable by ultrasonic (in cm)
#define MAX_DISTANCE 100

void Ultrasonic::init(int trigger_pin, int echo_pin){
   pinMode(echo_pin, INPUT); 
}

float Ultrasonic::measure(){
   delay(20);
   int value = analogRead(echo_pin);
   int dist = (float)value/1024.0 * MAX_DISTANCE;
   Serial.println(dist);
   return dist;
}
