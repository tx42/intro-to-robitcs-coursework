#pragma once
#include <Arduino.h>

#include "motorboard.h"

// FIXME: the whole thing will fall apart
// if we go in cruise

class Localisation{
private:
    Motorboard& m_motorboard;

    int L_ENCODER_PIN;
    int R_ENCODER_PIN;

    bool m_last_enc_state_l;
    bool m_last_enc_state_r;

    float m_wheel_radius;
    float m_lr_wheel_dist;
    int m_num_slits;

    float encDistToRealDist(int enc_dist);

    void tickLeft();
    void tickRight();
public:
    // current x, y position and orientation
    // x, y is a righthanded coordinate system
    // at angle 0, y is forward direction
    // x, y are measured in cm
    // angle is measured in radians
    float x;
    float y;
    float angle;

    // all distances in cm
    Localisation(){};
    void init(Motorboard& motorboard, int r_encoder_pin, int l_encoder_pin,
                    float wheel_radius, float lr_wheel_dist, int num_slits);

    // checks data from encoders
    // and updates position
    //
    // must be called frequently
    // to not skip any samples 
    void tick();
};
