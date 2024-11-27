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
}

void loop(){
    motorboard.setVelocityTurn(40.0, 0.0);
    delay(2000);
    motorboard.setVelocityTurn(40.0, 40.0);
    delay(2000);
    motorboard.setVelocityTurn(40.0, -40.0);
    delay(2000);
    motorboard.setVelocityTurn(-40.0, 0);
    delay(2000);
    motorboard.stop();
    delay(10000);

    while(1);
}
