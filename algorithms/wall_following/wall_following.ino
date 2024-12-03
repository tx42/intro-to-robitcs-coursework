#include <Servo.h>

#include "motorboard.h"
#include "ultrasonic.h"

#include "wall_follower.h"
#include "hardware_conf.h"

Motorboard motorboard;
Ultrasonic ultrasonic;
Servo sonic_dir_servo;

WallFollower follower;

void setup(){
    // setup the motorboard
    motorboard.init(
        MOTOR_R_POWER_PIN, 
        MOTOR_L_POWER_PIN,
        MOTOR_R_FWD_PIN,
        MOTOR_R_REV_PIN,
        MOTOR_L_FWD_PIN,
        MOTOR_L_REV_PIN
    );

    // setup servo
    sonic_dir_servo.attach(SERVO_PIN);
    // setup ultrasonic
    ultrasonic.init(SONIC_TRIG_PIN, SONIC_ECHO_PIN);

    // setup wall follower
    follower.init(&motorboard, &ultrasonic, &sonic_dir_servo);

    Serial.begin(9600);
    Serial.println("Starting in 5 sec");
    // startup delay
    delay(5000);

    follower.start();
}

void loop(){
    Serial.print("measure: ");
    Serial.println(ultrasonic.measure());
  
//    follower.tick();
}
