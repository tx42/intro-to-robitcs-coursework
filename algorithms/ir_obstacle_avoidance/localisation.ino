#include "motorboard.h"
#include "localisation.h"
#include "hardware_conf.h"

Motorboard motorboard;
Localisation loc;

void setup(){
   motorboard.init(
      MOTOR_R_POWER_PIN, 
      MOTOR_L_POWER_PIN,
      MOTOR_R_FWD_PIN,
      MOTOR_R_REV_PIN,
      MOTOR_L_FWD_PIN,
      MOTOR_L_REV_PIN
   );

   loc.init(
      motorboard,
      ENCODER_R_PIN,
      ENCODER_L_PIN,
      WHEEL_RADIUS,
      LR_WHEEL_DIST,
      ENCODER_SLIT_NUM
   );
}

#define SPEED 255
#define MILESTONE_LENGTH 20

int milestone = 0;

void loop(){
   // simple code
   // robot will go in one direction stopping every 
   // 10 centimeters
   motorboard.forward(SPEED);
   
   // print coordinates

   // remai`neder of y
   if(milestone * MILESTONE_LENGTH < loc.y){
      // stop & wait
      motorboard.stop();
      delay(2000);
   }
}
