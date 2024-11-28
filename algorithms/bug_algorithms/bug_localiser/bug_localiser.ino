// implementation of BUG 2 algorithm using odometry
#include <Servo.h>

#include "motorboard.h"
#include "ultrasonic.h"

#include "localisation.h"
#include "wall_follower.h"

#include "hardware_conf.h"

// given in cm
#define LINE_SWITCH_DISTANCE 10
#define LINE_FOLLOW_SPEED 100.0

// in mm
#define GOAL_FINISH_DISTANCE 100

// goal position in mm
const float goal_x = 5000.0;
const float goal_y = 1000.0;

Servo sonic_servo;
Ultrasonic ultrasonic;

Motorboard motorboard;
Localisation loc;

WallFollower wall_follower;

enum State{
    LINE,
    WALL,
    FINISHED,
};

State state;

void setup(){
    // setting up sensors / actuators
    motorboard.init(
        MOTOR_R_POWER_PIN, 
        MOTOR_L_POWER_PIN,
        MOTOR_R_FWD_PIN,
        MOTOR_R_REV_PIN,
        MOTOR_L_FWD_PIN,
        MOTOR_L_REV_PIN
    );

    sonic_servo.attach(SERVO_PIN);
    ultrasonic.init(SONIC_TRIG_PIN, SONIC_ECHO_PIN);

    // setting up systems
    loc.init(
        &motorboard,
        ENCODER_R_PIN,
        ENCODER_L_PIN,
        WHEEL_RADIUS,
        LR_WHEEL_DIST,
        ENCODER_SLIT_NUM
    );

    wall_follower.init(
        &motorboard,
        &ultrasonic,
        &sonic_servo
    );
}

void loop(){
    // state based logic
    switch(state){
        case LINE:
            lineUpdate();
            break;
        case WALL:
            wallUpdate();
            break;
        case FINISHED:
            // if finished, just don't do anything
            motorboard.stop();
            break;
    }
    // follow line
    // if detected object switch to wall following mode
    // follow the wall until the line is detected again
    // then switch to line following

    // ticks and updates
    loc.tick();
}

float calculateTargetAngle(){
    float dx = goal_x - loc.x;
    float dy = goal_y - loc.y;

    float angle = -atan(dx/dy);
    if(dy < 0){
        angle += PI;
    }

    return angle;
}

/* LINE FUNCTIONS */

// uses location information to align itself
// to the correct track to goal
void alignWithLine(){
    float cur_angle = loc.angle;
    float desired_angle = calculateTargetAngle();

    float delta = desired_angle - cur_angle;
    motorboard.setVelocityTurn(LINE_FOLLOW_SPEED, delta);
}

void switchLine(){
    ...
    state = LINE;
}

void lineUpdate(){
    // TODO: slowing down based on obstacle proximity?
    // check if obstacle is on the way
    if(ultrasonic.measure() <= LINE_SWITCH_DISTANCE){
        motorboard.stop();
        switchWall();
        return;
    }

    // check if we are near the goal
    if(reachedGoal()){
        motorboard.stop();
        state = FINISHED;
        return;
    }

    alignWithLine();
}

/* WALL FUNCTIONS */

void switchWall(){
    rotateToDesired();

    wall_follower.setupFollowing();
    state = WALL;
}

void wallUpdate(){
    if(wall_follower.getStatus() == READY){
        wall_follower.startFollowing();
    }
    if(wall_follower.getStatus() == LOST){
        wall_follower.stopFollowing();
        switchLine();
    }

    wall_follower.tick();
}
