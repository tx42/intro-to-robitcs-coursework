/* MOTORBOARD.H 
 * 
 * Provides useful functions concerning motorboard
 * operations.
 */
#pragma once
#include <Arduino.h>

enum MotorMode{
   MOTOR_FORWARD,
   MOTOR_REVERSE,
   MOTOR_CRUISE
};

class Motorboard{
private:
   // pin declarations
   int m_L_POWER_PIN;
   int m_R_POWER_PIN;

   int m_L_FWD_PIN;
   int m_L_REV_PIN;
   int m_R_FWD_PIN;
   int m_R_REV_PIN;
public:
   MotorMode l_motor_mode;
   MotorMode r_motor_mode;

   int l_power;
   int r_power;

   Motorboard(){};

   void init(int r_pwr,
               int l_pwr,

               int r_fwd,
               int r_rev,
               int l_fwd,
               int l_rev);

   // high-level motorboard control
   void forward(int power);
   // rotates in the CW direction in place
   void right(int power);
   // same as right() but doesn't reverse right motors
   void lightRight(int power);
   void left(int power);
   void lightLeft(int power);
   void back(int power);
   void cruise();
   void stop();

   // velocity turn control
   // values for vel and turn are floats from -100 to 100
   // if supplied values are greater or passed in a combination,
   // input will be normalised.
   // positive turn is counter-clockwise
   void setVelocityTurn(float vel, float turn);

   void setSignedLeftMotor(int power);
   void setSignedRightMotor(int power);

   // low-level functionality
   void setLeftMotor(int power, MotorMode mode);
   void setRightMotor(int power, MotorMode mode);
};
