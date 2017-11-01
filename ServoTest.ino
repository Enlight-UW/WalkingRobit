#include <Servo.h>

  #define numberOfServos 2 //Does not go higher than 8
  #define Centered 90 //Hip is centered
  #define minROM 10 //Minimum Range of Motion
  #define maxROM 170 //Maximum Range of Motion
  
  int pinArray[numberOfServos] = {2,3}; //Add more pins as needed

 Servo servoArray[numberOfServos];//Init all servo motors

void moveMotor(int motoNumber, int pos){//pos is in degrees and will move TO the value given

  servoArray[motoNumber].write(pos); 
 
}


void setup() {
  
  for(int x = 0; x < numberOfServos; x++){//Assign each servo to a pin, array[0] = pinarray[0]
    servoArray[x].attach(pinArray[x]);
  }
 
}


void loop() {
  
  for(int x = 0; x < numberOfServos; x++){//For each servo
    for(int pos = minROM; pos <= maxROM; pos += 1){//Move motor from min to max
      moveMotor(x,pos);
      delay(20);
    }
  }
}
