#include "ultrasonic.h"

Ultrasonic sensor;

const int  pingPin = 10; // Trigger signal
const int echoPin = 2; // Echo signal

void pingSensor(){
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
}

// tof - time of flight in microseconds
// returns - distance in centimeters
static double tofToDistance(long tof){
  return 34.0*(double)tof/1000.0/2.0;
}

void setup(){
    sensor.init(pingPin, echoPin);

    Serial.begin(9600);
}

void loop(){
    float dist = sensor.measure();
    Serial.print("driver: ");
    Serial.print(dist);
    Serial.print(" cm\t");

    pingSensor(); 

    long duration = pulseIn(echoPin, HIGH);
    double distance = tofToDistance(duration);

    Serial.print("direct: ");
    Serial.print(distance);
    Serial.print(" cm");
    Serial.print("\t");
    Serial.print(duration);
    Serial.println();

    delay(100);
}
 
