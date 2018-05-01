int button[] = {3,5,7,9,2,4,6,8};
byte output = 0;
int buttonState[] = {0,0,0,0,0,0,0,0};
int val = 0;


void setup() {
  for(int i=0;i<8;i++){
    pinMode(button[i],INPUT_PULLUP);
  }
  Serial.begin(9600);

}

void loop() {
  for(int i=0;i<8;i++){
    val = digitalRead(button[i]);
    if(val == LOW){
      buttonState[i] = 1;
    }else{
      buttonState[i] = 0;
    }
  }

  for(int i=0;i<8;i++){
    if(buttonState[i] == 1){
      bitSet(output,i);
    }else{
      bitClear(output,i);
    }
  }

  if(output != 0){
    Serial.write(output);
    delay(100);
  }

}
