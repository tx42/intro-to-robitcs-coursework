#include "wall_follower.h"

void WallFollower::init(Motorboard* motorboard, Ultrasonic* ultrasonic, Servo* dir_servo){
    m_motorboard = motorboard;
    m_ultrasonic = ultrasonic;
    m_servo = dir_servo;

    setFollowDirection(CW);
}

void WallFollower::setFollowDirection(FollowDirection direction){
    if(getStatus() != STOPPED) return;

    if(direction == CW){
        m_dir_coef = 1;
    }else{
        m_dir_coef = -1;
    }
}

void WallFollower::start(){
    // can only start from STOPPED state
    if(m_status != STOPPED) return;

    // stop all movement
    m_motorboard->stop();
    // setting up servomotor
    m_servo->write(-sensor_degree * m_dir_coef + 90);
    delay(SERVO_SETUP_DELAY);
    m_status = FOLLOWING;
}

void WallFollower::tick(){
    if(m_status != FOLLOWING) return;
    // get distance measurement
    float dist = m_ultrasonic->measure();
    // calculate turn
    float turn = target_distance - dist;
    turn *= turn_coefficient;
    turn *= m_dir_coef;

    // apply turn and velocity
    m_motorboard->setVelocityTurn(follow_vel, turn);
}

void WallFollower::stop(){
    m_motorboard->stop();

    m_status = STOPPED;
}

FollowStatus WallFollower::getStatus(){
    return m_status;
}
