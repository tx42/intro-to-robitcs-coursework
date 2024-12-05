#include "line_follower.h"

void LineFollower::init(Motorboard* motorboard, int ext_left, int left, int center, int right, int ext_right){
   m_motorboard = motorboard;
   
   m_sensor_pins[0] = ext_left;
   m_sensor_pins[1] = left;
   m_sensor_pins[2] = center;
   m_sensor_pins[3] = right;
   m_sensor_pins[4] = ext_right;

   // initialise hardware
   for (int i = 0; i < 5; i++)
   {
      pinMode(m_sensor_pins[i], INPUT);
   }
}

void LineFollower::tick(){
   int sensor_state = getSensorState();

   // decision picker based on state
   // yes, I know that's horrible
   // I didn't even add mirroring
   // should be ashamed of myself...
   // but to be fair, I'm just modifying their code, soo..
   switch(sensor_state){
      // STRONG LEFT
      case 0b10001:
         m_motorboard->left(speed);
         break;

      // SLIGHT LEFT
      case 0b10100:
      case 0b01000:
      case 0b01100:
      case 0b11100:
      case 0b10010:
      case 0b11010:
         m_motorboard->lightLeft(speed);
         break;
         
      // STRONG RIGHT
      case 0b00001:
         m_motorboard->right(speed);
         break;

      // SLIGHT RIGHT
      case 0b00010:
      case 0b00011:
      case 0b00101:
      case 0b00110:
      case 0b00111:
      case 0b01001:
      case 0b01011:
      case 0b01101:
      case 0b01111:
         m_motorboard->lightRight(speed);
         break;

      // STOP
      case 0b11111:
         m_motorboard->stop();
         break;
   }
}

int LineFollower::getSensorState(){
   int state = 0;

   // true read means ground
   for (int i = 0; i < 5; i++)
   {
      // inversed shift order
      state |= (digitalRead(m_sensor_pins[i]) == LOW) << (4 - i);
   }

   return state;
}