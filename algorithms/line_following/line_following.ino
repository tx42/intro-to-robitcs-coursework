#include "hardware_conf.h"
#include "motorboard.h"
#include "line_follower.h"

Motorboard motorboard;

LineFollower follower;

void setup()
{
   motorboard.init(
      MOTOR_R_POWER_PIN,
      MOTOR_L_POWER_PIN,
      MOTOR_R_FWD_PIN,
      MOTOR_R_REV_PIN,
      MOTOR_L_FWD_PIN,
      MOTOR_L_REV_PIN
   );

   follower.init(
      &motorboard,
      LINE_SENSOR_0_PIN,
      LINE_SENSOR_1_PIN,
      LINE_SENSOR_2_PIN,
      LINE_SENSOR_3_PIN,
      LINE_SENSOR_4_PIN
   );
}

void loop()
{ 
   follower.tick();
}
