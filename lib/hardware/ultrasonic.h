/* ULTRASONIC.H 
 * 
 * Provides ultrasonic sensor functionality.
 * Exposes class Ultrasonic which has the
 * ability to poll data and return distance
 */
#pragma once
#include "auxtypes.h"

// speed of sound in meters per second
#define SPEED_OF_SOUND 343.0

class Ultrasonic{
   private:
   // pin defenitions
   pin m_TRGGER_PIN;
   pin m_ECHO_PIN;

   public:
   Ultrasonic(pin trigger_pin, pin echo_pin);

   float measure();
};

