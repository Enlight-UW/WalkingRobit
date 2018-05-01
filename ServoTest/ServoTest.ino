#include <Servo.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

//Servos on robot
#define numberOfServos 8

#define DOWN 90
#define RDOWN 180
#define UP 10
#define RUP 130
#define BACK 25
#define RBACK 170
#define FORWARD 170
#define RFORWARD 25

//direction of knee and leg value, not physical robot
#define DIR_UP 1
#define DIR_DOWN 0

//byte sent over serial
byte byteRead;

//RX | TX
SoftwareSerial BTserial(13, 12);

int pinArray[numberOfServos] = {2, 3, 4, 5, 6, 7, 8, 9}; //GPIO Pins used
Servo servoArray[numberOfServos];//all servo motors in one array, initialized in setup

//positions as limits [0] - [1] -> low - high
//Leg full range = 30, 160
int legBounds[][2] = {{40, 150}, {40, 150}, {40, 150}, {40, 150}};
int kneeBounds[][2] = {{60, 80}, {RUP, RDOWN}, {60, 80}, {RUP - 10, RDOWN - 10}};

//current position in degrees
int currKneePos[4] = {50, 90, 50, 80};
int currLegPos[4] = {90, 90, 90, 70};

int startKneePos[4] = {50, 90, 50, 80};
int startLegPos[4] = {90, 90, 90, 70};
int moveMode = 0;
int moveCounter = 0;
int motionModes[5][2] = {
  {0,0},
  {144,9},//up
  {24,129},//down
  {6, 96},//left
  {36,66},//right
};
int motionStarts[5] = {0,(1<<1), (1<<1), (1<<4), (1<<4)};
int isSetup = false;
//pos - position of leg or knee
//isLeg - 1 if leg is being moved, 0 for knee
//Changes position of chosen leg or knee then moves the appropriate part
/*
 * Move a pair of legs or knees incrementally in a direction.
 * @param pairID pair of legs or knees to move (0 or 1)
 * @param isLeg 1 if leg is being moved, 0 for knee
 * @param dir boolean direction to move leg or knee pair
 * 
 */
void moveRobot(int pairID, int isLeg, int dir) {
//  Serial.println("In moveRobot");
  if(pairID < 0 || pairID > 3) {
//    Serial.println("Error: moveRobot incorrect usage of pairID");
  }
  if(isLeg == 1) {
     
     moveLeg(pairID, dir);
     if(dir == DIR_DOWN) {
      moveLeg(pairID+2, DIR_UP);
     }
     else {
      moveLeg(pairID+2, DIR_DOWN);
     }
     /*
     Serial.print("Attempting to move leg ");
     Serial.print(pairID);
     Serial.print(" to position ");
     Serial.println(currLegPos[pairID]);
     Serial.print("Attempting to move leg ");
     Serial.print(pairID+2);
     Serial.print(" to position ");
     Serial.println(currLegPos[pairID+2]);
     */
     
  }
  else if(isLeg == 0) {
     
     moveKnee(pairID, dir);
     moveKnee(pairID+2, dir);
     /*
     Serial.print("Attempting to move knee ");
     Serial.print(pairID);
     Serial.print(" to position ");
     Serial.println(currKneePos[pairID]);
     Serial.print("Attempting to move knee ");
     Serial.print(pairID+2);
     Serial.print(" to position ");
     Serial.println(currKneePos[pairID+2]);
     */
  }
  else {
    //Serial.println("Error: moveRobot incorrect usage of isLeg");
  }
}

/* 
 *  Write to a servo to move motor to exact position.
 *  @param motoNumber number of motor being written to, corresponds to markings on robot
 *  @param position to move the motor to in degrees
 * 
 */
void moveMotor(int motoNumber, int pos){
  servoArray[motoNumber].write(pos); 
}

/* 
 * Move a knee incrementally in a particular direction
 * @param knee index of knee to move (0 - 3), motor = knee * 2 
 * @param dir direction to move the knee
 * 
 */
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

/* 
 * Move a leg incrementally in a particular direction
 * @param leg index of leg to move (0 - 3), motor = (leg * 2) + 1 
 * @param dir direction to move the knee
 * 
 */
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

/* 
 * Initial setup of robot. Setup servo array and move robot to resting position.
 */
void setup() {
  
  // turn on serial protocol
  //Serial.begin(9600);
  
  //Serial.println("Setup started");

  //Assign each servo to a pin, array[0] = pinarray[0]
  for(int x = 0; x < numberOfServos; x++){
    servoArray[x].attach(pinArray[x]);
  }

  //read current positions in from EEProm
  /*for(int i = 0; i < 4; i++) {
    currKneePos[i] = EEPROM.read(i);
    currLegPos[i] = EEPROM.read(i+4);
  }*/

  setupRobot();

  //setup wireless receiver
  BTserial.begin(9600);

  //delay(3000);
  //Serial.println("Setup finished");
}

/* 
 * Resets robot to default standing position. Moves each motor.
 */
void setupRobot() { 
  for(int i=0; i<=3; i++){
    moveMotor(i*2, startKneePos[i]);
    moveMotor(i*2 + 1, startLegPos[i]);
    delay(50);
  }
}

void loop() {
   readInput();
}

/* 
 * Receive incoming commands and move appropriate motors.
 */
void readInput() {
  //Serial.println("Serial event occured!");
  //check for serial input
  if(true/*BTserial.available() > 0*/) {
    
    //read most recent byte
    if(BTserial.available() > 0) {
      moveMode = BTserial.read();
      moveCounter = 0;
      if(moveMode == 0 && !isSetup){
        isSetup = true; 
        setupRobot();
      }else if (moveMode != 0){
        isSetup = false; 
      }
    }
    /*
    else {
      byteRead = Serial.read();
    }
    */
    
    //Serial.print("Byte received: ");
    //Serial.println(byteRead);
    

    //TEMPORARY DEBUG
    /*char keyRead = byteRead;

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
    }*/

    //TEMPORARY DEBUG END
    moveCounter++;
    if(moveCounter < 2 && moveMode > 0){
        byteRead = motionStarts[moveMode];
    }
    else{
        byteRead = motionModes[moveMode][(moveCounter/10)%2];
    }
    //pair - isLeg - dir
    if(bitRead(byteRead, 0)){
      moveRobot(0, 0, 0);
    }
    if(bitRead(byteRead, 1)) {
      moveRobot(1, 0, 0);
    }
    if(bitRead(byteRead, 2)) {
      moveRobot(0, 1, 1);
    }
    if(bitRead(byteRead, 3)) {
      moveRobot(1, 1, 1);
    }
    if(bitRead(byteRead, 4)) {
      moveRobot(0, 0, 1);
    }
    if(bitRead(byteRead, 5)) {
      moveRobot(1, 0, 1);
    }
    if(bitRead(byteRead, 6)) {
      moveRobot(0, 1, 0);
    }
    if(bitRead(byteRead, 7)) {
      moveRobot(1, 1, 0);
    }

    //update EEPROM
    /*for(int i = 0; i < 4; i++) {
      EEPROM.write(i, currKneePos[i]);
      EEPROM.write(i+4, currLegPos[i]);
    }*/

    delay(100);
  }
  delay(100);
}

