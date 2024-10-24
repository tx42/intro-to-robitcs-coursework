/*
   Program to record and log data from ultrasonic sensor 
*/

const int  pingPin = 10; // Trigger signal
const int echoPin = 2; // Echo signal
const int maxNumMeasures = 30;

long cur_real_distance;
int num_measures = 0;

void pingSensor(){
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
}

// tof - time of flight in microseconds
// returns - distance in centimeters
double tofToDistance(long tof){
  return 34.0*(double)tof/1000.0/2.0;
}

void print_header(){
  Serial.println("actual dist\tmeasured dist\ttof");
}

void request_dist_update(){
  Serial.println("enter measure");
  while(Serial.available() == 0);
  update_real_distance();
}

void set_real_distance(long real_distance){
  Serial.print("---- ");
  Serial.print(real_distance);
  Serial.print(" ----");
  Serial.println();

  cur_real_distance = real_distance;
}

// updates real distance using data from serial
void update_real_distance(){
  if(Serial.available() == 0) return;

  int new_real_distance = Serial.parseInt();
  set_real_distance(new_real_distance);

  // consume any remaining bytes
  while(Serial.read() != -1);
}

void log_results(double distance, long duration){
  Serial.print(cur_real_distance);
  Serial.print("\t");
  Serial.print(distance);
  Serial.print("\t");
  Serial.print(duration);
  Serial.println();
}

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(pingPin, OUTPUT); // intitiate PING transmit
  pinMode(echoPin, INPUT); // initiate PING echo receive

  print_header();
  request_dist_update();
}
 
void loop() {
  if(num_measures > maxNumMeasures){
    // discard any data before
    while(Serial.read() != -1);
    request_dist_update();
    num_measures = 0;
  }

  pingSensor(); 
  
  long duration = pulseIn(echoPin, HIGH);
  double distance = tofToDistance(duration);
  
  log_results(distance, duration);
  num_measures++;
}
 
