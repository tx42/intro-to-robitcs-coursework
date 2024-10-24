#define speedPinR 3   // RIGHT PWM pin connect MODEL-X ENA
#define RightDirectPin1  12    //  Right Motor direction pin 1 to MODEL-X IN1 
#define RightDirectPin2  11    // Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6        //  Left PWM pin connect MODEL-X ENB
#define LeftDirectPin1  7    // Left Motor direction pin 1 to MODEL-X IN3
#define LeftDirectPin2  8   ///Left Motor direction pin 1 to MODEL-X IN4

#define BeeperPin 13

#define SPEED   180 // motor speed

#define Trig 10
#define Echo 3
#define ServoPin 9

void go_Advance(void)
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
  set_Motorspeed(SPEED,SPEED);
}


void back_Right(void)
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
    set_Motorspeed(SPEED,0);
}


void back_Left(void)
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
    set_Motorspeed(0,SPEED);
}

void go_Back(void) // Reverse
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
    set_Motorspeed(SPEED,SPEED);
}

void stop_Stop() // Stop
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,LOW);
}

/*set motor speed */
void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(speedPinL,speed_L); 
  analogWrite(speedPinR,speed_R);   
}

void setup()
{
 pinMode(speedPinL,OUTPUT); // left motor PWM pin
 pinMode(speedPinR,OUTPUT); // rignt motor PWM  pin
 pinMode(RightDirectPin1,OUTPUT); // left motor direction pin1
 pinMode(RightDirectPin2,OUTPUT); // left motor direction pin2
 pinMode(LeftDirectPin1,OUTPUT); // right motor direction Pin 1
 pinMode(LeftDirectPin2,OUTPUT); // right motor direction Pin 2

  pinMode(BeeperPin, OUTPUT);
  digitalWrite(BeeperPin, HIGH);

  for(int i = 0; i < 4; i++){
    beep(250);
    delay(500);
  }
  beep(1000);
  do_path();
}

void beep(int duration){
  digitalWrite(BeeperPin, LOW);
  delay(duration);
  digitalWrite(BeeperPin, HIGH);
}

void do_path(){
   go_Advance();
   delay(1000);
   stop_Stop();
   delay(1000);
   back_Right();
   delay(1000);
   stop_Stop();
}

void loop(){
 
}
