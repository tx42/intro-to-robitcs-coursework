void setup(){
   // write initialisation code here
   pinMode(LED, OUTPUT);
   pinMode(isObstaclePin, INPUT);
   Serial.begin(9600); 
}

void loop(){
   // write code that loops here
   isObstacle = digitalRead(isObstaclePin);
  if (isObstacle == LOW)
  {
    Serial.println("OBSTACLE!!, OBSTACLE!!");
    digitalWrite(LED, HIGH);
  }
  else
  {
    Serial.println("clear");
    digitalWrite(LED, LOW);
  }
  delay(200);
}
