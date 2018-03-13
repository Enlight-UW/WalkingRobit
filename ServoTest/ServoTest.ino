#include <Servo.h>

//TODO: Tie opposite legs and knees together
//TODO: Change buttons to forward and backward for each knee and leg pair 

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

int pinArray[numberOfServos] = {2, 3, 4, 5, 6, 7, 8, 9}; //GPIO Pins used
Servo servoArray[numberOfServos];//all servo motors in one array, initialized in setup

//positions as limits [0] - [1] -> low - high
int legBounds[][2] = {{30, 160}, {30, 160}, {30, 160}, {20, 170}};
int kneeBounds[][2] = {{40, 80}, {RUP, RDOWN}, {40, 80}, {RUP, RDOWN}};

int motorInstructions[] = {1, 1, END}; //Which motor to move
int moveInstructions[]  = {0, 4, END}; //Where to move it
int i = 0;

//current position within sequence
int currKneePos[4] = {50, 90, 50, 90};
int currKneeDir[4] = {DIR_UP, DIR_UP, DIR_UP, DIR_UP};
int currLegPos[4] = {90, 90, 90, 70};
int currLegDir[4] = {DIR_UP, DIR_UP, DIR_UP, DIR_UP};

//pos - position of leg or knee
//isLeg - 0 if leg is being moved, else 1
//Changes position of chosen leg or knee then moves the appropriate part
void moveRobot(int pairID, int isLeg, int dir) {
  Serial.println("In moveRobot");
  if(pairID < 0 || pairID > 3) {
    Serial.println("Error: moveRobot incorrect usage of pairID");
  }
  if(isLeg == 0) {
     Serial.print("Attempting to move leg ");
     Serial.print(pairID);
     Serial.print(" to position ");
     Serial.println(currLegPos[pairID]);
     Serial.print("Attempting to move leg ");
     Serial.print(pairID+2);
     Serial.print(" to position ");
     Serial.println(currLegPos[pairID+2]);
     moveLeg(pairID, dir);
     //moveLeg(pairID+2, dir);
     if(dir == DIR_DOWN) {
      moveLeg(pairID+2, DIR_UP);
     }
     else {
      moveLeg(pairID+2, DIR_DOWN);
     }
     
  }
  else if(isLeg == 1) {
     Serial.print("Attempting to move knee ");
     Serial.print(pairID);
     Serial.print(" to position ");
     Serial.println(currKneePos[pairID]);
     Serial.print("Attempting to move knee ");
     Serial.print(pairID+2);
     Serial.print(" to position ");
     Serial.println(currKneePos[pairID+2]);
     moveKnee(pairID, dir);
     moveKnee(pairID+2, dir);
  }
  else {
    Serial.println("Error: moveRobot incorrect usage of isLeg");
  }
}

void moveMotor(int motoNumber, int pos){//pos is in degrees and will move TO the value given
  servoArray[motoNumber].write(pos); 
}

void moveKnee(int knee, int dir) {
  
   //Determine next knee position
   if(dir == DIR_UP) {
     currKneePos[knee] += 10;
     if(currKneePos[knee] > kneeBounds[knee][1]) {
      currKneePos[knee] = kneeBounds[knee][1];
     }
   }
   else {
     currKneePos[knee] -= 10;
     if(currKneePos[knee] < kneeBounds[knee][0]) {
      currKneePos[knee] = kneeBounds[knee][0];
     }
   }

   //Move the knee to correct position
   moveMotor(knee*2, currKneePos[knee]);
}

void moveLeg(int leg, int dir) {

  //Determine next leg position
  if(dir == DIR_UP) {
     currLegPos[leg] += 10;
     if(currLegPos[leg] > legBounds[leg][1]) {
      currLegPos[leg] = legBounds[leg][1];
     }
   }
   else {
     currLegPos[leg] -= 10;
     if(currLegPos[leg] < legBounds[leg][0]) {
      currLegPos[leg] = legBounds[leg][0];
     }
   }

  //move leg to correct position
  moveMotor(leg*2+1,currLegPos[leg]);
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
void setupRobot() { 
  for(int i=0; i<=3; i++){
    moveMotor(i*2, currKneePos[i]);
    moveMotor(i*2 + 1, currLegPos[i]);
    delay(100);
  }
}

void loop() {  
}

//Receive incoming commands and move appropriate motors
void serialEvent() {
  Serial.println("Serial event occured!");
  //check for key input
  while(Serial.available() > 0) {
    
    //read most recent byte
    byteRead = Serial.read();
    //Serial.print("Byte received: ");
    //Serial.println(byteRead);
    

    //TEMPORARY DEBUG
    char keyRead = byteRead;

    Serial.print("Key received: ");
    Serial.println(keyRead);

    //knee pair 1
    if(keyRead == 'q') {
      byteRead = 0x08;
    }
    if(keyRead == 'w') {
      byteRead = 0x04;
    }
    //knee pair 2
    if(keyRead == 'a') {
      byteRead = 0x80;
    }
    if(keyRead == 's') {
      byteRead = 0x40;
    }
    //leg pair 1
    if(keyRead == 'o') {
      byteRead = 0x01;
    }
    if(keyRead == 'p') {
      byteRead = 0x02;
    }
    //leg pair 2
    if(keyRead == 'k') {
      byteRead = 0x10;
    }
    if(keyRead == 'l') {
      byteRead = 0x20;
    }

    //TEMPORARY DEBUG END

    Serial.print("Byte received: ");
    Serial.println(byteRead);
    
    
    if(bitRead(byteRead, 0)){
      moveRobot(0, 0, 0);
    }
    if(bitRead(byteRead, 1)) {
      moveRobot(0, 0, 1);
    }
    if(bitRead(byteRead, 2)) {
      moveRobot(0, 1, 0);
    }
    if(bitRead(byteRead, 3)) {
      moveRobot(0, 1, 1);
    }
    if(bitRead(byteRead, 4)) {
      moveRobot(1, 0, 0);
    }
    if(bitRead(byteRead, 5)) {
      moveRobot(1, 0, 1);
    }
    if(bitRead(byteRead, 6)) {
      moveRobot(1, 1, 0);
    }
    if(bitRead(byteRead, 7)) {
      moveRobot(1, 1, 1);
    }
    /*else{
      //Unsupported key
      Serial.print("Unsupported key: ");
      Serial.println(keyRead);
    }*/
    delay(100);
  }
}

