#include "motorboard.h"
#include "localisation.h"
#include "hardware_conf.h"


// #define SPEED                      
// #define MILESTONE_LENGTH            
// #define OBSTACLE_DISTANCE_THRESHOLD  

Motorboard motorboard;
Localisation loc;

// int milestone = ;

void setup() {
    motorboard.init(
        MOTOR_R_POWER_PIN, 
        MOTOR_L_POWER_PIN,
        MOTOR_R_FWD_PIN,
        MOTOR_R_REV_PIN,
        MOTOR_L_FWD_PIN,
        MOTOR_L_REV_PIN
    );

    loc.init(
        motorboard,
        ENCODER_R_PIN,
        ENCODER_L_PIN,
        WHEEL_RADIUS,
        LR_WHEEL_DIST,
        ENCODER_SLIT_NUM
    );

    pinMode(isObstaclePin, INPUT); 
    Serial.begin(9600);           
}

void loop() {
    int isObstacle = digitalRead(isObstaclePin); 

    if (isObstacle == LOW) { 
        Serial.println("OBSTACLE DETECTED!");
        motorboard.stop();       
        delay(1000);             

        /// Performing obstacle avoidance
        Serial.println("Turning to avoid obstacle...");
        motorboard.right(SPEED);
        delay(1000);            
        motorboard.forward(SPEED);
    } else {
        if (milestone * MILESTONE_LENGTH < loc.y) {
            Serial.print("Milestone reached at distance: ");
            Serial.println(loc.y);
            
            motorboard.stop();   
            delay(2000);         
            
            milestone++;         
        }

        motorboard.forward(SPEED);
    }

    delay(100); 
}
