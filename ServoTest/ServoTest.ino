#include <Servo.h>

//TODO: Encode characters so each bit in the byte matches a servo. Allow for simultaneous movement of servos.

#define numberOfServos 8 //Does not go higher than 8

#define minROM 10 //Minimum Range of Motion
#define maxROM 170 //Maximum Range of Motion
#define END -1
#define nPositions 4
#define DOWN 90
#define RDOWN 180
#define UP 10
#define RUP 0
#define BACK 25
#define RBACK 170
#define FORWARD 170
#define RFORWARD 25

//direction of knee and leg value, not physical robot
#define DIR_UP 1
#define DIR_DOWN 0

//byte typed by keyboard
byte byteRead;

int pinArray[numberOfServos] = {2 ,3, 4, 5, 6, 7, 8, 9}; //GPIO Pins used
Servo servoArray[numberOfServos];//all servo motors in one array, initialized in setup

//positions as limits [0] - [1] -> low - high
int legBounds[][2] = {{25, 170}, {25, 170}, {25, 170}, {25, 170}};
int kneeBounds[][2] = {{UP, DOWN}, {RUP, RDOWN}, {UP, DOWN}, {RUP, RDOWN}};

int motorInstructions[] = {1, 1, END}; //Which motor to move
int moveInstructions[]  = {0, 4, END}; //Where to move it
int i = 0;

//each character a byte, assign a bit to each motor
//decode so multiple motors can move simultaneously

//current position within sequence
int currKneePos[4] = {DOWN,RDOWN,DOWN,RDOWN};
int currKneeDir[4] = {DIR_UP, DIR_UP, DIR_UP, DIR_UP};
int currLegPos[4] = {BACK, RBACK, BACK, RBACK};
int currLegDir[4] = {DIR_UP, DIR_UP, DIR_UP, DIR_UP};

//pos - position of leg or knee
//isLeg - 0 if leg is being moved, else 1
//Changes position of chosen leg or knee then moves the appropriate part
void moveRobot(int pos, int isLeg) {
  Serial.println("In moveRobot");
  if(pos < 0 || pos > 3) {
    Serial.println("Error: moveRobot incorrect usage of pos");
  }
  if(isLeg == 0) {
     Serial.print("Attempting to move leg ");
     Serial.print(pos);
     Serial.print(" to position ");
     Serial.println(currLegPos[pos]);
     moveLeg(pos);
  }
  else if(isLeg == 1) {
     Serial.print("Attempting to move knee ");
     Serial.print(pos);
     Serial.print(" to position ");
     Serial.println(currKneePos[pos]);
     moveKnee(pos);
  }
  else {
    Serial.println("Error: moveRobot incorrect usage of isLeg");
  }
}

void moveMotor(int motoNumber, int pos){//pos is in degrees and will move TO the value given
  servoArray[motoNumber].write(pos); 
}

void moveKnee(int knee){

   //Move the knee to correct position
   moveMotor(knee*2, currKneePos[knee]);

   //Determine next knee position
   if(currKneeDir[knee] == DIR_UP) {
     currKneePos[knee] += 10;
     if(currKneePos[knee] >= kneeBounds[knee][1]) {
      currKneeDir[knee] = DIR_DOWN;
     }
   }
   else {
     currKneePos[knee] -= 10;
     if(currKneePos[knee] <= kneeBounds[knee][0]) {
      currKneeDir[knee] = DIR_UP;
     }
   }
}

void moveLeg(int leg){

  //move leg to correct position
  moveMotor(leg*2+1,currLegPos[leg]);

  //Determine next leg position
  if(currLegDir[leg] == DIR_UP) {
     currLegPos[leg] += 10;
     if(currLegPos[leg] >= legBounds[leg][1]) {
      currLegDir[leg] = DIR_DOWN;
     }
   }
   else {
     currLegPos[leg] -= 10;
     if(currLegPos[leg] <= legBounds[leg][0]) {
      currLegDir[leg] = DIR_UP;
     }
   }
}

void setup() {
  
  // turn on serial protocol
  Serial.begin(9600);
  
  Serial.println("Setup started");

  //Assign each servo to a pin, array[0] = pinarray[0]
  for(int x = 0; x < numberOfServos; x++){
    servoArray[x].attach(pinArray[x]);
  }

  setupRobot();

  delay(3000);
  Serial.println("Setup finished");
}

//resets robot to default standing position
void setupRobot(){ 

  for(int i=0; i<=3; i++){
    moveLeg(i);
    moveKnee(i);
    delay(100);
  }
}

void loop() {  
}

void serialEvent() {
  Serial.println("Serial event occured!");
  //check for key input
  while(Serial.available() > 0) {
    
    //read most recent byte
    byteRead = Serial.read();
    //Serial.print("Byte received: ");
    //Serial.println(byteRead);
    
    if(bitRead(byteRead, 0)){
      moveRobot(0, 1);
    }
    if(bitRead(byteRead, 1)) {
      moveRobot(0, 0);
    }
    if(bitRead(byteRead, 2)) {
      moveRobot(1, 1);
    }
    if(bitRead(byteRead, 3)) {
      moveRobot(1, 0);
    }
    if(bitRead(byteRead, 4)) {
      moveRobot(2, 1);
    }
    if(bitRead(byteRead, 5)) {
      moveRobot(2, 0);
    }
    if(bitRead(byteRead, 6)) {
      moveRobot(3, 1);
    }
    if(bitRead(byteRead, 7)) {
      moveRobot(3, 0);
    }
    /*else{
      //Unsupported key
      Serial.print("Unsupported key: ");
      Serial.println(keyRead);
    }*/
    delay(100);
  }
}

