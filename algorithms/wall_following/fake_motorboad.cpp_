/*
 * TO USE RENAME ANY CONFLICTING .CPP FILES
*/

#include "motorboard.h"

#define log(msg) Serial.println("FAKE_MOT: " msg)
#define startmlog(msg) Serial.print("FAKE_MOT: " msg)
#define addmlog(msg) Serial.print(msg)
#define endmlog() Serial.println()


void Motorboard::init(int r_pwr,
                        int l_pwr,

                        int r_fwd,
                        int r_rev,
                        int l_fwd,
                        int l_rev){
    
    log("motorboard initialised");
}

void Motorboard::setLeftMotor(int power, MotorMode mode){}
void Motorboard::setRightMotor(int power, MotorMode mode){}
void Motorboard::setSignedLeftMotor(int power){}

void Motorboard::setSignedRightMotor(int power){}

void Motorboard::forward(int power){
   log("forward");
}

void Motorboard::right(int power){
   log("right");
}

void Motorboard::left(int power){
    log("left");
}

void Motorboard::back(int power){
   log("back");
}

void Motorboard::stop(){
   log("stop");
}

void Motorboard::cruise(){
   log("cruise");
}

void Motorboard::setVelocityTurn(float vel, float turn){
   startmlog("vel: ");
   addmlog(vel);
   addmlog(" turn:");
   addmlog(turn);
   endmlog();
}
