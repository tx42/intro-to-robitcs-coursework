/* LINE_FOLLOWER.H
 * 
 * Provides implementation of the line follower algorithm.
 * Can be used in a similiar fashion to the wall follower algorithm.
 */

#pragma once
#include <Arduino.h>
#include "motorboard.h"

class LineFollower{
private:
   Motorboard* m_motorboard;
   int m_sensor_pins[5];
public:
   int speed = 150;

   LineFollower(){};

   void init(Motorboard* motorboard, int ext_left, int left, int center, int right, int ext_right);
   void tick();
   
   // Returns sensor state.
   // Sensor state is stored in first five byte positions.
   // Set bits indicate tape, while clear indicate ground.
   // Highest bit is leftmost sensor
   int getSensorState();
};