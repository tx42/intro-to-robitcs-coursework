/* ULTRASONIC.H 
 * 
 * Provides ultrasonic sensor functionality.
 * Exposes class Ultrasonic which has the
 * ability to poll data and return distance
 */
#pragma once
#include "auxtypes.h"

class Ultrasonic{
   private:
   // pin defenitions
   pin _TRGGER_PIN;
   pin _ECHO_PIN;

   public:
   Ultrasonic(pin trigger_pin, pin echo_pin);

   float measure();
};

