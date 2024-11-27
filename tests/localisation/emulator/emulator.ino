#include "motorboard.h"
#include "_localisation.h"
#include "hardware_conf.h"

Motorboard motorboard;
Localisation loc;

void setup(){
   motorboard.init(
      MOTOR_R_POWER_PIN, 
      MOTOR_L_POWER_PIN,
      MOTOR_R_FWD_PIN,
      MOTOR_R_REV_PIN,
      MOTOR_L_FWD_PIN,
      MOTOR_L_REV_PIN
   );

   loc.init(
      &motorboard,
      ENCODER_R_PIN,
      ENCODER_L_PIN,
      WHEEL_RADIUS,
      LR_WHEEL_DIST,
      ENCODER_SLIT_NUM
   );

   Serial.begin(9600);
}

bool execUserCommand(){
    
    char first_char = Serial.read();

    if(first_char == '\n'){
        // update command
        loc.update();
        return true;
    }
    char motor_select = first_char;
    char mode_select = Serial.read();

    while(Serial.available() > 0) Serial.read();

    MotorMode mode;
    if(mode_select == 'f'){
        mode = MOTOR_FORWARD;
    }else if(mode_select == 'r'){
        mode = MOTOR_REVERSE;
    }else{
        return false;
    }

    if(motor_select == 'l'){
        motorboard.setLeftMotor(0, mode);
    }else if(motor_select == 'r'){
        motorboard.setRightMotor(0, mode);
    }else{
        return false;
    }

    return true;
}

void loop(){
    if(Serial.available() > 0){
        if(execUserCommand()){
            Serial.print("Command accepted");
        }else{
            Serial.print("Invalid command");
        }
    }    

    Serial.print(loc.x);
    Serial.print('\t');
    Serial.print(loc.y);
    Serial.print('\t');
    Serial.print(loc.angle);
    Serial.print('\n');

    loc.sample();
    delay(100);
}
