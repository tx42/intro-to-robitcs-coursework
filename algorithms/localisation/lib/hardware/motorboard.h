/* MOTORBOARD.H 
 * 
 * Provides useful functions concerning motorboard
 * operations.
 */
#include "auxtypes.h"

enum MotorMode{
   MOTOR_FORWARD,
   MOTOR_REVERSE,
   MOTOR_CRUISE
};

struct MotorboardConfig{
   public:
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
   pin _L_POWER_PIN;
   pin _R_POWER_PIN;

   pin _L_FWD_PIN;
   pin _L_REV_PIN;
   pin _R_FWD_PIN;
   pin _R_REV_PIN;

   public:
   Motorboard(MotorboardConfig config);

   // high-level motorboard control
   void forward(int power);
   void right(int power);
   void left(int power);
   void back(int power);
   void cruise();
   void stop();

   // velocity-turn control
   void set_cw_turn(int turn_velocity);
   void set_forward_velocity(int forward_velocity);

   // low-level functional
   void set_left_motor(byte power, MotorMode mode);
   void set_right_motor(byte power, MotorMode mode);
};

