#pragma once

#include <Arduino.h>
#include <Servo.h>

#include "motorboard.h"
#include "ultrasonic.h"

// how much time(us) will servo take to rotate by one degree (VERY APPROXIMATE)
#define SERVO_ANGULAR_INVERSE_SPEED 100000/60
#define NUMBER_SAMPLES 3

enum FollowDirection{
    CW,
    CCW
};

enum FollowStatus{
    STOPPED,
    FOLLOWING,
    LOST
};

class WallFollower{
private:
    Motorboard* m_motorboard;
    Ultrasonic* m_ultrasonic;
    Servo* m_servo;

    FollowStatus m_status = STOPPED;

    short m_traverse_direction_coef;

    float m_follow_measure;
    float m_forward_measure;
    float m_narrow_measure;

    long m_last_narrow_update;
    long m_return_home_time;

    // Perform distance measure with ultrasonic at specific angle.
    // Angle is measured from center, with positive
    // angles being towards the wall.
    float measureAtAngle(int angle);
    void updateMeasurements();

    // Performs narrow check and returns new follow distance
    float calculateFollowDistance();
    // Performs obsticle check. Returns smaller speed if obstacle is close
    float calculateVelocity();
    // Calculates turn based on follow sensor's information (positive is CCW)
    float calculateTurn(float follow_dist);

    float inline convertOrientCentral2Servo(int angle);
public:
    int follow_sensor_degree = 45; // degrees shift from center
    int narrow_sensor_degree = 80;
    float turn_coefficient = 0.5;
    
    float follow_vel = 100.0;
    float min_follow_vel = 10.0;

    // distances are in cm
    float target_distance = 30.0;
    float min_target_dist = 20.0;
    float deceleration_end_dist = 10.0;
    float deceleration_start_dist = 30.0;

    // defines how often narrow sensor is updated (in ms)
    int narrow_update_delay = 500;

    WallFollower(){};
    void init(Motorboard* motorboard, Ultrasonic* ultrasonic, Servo* dir_servo);

    // Sets which way wall will be traversed:
    // either clockwise or counter-clockwise.
    // Will ignore if follower is not in STOPPED state
    void setFollowDirection(FollowDirection direction);

    void start();

    // Recalculates and updates motor velocities
    // should be called on
    // regular basis while status isn't STOPPED 
    void tick();

    // Stops following.
    void stop();

    FollowStatus getStatus();
};

// Returns safe estimate of time (in ms)
// it takes servo to rotate by specified angle
inline int servoRotationDuration(int angle){
    if(angle == 0){
        return 0;
    }

    return (SERVO_ANGULAR_INVERSE_SPEED * abs(angle)) / 1000 + 10;
}