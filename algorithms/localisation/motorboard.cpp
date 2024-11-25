#include "motorboard.h"

void Motorboard::init(int r_pwr,
                        int l_pwr,

                        int r_fwd,
                        int r_rev,
                        int l_fwd,
                        int l_rev){
   m_L_POWER_PIN = l_pwr;
   m_R_POWER_PIN = r_pwr;

   m_R_FWD_PIN = r_fwd;
   m_L_FWD_PIN = l_fwd;
   m_R_REV_PIN = r_rev;
   m_L_REV_PIN = l_rev;

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

   l_power = power;
   l_motor_mode = mode;

   analogWrite(m_L_POWER_PIN, power);

   // no match means cruise
   digitalWrite(m_L_FWD_PIN, mode == MOTOR_FORWARD);
   digitalWrite(m_L_REV_PIN, mode == MOTOR_REVERSE);
}

// we do a little copy-pasting
void Motorboard::setRightMotor(int power, MotorMode mode){
   power = constrain(power, 0, 255);

   r_power = power;
   r_motor_mode = mode;
   
   analogWrite(m_R_POWER_PIN, power);

   // no match means cruise
   digitalWrite(m_R_FWD_PIN, mode == MOTOR_FORWARD);
   digitalWrite(m_R_REV_PIN, mode == MOTOR_REVERSE);
}

void Motorboard::forward(int power){
   setRightMotor(power, MOTOR_FORWARD);
   setLeftMotor(power, MOTOR_FORWARD);
}

void Motorboard::right(int power){
   setRightMotor(power, MOTOR_REVERSE);
   setLeftMotor(power, MOTOR_FORWARD);
}

void Motorboard::left(int power){
   setRightMotor(power, MOTOR_FORWARD);
   setLeftMotor(power, MOTOR_REVERSE);
}

void Motorboard::back(int power){
   setRightMotor(power, MOTOR_REVERSE);
   setLeftMotor(power, MOTOR_REVERSE);
}

void Motorboard::stop(){
   setRightMotor(0, MOTOR_FORWARD);
   setLeftMotor(0, MOTOR_FORWARD);
}

void Motorboard::cruise(){
   setRightMotor(0, MOTOR_CRUISE);
   setLeftMotor(0, MOTOR_CRUISE);
}
