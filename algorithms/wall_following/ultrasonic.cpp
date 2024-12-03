#include "ultrasonic.h"

// max distance detectable by ultrasonic (in cm)
#define MAX_DISTANCE 100

void Ultrasonic::init(int trigger_pin, int echo_pin){
   m_TRIGGER_PIN = trigger_pin;
   m_ECHO_PIN = echo_pin;

   // pin init
   pinMode(m_TRIGGER_PIN, OUTPUT);
   pinMode(m_ECHO_PIN, INPUT);
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
   return SPEED_OF_SOUND*(float)tof/100.0/2.0;
}

float Ultrasonic::measure(){
    Serial.print("Sending pulse on ");
    Serial.println(m_TRIGGER_PIN);
   pingSensor(m_TRIGGER_PIN);
   long duration = pulseIn(m_ECHO_PIN, HIGH, MAX_DISTANCE * 30);
   if(duration == 0){
      return MAX_DISTANCE;
   }

   float distance = tofToDistance(duration);
}
