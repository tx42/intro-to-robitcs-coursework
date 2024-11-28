#pragma once
#include <Arduino.h>

#include "motorboard.h"

// FIXME: the whole thing will fall apart
// if we go in cruise

class Localisation{
private:
    Motorboard* m_motorboard;

    int L_ENCODER_PIN;
    int R_ENCODER_PIN;

    bool m_last_enc_state_l;
    bool m_last_enc_state_r;

    int m_l_enc_dist;
    int m_r_enc_dist;

    // constants
    float m_wheel_radius;
    float m_wheel_distance;
    int m_num_slits;

    // accepts number of encoder ticks (encoder distance)
    // returns distance travelled by the wheel (in mm)
    float encDistToRealDist(int enc_dist);

    void sample();
    void update();

    unsigned long m_last_update_time;
public:
    int update_rate = 50; // in ms
    // current x, y position and orientation
    // x, y is a righthanded coordinate system
    // at angle 0, y is forward direction
    // x, y are measured in mm
    // angle is measured in radians
    float x;
    float y;
    float angle;

    // all distances in mm
    Localisation(){};
    void init(Motorboard* motorboard, int r_encoder_pin, int l_encoder_pin,
                    float wheel_radius, float wheel_dist, int num_slits);

    // checks data from encoders
    // and updates position
    //
    // must be called frequently
    // to not skip any samples 
    void tick();
};
