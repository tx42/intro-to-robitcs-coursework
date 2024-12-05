#include "hardware_conf.h"
#include "motorboard.h"

Motorboard motorboard;

void setup(){
    motorboard.init(
        MOTOR_R_POWER_PIN, 
        MOTOR_L_POWER_PIN,
        MOTOR_R_FWD_PIN,
        MOTOR_R_REV_PIN,
        MOTOR_L_FWD_PIN,
        MOTOR_L_REV_PIN
   );

   Serial.begin(9600);
   Serial.println("started");
}

void loop(){
    if(Serial.available()){
        float vel = Serial.parseInt() * 0.01;
        float turn = Serial.parseInt() * 0.01;
        while(Serial.available()) Serial.read();

        motorboard.setVelocityTurn(vel, turn);
    }
}
