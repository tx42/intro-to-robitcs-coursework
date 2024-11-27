/* MOTORBOARD.H 
 * 
 * Provides useful functions concerning motorboard
 * operations.
 */
#pragma once

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

   // int m_turn_vel;
   // int m_fwd_vel;
public:
   MotorMode l_motor_mode;
   MotorMode r_motor_mode;

   int l_power;
   int r_power;

   Motorboard();

   void init(int r_pwr,
               int l_pwr,

               int r_fwd,
               int r_rev,
               int l_fwd,
               int l_rev);

   // high-level motorboard control
   void forward(int power);
   void right(int power);
   void left(int power);
   void back(int power);
   void cruise();
   void stop();

   // positive turn is counter-clockwise
   void setVelocityTurn(float vel, float turn);

   // low-level functionality
   void setLeftMotor(int power, MotorMode mode);
   void setRightMotor(int power, MotorMode mode);
};
