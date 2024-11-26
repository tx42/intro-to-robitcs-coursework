#include "motorboard.h"
#include "localisation.h"
#include "hardware_conf.h"

#define SPEED 255                    
#define MILESTONE_LENGTH 10          
#define OBSTACLE_DISTANCE_THRESHOLD 10

Motorboard motorboard;
Localisation loc;

int milestone = 0 ;
unsigned long lastTurnTime = 0;
bool avoidingObstacle = false;

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

    if (isObstacle == LOW && !avoidingObstacle) {
        Serial.println("OBSTACLE DETECTED!");
        motorboard.stop();
        avoidingObstacle = true;
        lastTurnTime = millis();
    }

    if (avoidingObstacle) {
        unsigned long currentTime = millis();
        if (currentTime - lastTurnTime < 1000) {
            Serial.println("Turning to avoid obstacle...");
            motorboard.right(SPEED);
        } else {
            motorboard.forward(SPEED);
            avoidingObstacle = false; 
        }
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

// can i use diffrent speeds when turning or find a way to get a specfic angle, could help. 
// find a way to get it to move back in the same directoon it was walking, so it can turn and avoiding checking angles or having to.
// 
