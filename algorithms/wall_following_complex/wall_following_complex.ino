#include <Servo.h>

#include "motorboard.h"
#include "ultrasonic.h"

#include "wall_follower_v2.h"
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

    // follower detailed configurations
    // basic
    follower.follow_sensor_degree = 40;
    follower.turn_coefficient = 2.0;
    follower.target_distance = 60.0;
    follower.follow_vel = 60.0;

    // deceleration
    follower.deceleration_start_dist = 50.0;
    follower.deceleration_end_dist = 40;
    follower.min_follow_vel = 40.0;

    // narrowing
    follower.min_target_dist = 30.0;
    follower.narrow_update_delay = 1000;

    follower.start();
}

void loop(){
   follower.tick();
}
