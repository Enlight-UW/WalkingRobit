#include <Servo.h>

#define numberOfServos 8 //Does not go higher than 8
#define Centered 90 //Hip is centered
#define minROM 10 //Minimum Range of Motion
#define maxROM 170 //Maximum Range of Motion
#define END -1
#define nPositions 5
int pinArray[numberOfServos] = {2,3, 4,5, 6,7, 8,9}; //Add more pins as needed
Servo servoArray[numberOfServos];//all servo motors, initialized in setup
int Positions[] = {10, 50, 90, 130, 170}; //Degrees of motion that legs/knees can mvoe to

int motorInstructions[] = {1, 1, END}; //Which motor to move
int moveInstructions[]  = {0, 4, END}; //Where to move it
int i = 0;

void moveMotor(int motoNumber, int pos){//pos is in degrees and will move TO the value given
  servoArray[motoNumber].write(Positions[pos]); 
}

void moveKnee(int knee, int pos){
   moveMotor(knee*2,pos); 
}

void moveLeg(int leg, int pos){
   moveMotor(leg*2+1,pos); 
}


void setup() {
  
  for(int x = 0; x < numberOfServos; x++){//Assign each servo to a pin, array[0] = pinarray[0]
    servoArray[x].attach(pinArray[x]);
  }
 
}


void loop() {  
  //Test: Loop through all the positions on a specific motor
  moveMotor(0,i); 
  i = (i+1)%nPositions;
  delay(500);
}
