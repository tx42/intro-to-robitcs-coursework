/* MOTORBOARD.H 
 * 
 * Provides useful functions concerning motorboard
 * operations.
 */
#pragma once
#include "auxtypes.h"

enum MotorMode{
   MOTOR_FORWARD,
   MOTOR_REVERSE,
   MOTOR_CRUISE
};

struct MotorboardConfig{
   pin r_pwr;
   pin l_pwr;

   pin r_fwd;
   pin r_rev;
   pin_l_fwd;
   pin_l_rev;
};

class Motorboard{
   private:
   // pin declarations
   pin m_L_POWER_PIN;
   pin m_R_POWER_PIN;

   pin m_L_FWD_PIN;
   pin m_L_REV_PIN;
   pin m_R_FWD_PIN;
   pin m_R_REV_PIN;

   int m_turn_vel;
   int m_fwd_vel;
   public:
   Motorboard(MotorboardConfig config);

   // high-level motorboard control
   void forward(int power);
   void right(int power);
   void left(int power);
   void back(int power);
   void cruise();
   void stop();

   // TODO: velocity-turn control
   // void setCwTurn(int turn_velocity);
   // void setForwardVelocity(int forward_velocity);

   // low-level functionality
   void setLeftMotor(int power, MotorMode mode);
   void setRightMotor(int power, MotorMode mode);
};

