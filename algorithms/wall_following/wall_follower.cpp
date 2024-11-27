#include "wall_follower.h"

const float dir_degree = 45.0;
const float turn_coefficient = 0.5;
const float target_dist = 10.0; // in cm
const float follow_vel = 100.0;

void WallFollower::init(Motorboard& motorboard, Ultrasonic& ultrasonic, Servo& dir_servo){
    m_motorboard = motorboard;
    m_ultrasonic = ultrasonic;
    m_servo = dir_servo;
}

void WallFollower::setFollowDirection(FollowDirection direction){
    m_follow_dir = direction;
}

void WallFollower::setupFollowing(){
    if(m_status != STOPPED) return; // shouldn't normally happen

    m_servo.write(dir_degree);
    m_status = SETTING_UP;
    m_setup_finish_time = millis() + SERVO_SETUP_DELAY;
}

void WallFollower::startFollowing(){
    if(m_status != READY) return;

    m_status = FOLLOWING;
}

void WallFollower::blockingSetupAndStart(){
    setupFollowing();
    while(getStatus() == SETTING_UP){
        checkSetupTimer();
    }

    startFollowing();
}

void WallFollower::checkSetupTimer(){
    if(m_setup_finish_time < millis()){
        m_status = READY;
    }
}

void WallFollower::followTick(){
    // get distance measurement
    float dist = m_ultrasonic.measure();
    // calculate turn
    float turn = turn_coefficient * (target_dist - dist);
    // apply turn and velocity
    m_motorboard.setVelocityTurn(follow_vel, turn);
}

void WallFollower::tick(){
    switch(m_status){
        case SETTING_UP:
            checkSetupTimer();
            break;
        case FOLLOWING:
        case LOST:
            followTick();
            break;
    }
}

void WallFollower::stopFollowing(){
    m_motorboard.stop();

    m_status = STOPPED;
}

FollowStatus WallFollower::getStatus(){
    return m_status;
}