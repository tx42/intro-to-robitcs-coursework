#include <Servo.h>

#include "motorboard.h"
#include "localisation.h"
#include "ultrasonic.h"
#include "hardware_conf.h"

Motorboard motorboard;
Localisation loc;
Ultrasonic sensor;

Servo servo;

void setup(){
   servo.attach(9);
   servo.write(90);
   motorboard.init(
      MOTOR_R_POWER_PIN, 
      MOTOR_L_POWER_PIN,
      MOTOR_R_FWD_PIN,
      MOTOR_R_REV_PIN,
      MOTOR_L_FWD_PIN,
      MOTOR_L_REV_PIN
   );

   sensor.init(SONIC_TRIG_PIN, SONIC_ECHO_PIN);

   loc.init(
      &motorboard,
      ENCODER_R_PIN,
      ENCODER_L_PIN,
      WHEEL_RADIUS,
      LR_WHEEL_DIST,
      ENCODER_SLIT_NUM
   );

   loc.update_rate = 50;
   Serial.begin(9600);
}

#define SPEED 250
int milestone = 1;

void loop(){
    // simple code
    // robot will go in one direction stopping every 
    // 10 centimeters
    motorboard.forward(SPEED);
    
    Serial.print(loc.x);
    Serial.print('\t');
    Serial.print(loc.y);
    Serial.print('\t');
    Serial.print(loc.angle);
    Serial.print('\n');

    loc.angle = 0;
    if(loc.y > milestone*100){
      motorboard.stop();
      milestone++;
      while(sensor.measure() > 10.0){
         delay(300);
      }
    }
  
   loc.tick();
}
