#include "hardware_conf.h"
#include "motorboard.h"
#include "ultrasonic.h"
#include "wall_follower.h"

#include <Servo.h>

Ultrasonic ultrasonic;
Motorboard motorboard;

WallFollower follower;

Servo my_servo;

void setup(){
    my_servo.attach(4);
    ultrasonic.init(SONIC_TRIG_PIN, SONIC_ECHO_PIN);
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
        &ultrasonic,
        &my_servo
    );

    follower.start();
}

void loop(){
    if(millis() > 5000){
        follower.stop();
    }else{
        follower.tick();
        delay(100);
    }
}