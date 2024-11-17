#include "motorboard.h"

void Motorboard::Motorboard(MotorboardConfig config){
   m_L_POWER_PIN = config.l_pwr;
   m_R_POWER_PIN = config.r_pwr;

   m_R_FWD_PIN = config.r_fwd;
   m_L_FWD_PIN = config.l_fwd;
   m_R_REV_PIN = config.r_rev;
   m_L_REV_PIN = config.l_rev;

   // hardware setup
   pinMode(m_L_POWER_PIN, OUTPUT);
   pinMode(m_L_FWD_PIN, OUTPUT);
   pinMode(m_L_REV_PIN, OUTPUT);

   pinMode(m_R_POWER_PIN, OUTPUT);
   pinMode(m_R_FWD_PIN, OUTPUT);
   pinMode(m_R_REV_PIN, OUTPUT);
}

void Motorboard::setLeftMotor(int power, MotorMode mode){
   // cap power to byte
   power = constrain(power, 0, 255);

   analogWrite(m_L_POWER_PIN, power);

   // no match means cruise
   digitalWrite(m_L_FWD_PIN, mode == MOTOR_FORWARD);
   digitalWrite(m_L_REV_PIN, mode == MOTOR_REVERSE);
}

// we do a little copy-pasting
void Motorboard::setRightMotor(int power, MotorMode mode){
   power = constrain(power, 0, 255);
   analogWrite(m_R_POWER_PIN, power);

   // no match means cruise
   digitalWrite(m_R_FWD_PIN, mode == MOTOR_FORWARD);
   digitalWrite(m_R_REV_PIN, mode == MOTOR_REVERSE);
}

void Motorboard::forward(int power){
   set_right_motor(power, MOTOR_FORWARD);
   set_left_motor(power, MOTOR_FORWARD);
}

void Motorboard::right(int power){
   set_right_motor(power, MOTOR_REVERSE);
   set_left_motor(power, MOTOR_FORWARD);
}

void Motorboard::left(int power){
   set_right_motor(power, MOTOR_FORWARD);
   set_left_motor(power, MOTOR_REVERSE);
}

void Motorboard::back(int power){
   set_right_motor(power, MOTOR_REVERSE);
   set_left_motor(power, MOTOR_REVERSE);
}

void Motorboard::stop(){
   set_right_motor(0, MOTOR_FORWARD);
   set_left_motor(0, MOTOR_FORWARD);
}

void Motorboard::cruise(){
   set_right_motor(0, MOTOR_CRUISE);
   set_left_motor(0, MOTOR_CRUISE);
}
