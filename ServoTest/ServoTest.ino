#include <Servo.h>

#define numberOfServos 8 //Does not go higher than 8

#define minROM 10 //Minimum Range of Motion
#define maxROM 170 //Maximum Range of Motion
#define END -1
#define nPositions 4
int pinArray[numberOfServos] = {2 ,3, 4,5, 6,7, 8, 9}; //Add more pins as needed
Servo servoArray[numberOfServos];//all servo motors, initialized in setup

int legPositions[] = {25, 170, 170, 25}; //Degrees of motion that legs/knees can mvoe to
int kneePositions[] = {90, 10, 90, 90};

int motorInstructions[] = {1, 1, END}; //Which motor to move
int moveInstructions[]  = {0, 4, END}; //Where to move it
int i = 0;

void moveMotor(int motoNumber, int pos){//pos is in degrees and will move TO the value given
  servoArray[motoNumber].write(pos); 
}

void moveKnee(int knee, int posIdx){
   moveMotor(knee*2,kneePositions[posIdx]); 
}

void moveLeg(int leg, int posIdx){
   moveMotor(leg*2+1,legPositions[posIdx]); 
}

void step(int legNumber){
    //walk the leg and knee through the full range of motion
    int j;
    for(j=0;j<nPositions;j++){
        moveLeg(legNumber,j);
        moveKnee(legNumber,j); 
        delay(200);
    } 
    moveLeg(legNumber,0);
    moveKnee(legNumber,0);
}

void setup() {
  
  for(int x = 0; x < numberOfServos; x++){//Assign each servo to a pin, array[0] = pinarray[0]
    servoArray[x].attach(pinArray[x]);
  }
 
}


void loop() {  
  //Test: Loop through all the positions on a specific motor
  step(0);
}
