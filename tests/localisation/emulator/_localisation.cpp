#include "_localisation.h"

void Localisation::init(Motorboard* motorboard, int r_encoder_pin, int l_encoder_pin,
                            float wheel_radius, float wheel_dist, int num_slits){
    m_motorboard = motorboard;

    L_ENCODER_PIN = l_encoder_pin;
    R_ENCODER_PIN = r_encoder_pin;

    m_wheel_radius = wheel_radius;
    m_wheel_distance = wheel_dist;
    m_num_slits = num_slits;

    // hardware init
    pinMode(L_ENCODER_PIN, INPUT);
    pinMode(R_ENCODER_PIN, INPUT);

    // init coordinate system
    x = 0.0;
    y = 0.0;
    angle = 0.0;

    // first measurement
    m_last_enc_state_l = digitalRead(L_ENCODER_PIN);
    m_last_enc_state_r = digitalRead(R_ENCODER_PIN);
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
    float disk_percentage = ((float)enc_dist) / m_num_slits;
    float radians = 2 * PI * disk_percentage; 
    // radians to distance
    float distance = radians * m_wheel_radius;
    return distance;
}

void Localisation::sample(){
    bool left_measure = digitalRead(L_ENCODER_PIN);
    bool right_measure = digitalRead(R_ENCODER_PIN);

    m_l_enc_dist += singedEncoderDistance(m_last_enc_state_l, left_measure, m_motorboard->l_motor_mode);
    m_r_enc_dist += singedEncoderDistance(m_last_enc_state_r, right_measure, m_motorboard->r_motor_mode);

    Serial.print("sampling >> ");
    Serial.print(m_l_enc_dist);
    Serial.print(' ');
    Serial.println(m_r_enc_dist);

    m_last_enc_state_l = left_measure;
    m_last_enc_state_r = right_measure;
}

void Localisation::update(){
    float left_dist = encDistToRealDist(m_l_enc_dist);
    float right_dist = encDistToRealDist(m_r_enc_dist);

    // reset encoder delta
    m_l_enc_dist = 0;
    m_r_enc_dist = 0;

    // Serial.print("update called: ");
    // Serial.print(left_dist);
    // Serial.print('\t');
    // Serial.print(right_dist);
    // Serial.print('\n');

    float displacement = (left_dist + right_dist)/2.0;
    float delta_angle = (right_dist - left_dist)/(m_wheel_distance);

    // Serial.print(displacement);
    // Serial.print('\t');
    // Serial.print(delta_angle);
    // Serial.print('\n');

    angle += delta_angle;

    float delta_x = displacement * -sin(angle);
    float delta_y = displacement * cos(angle);

    x += delta_x;
    y += delta_y;
}

void Localisation::tick(){
    sample();

    if(millis() > m_last_update_time + update_rate){
        m_last_update_time = millis();
        update();
    }
}
