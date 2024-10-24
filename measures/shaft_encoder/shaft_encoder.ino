#define ENC1 A4
#define ENC2 A3

void setup(){
  Serial.begin(9600);
}

void loop(){
  int enc1 = analogRead(ENC1);
  int enc2 = analogRead(ENC2);

  Serial.print(enc1);
  Serial.print(" ");
  Serial.println(enc2);
}
