#define ENC1 A4
#define ENC2 A3

void setup(){
  Serial.begin(9600);
}

int counter = 0;

bool last_value;

void loop(){
  bool value = analogRead(ENC1) > 40;

  if(value){
    if(value != last_value){
      counter++; 
      Serial.println(counter);
    }
  }

  last_value = value;
}
