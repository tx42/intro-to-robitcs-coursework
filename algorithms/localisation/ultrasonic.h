/* ULTRASONIC.H 
 * 
 * Provides ultrasonic sensor functionality.
 * Exposes class Ultrasonic which has the
 * ability to poll data and return distance
 */
#pragma once
#include <Arduino.h>

// speed of sound in meters per second
#define SPEED_OF_SOUND 343.0

class Ultrasonic{
private:
   // pin defenitions
   int TRIGGER_PIN;
   int ECHO_PIN;

public:
   Ultrasonic(){};
   void init(int trigger_pin, int echo_pin);

   // returns distance in cm
   float measure();
   float measureSamples(int number);
};

