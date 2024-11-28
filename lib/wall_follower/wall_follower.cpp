#include "wall_follower.h"

void WallFollower::init(Motorboard* motorboard, Ultrasonic* ultrasonic, Servo* dir_servo){
    m_motorboard = motorboard;
    m_ultrasonic = ultrasonic;
    m_servo = dir_servo;
}

void WallFollower::setFollowDirection(FollowDirection direction){
    if(getStatus() != STOPPED) return;

    m_follow_dir = direction;
}

float WallFollower::getFollowDirectionCoefficient(){
    if(m_follow_dir == CW){
        return 1.0;
    }else{
        return -1.0;
    }
}

void WallFollower::start(){
    // can only start from STOPPED state
    if(m_status != STOPPED) return;

    // stop all movement
    m_motorboard->stop();
    // setting up servomotor
    m_servo->write(-sensor_degree * getFollowDirectionCoefficient() + 90);
    delay(SERVO_SETUP_DELAY);
    m_status = FOLLOWING;
}

void WallFollower::tick(){
    if(m_status != FOLLOWING) return;
    // get distance measurement
    float dist = m_ultrasonic->measure();
    // calculate turn
    float turn = turn_coefficient * (target_distance - dist);
    turn *= getFollowDirectionCoefficient();
    // apply turn and velocity
    m_motorboard->setVelocityTurn(follow_vel, turn);
}

void WallFollower::stopFollowing(){
    m_motorboard->stop();

    m_status = STOPPED;
}

FollowStatus WallFollower::getStatus(){
    return m_status;
}
