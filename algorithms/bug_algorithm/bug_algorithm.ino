#include <Servo.h>

// hardware
#include "hardware_conf.h"
#include "motorboard.h"
#include "ultrasonic.h"

// algorithms
#include "wall_follower.h"
#include "line_follower.h"

// hardware defs
Motorboard motorboard;
Ultrasonic ultrasonic;
Servo sonic_servo;

// algo defs
WallFollower wall_follower;
LineFollower line_follower;

// local defs
enum BugState{
    LINE,
    WALL
} state = LINE;

const float wall_activation_distance = 20.0; // in cm

int max_scan_angle = 70;


void setup(){
    /* HARDWARE SETUPS */
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
    sonic_servo.attach(SERVO_PIN);
    // setup ultrasonic
    ultrasonic.init(SONIC_TRIG_PIN, SONIC_ECHO_PIN);

    /* ALGO SETUPS */
    // setup wall follower
    wall_follower.init(&motorboard, &ultrasonic, &sonic_servo);
    // basic
    wall_follower.follow_sensor_degree = 40;
    wall_follower.turn_coefficient = 0.01;
    wall_follower.target_distance = 20.0;
    wall_follower.follow_vel = 5.0;

    // deceleration
    wall_follower.deceleration_start_dist = 30.0;
    wall_follower.deceleration_end_dist = 10;
    wall_follower.min_follow_vel = 1.0;

    // narrowing
    wall_follower.min_target_dist = 10.0;
    wall_follower.narrow_update_delay = 1000;


    // setup line follower
    line_follower.init(
        &motorboard, 
        LINE_SENSOR_0_PIN,
        LINE_SENSOR_1_PIN,
        LINE_SENSOR_2_PIN,
        LINE_SENSOR_3_PIN,
        LINE_SENSOR_4_PIN
    );

    // speed in range from 0 to 255
    // NOT FLOAT (PERCENTAGE)
    line_follower.speed = 150;
}

float last_measure = 10 * wall_activation_distance;
long scan_swipe_end_time;

bool checkLineExitCondition(){
    // we switch to wall-following when we detect that we are near an obstacle
    return (last_measure < wall_activation_distance);
}

void lineEnter(){
    // some shananigans with rotations...
    // will have to think...
    // TODO!!
}

void lineExit(){
    motorboard.stop();
    delay(1000);
}

void lineTick(){
    // check whether servo had swiped already
    if(millis() > scan_swipe_end_time){
        // restart swipe
        int cur_position = sonic_servo.read();
        int new_position;

        if(cur_position < 90){
            new_position = max_scan_angle;
        }else{
            new_position = -max_scan_angle;
        }

        new_position += 90;

        // start the next swipe
        sonic_servo.write(new_position);
        scan_swipe_end_time = servoRotationDuration(new_position - cur_position) + millis();
    }

    // take ultrasound measurements along the way
    // they aren't fixed to specific position, instead they are 
    // taken while the sensor is moving
    last_measure = ultrasonic.measure();
    // last_measure = ultrasonic.measureSamples(5); // may go badly

    // tick the algorithm
    line_follower.tick();
}

// time after loosing the original line
// during which new line detections
// won't be valid (in ms)
#define LINE_GETAWAY_TIME 1000

bool lost_starting_line = false;
long line_find_acceptance_time;

bool checkWallExitCondition(){
    // wall following stops if line is detected
    int state = line_follower.getSensorState();
    if(!lost_starting_line){
        // waiting to move away from starting line
        if(!state){
            // lost line
            lost_starting_line = true;
            // start timer so we have time to move away from starting line
            line_find_acceptance_time = millis() + LINE_GETAWAY_TIME;
        }
    }else{
        // found a line!
        // check for timer
        if(millis() > line_find_acceptance_time){
            // found genuine line
            return true;
        }
    }

    return false;
}

void wallEnter(){
    lost_starting_line = false;
    wall_follower.start();
}

void wallExit(){
    wall_follower.stop();
}

void wallTick(){
    wall_follower.tick();
}

void loop(){
    // phew.. that was a lengthy init
    // now let's do some magic
    
    // test exit condition, exit/enter as needed
    switch(state){
    case LINE:
        if(checkLineExitCondition()){
            lineExit();
            wallEnter();
            state = WALL;
        }       
        break;
    case WALL:
        if(checkWallExitCondition()){
            wallExit();
            lineEnter();
            state = LINE;
        }
        break;
    }

    // tick
    switch(state){
    case LINE:
        lineTick();
        break;
    case WALL:
        wallTick();
        break;
    }
}
