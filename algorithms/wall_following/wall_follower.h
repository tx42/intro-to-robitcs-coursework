#pragma once

#include <Arduino.h>
#include <Servo.h>

#include "motorboard.h"
#include "ultrasonic.h"

// time (in ms) it takes for servo motor to set in desired position
// from any other position
#define SERVO_SETUP_DELAY 1000

enum FollowDirection{
    CW,
    CCW
};

enum FollowStatus{
    STOPPED,
    SETTING_UP,
    READY,
    FOLLOWING,
    LOST
};

class WallFollower{
private:
    Motorboard& m_motorboard;
    Ultrasonic& m_ultrasonic;
    Servo& m_servo;

    FollowDirection m_follow_dir = CW;
    FollowStatus m_status = STOPPED;

    unsigned long m_setup_finish_time;
    // Calculates and updates motor velocities
    void followTick();

    void checkSetupTimer();
public:
    WallFollower();
    void init(Motorboard& motorboard, Ultrasonic& ultrasonic, Servo& dir_servo);

    // Sets which way wall will be traversed:
    // either clockwise or counter-clockwise
    void setFollowDirection(FollowDirection direction);

    // Asyncronisly sets up everything needed
    // for wall following to function.
    // Updates status to READY when finished.
    void setupFollowing();

    // Starts wall following
    void startFollowing();

    // Blocking function that's both sets up and starts wall following
    void blockingSetupAndStart();

    // Preforms vital update functions.
    // This method should be called on 
    // regular basis while status isn't STOPPED 
    void tick();

    // Stops following.
    void stopFollowing();

    FollowStatus getStatus();
};