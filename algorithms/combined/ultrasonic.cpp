#include "ultrasonic.h"

// max distance detectable by ultrasonic (in cm)
#define MAX_DISTANCE 100

void Ultrasonic::init(int trigger_pin, int echo_pin){
   TRIGGER_PIN = trigger_pin;
   ECHO_PIN = echo_pin;

   // pin init
   pinMode(TRIGGER_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);
}

void pingSensor(int trigger_pin){
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigger_pin, LOW);
}

// tof - time of flight in microseconds
// returns - distance in centimeters
float tofToDistance(long tof){
   return SPEED_OF_SOUND*(float)tof/10000.0/2.0;
}

float Ultrasonic::measure(){
   pingSensor(TRIGGER_PIN);
   long duration = pulseIn(ECHO_PIN, HIGH);

   if(duration == 0){
      return MAX_DISTANCE;
   }

   return tofToDistance(duration);
}
