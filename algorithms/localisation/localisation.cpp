#include "localisation.h"

void Localisation::init(Motorboard& motorboard, int r_encoder_pin, int l_encoder_pin,
                            float wheel_radius, float lr_wheel_dist, int num_slits){
    m_motorboard = motorboard;

    L_ENCODER_PIN = l_encoder_pin;
    R_ENCODER_PIN = r_encoder_pin;

    m_wheel_radius = wheel_radius;
    m_lr_wheel_dist = lr_wheel_dist;

    // hardware init
    pinMode(L_ENCODER_PIN, INPUT);
    pinMode(R_ENCODER_PIN, INPUT);

    // init coordinate system
    x = 0;
    y = 0;
    angle = 0;
}

int singedEncoderDistance(bool last_measure, bool current_measure, MotorMode motor_mode){
    // if moving forward: check rising edge
    // if moving reverse: check falling edge
    if(motor_mode == MOTOR_FORWARD){
        // rising edge
        if(!last_measure && current_measure){
            return 1;
        }
    }
    if(motor_mode == MOTOR_REVERSE){
        // falling edge
        if(last_measure && !current_measure){
            return -1;
        }
    }

    return 0;
}

float Localisation::encDistToRealDist(int enc_dist){
    // convert encoder distance to radians
    float radians = ((float)enc_dist) / m_num_slits * PI; 
    // radians to distance
    return radians * m_wheel_radius;
}

void Localisation::tickLeft(){
    bool measure = digitalRead(L_ENCODER_PIN);
    int enc_dist = singedEncoderDistance(m_last_enc_state_l, measure, m_motorboard.l_motor_mode);
    m_last_enc_state_l = measure;

    // convert enc_dist to real distance
    float dist = encDistToRealDist(enc_dist);

    // forward move from the left produces CW turn
    float turn = -dist / m_lr_wheel_dist;
    // get centroid shift
    float x_shift = -cos(turn) * m_lr_wheel_dist / 2;
    float y_shift = -sin(turn) * m_lr_wheel_dist / 2;

    // update position
    x += x_shift;
    y += y_shift;
    angle += turn;
}

void Localisation::tickRight(){
    bool measure = digitalRead(R_ENCODER_PIN);
    int enc_dist = singedEncoderDistance(m_last_enc_state_r, measure, m_motorboard.r_motor_mode);
    m_last_enc_state_r = measure;

    // convert enc_dist to real distance
    float dist = encDistToRealDist(enc_dist);

    float turn = dist / m_lr_wheel_dist;
    // get centroid shift
    float x_shift = cos(turn) * m_lr_wheel_dist / 2;
    float y_shift = sin(turn) * m_lr_wheel_dist / 2;

    // update position
    x += x_shift;
    y += y_shift;
    // forward move from the right produces CCW turn
    angle += turn;
}

void Localisation::tick(){
    tickLeft();
    tickRight();
}
