// Sample hardware configuration file

// To use:
// - Copy it to the current project directory.
// - Delete the following line:
#error Don't use sample config directly
// - Change pin values according to your setup.


#pragma once

/* MOTORBOARD */
#define MOTOR_R_POWER_PIN 3
#define MOTOR_L_POWER_PIN 6

#define MOTOR_R_FWD_PIN  12
#define MOTOR_R_REV_PIN  11
#define MOTOR_L_FWD_PIN  7
#define MOTOR_L_REV_PIN  8

#define SERVO_PIN 9

/* SENSORS */
#define SONIC_TRIG_PIN 10
#define SONIC_ECHO_PIN 3

#define ENCODER_R_PIN // FILL IN
#define ENCODER_L_PIN // FILL IN

#define IR_R_PIN // FILL IN 
#define IR_L_PIN // FILL IN 

#define LINE_SENSOR_0_PIN A0
#define LINE_SENSOR_1_PIN A1
#define LINE_SENSOR_2_PIN A2
#define LINE_SENSOR_3_PIN A3
#define LINE_SENSOR_4_PIN A4

/* PERIPHERALS */
#define BEEPER_PIN 13

