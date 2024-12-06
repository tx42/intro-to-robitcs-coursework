#include "wall_follower.h"

inline float WallFollower::convertOrientCentral2Servo(int angle){
    return -angle * m_traverse_direction_coef + 90;
}

void WallFollower::init(Motorboard* motorboard, Ultrasonic* ultrasonic, Servo* dir_servo){
    m_motorboard = motorboard;
    m_ultrasonic = ultrasonic;
    m_servo = dir_servo;

    setFollowDirection(CW);
}

void WallFollower::setFollowDirection(FollowDirection direction){
    if(getStatus() != STOPPED) return;

    if(direction == CW){
        m_traverse_direction_coef = 1;
    }else{
        m_traverse_direction_coef = -1;
    }
}

void WallFollower::start(){
    // can only start from STOPPED state
    if(m_status != STOPPED) return;

    // stop all movement
    m_motorboard->stop();
    // setting up servomotor
    m_servo->write(convertOrientCentral2Servo(follow_sensor_degree));
    delay(servoRotationDuration(180));
    m_status = FOLLOWING;
}

float WallFollower::measureAtAngle(int angle){
    // convert angle representation
    angle = convertOrientCentral2Servo(angle);
    // rotate servo
    int last_angle = m_servo->read();
    m_servo->write(angle);
    delay(servoRotationDuration(angle - last_angle));

    // by this time, servo should be in position
    // take measures
    float measure = m_ultrasonic->measureSamples(NUMBER_SAMPLES);
    return measure; 
}

void WallFollower::updateMeasurements(){
    // check that the sensor returned to home position after last call
    while(millis() < m_return_home_time){
        // block until it does return
        ;
    }

    // update follow sensor
    m_follow_measure = measureAtAngle(follow_sensor_degree);

    if(millis() > m_last_forward_update + forward_update_delay){
        m_forward_measure = measureAtAngle(0);
        m_last_forward_update = millis();
    }

    // check whether we should update narrow sensor
    if(millis() > m_last_narrow_update + narrow_update_delay){
        m_narrow_measure = measureAtAngle(-narrow_sensor_degree);
        m_last_narrow_update = millis();
    }

    // make sensor go to home position (asynchronously)
    int last_pos = m_servo->read();
    m_servo->write(convertOrientCentral2Servo(follow_sensor_degree));
    // set block for the time of return, just in case
    m_return_home_time = millis() + servoRotationDuration(follow_sensor_degree - last_pos);
}

float WallFollower::calculateFollowDistance(){
    // don't bother if the path is not narrowing
    if(m_narrow_measure >= target_distance){
        return target_distance;
    }

    float follow_dist = 0.5*(target_distance + m_narrow_measure);
    // check it it's smaller then minimum
    return min(follow_dist, min_target_dist);
}

float WallFollower::calculateVelocity(){
    // TODO: add narrow sensor to deceleration as well
    float dist = m_forward_measure;

    // rename symbols to be consistent with the formula
    float v_s = follow_vel;
    float v_e = min_follow_vel;
    float d_s = deceleration_start_dist;
    float d_e = deceleration_end_dist;

    // check bounds
    if(dist > d_s){
        // too far for deceleration to kick in
        return v_s;
    }
    if(dist < d_e){
        // too close, limit speed to minimum
        return v_e;
    }

    float k = (v_e - v_s) / (d_e - d_s);
    float vel = k*(dist - d_s) + v_s;

    return vel;
}

float WallFollower::calculateTurn(float follow_dist)
{
    float turn = follow_dist - m_follow_measure;
    turn *= turn_coefficient;
    turn *= m_traverse_direction_coef;

    return turn;
}

void WallFollower::tick(){
    if(m_status != FOLLOWING) return;

    updateMeasurements();

    float follow_dist = calculateFollowDistance();
    float velocity = calculateVelocity();

    float turn = calculateTurn(follow_dist);
    
    // apply turn and velocity
    m_motorboard->setVelocityTurn(velocity, turn);
}

void WallFollower::stop(){
    m_motorboard->stop();

    m_status = STOPPED;
}

FollowStatus WallFollower::getStatus(){
    return m_status;
}
