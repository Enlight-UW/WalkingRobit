#include <Servo.h>

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

//byte typed by keyboard
int byteRead;
char keyRead;

int pinArray[numberOfServos] = {2 ,3, 4, 5, 6, 7, 8, 9}; //GPIO Pins used
Servo servoArray[numberOfServos];//all servo motors in one array, initialized in setup

//int legPositions[] = {25, 170, 170, 25}; //Degrees of motion that legs/knees can mvoe to
int legPositions[][4] = {{FORWARD, 120, 80, BACK}, {120, RBACK, RFORWARD, 80}, {BACK, FORWARD, 120, 80}, {80, 120, RBACK, RFORWARD}};
                                         //Make this a 2D array, 2 and maybe 3 are reverse of 0 and 1
//int kneePositions[][4] = {{UP, DOWN, UP, UP}, {180, 0, 180, 180}, {UP, DOWN, UP, UP}, {180, 0, 180, 180}};
int kneePositions[][4] = {{UP, DOWN, DOWN, DOWN}, {RDOWN, RDOWN, RUP, RDOWN}, {DOWN, UP, DOWN, DOWN}, {RDOWN, RDOWN, RDOWN, RUP}};

int motorInstructions[] = {1, 1, END}; //Which motor to move
int moveInstructions[]  = {0, 4, END}; //Where to move it
int i = 0;

//current position within sequence
int currKneePos[4] = {0,0,0,0};
int currLegPos[4] = {0,0,0,0};

//pos - position of leg or knee
//isLeg - 0 if leg is being moved, else 1
//Changes position of chosen leg or knee then moves the appropriate part
void moveRobot(int pos, int isLeg) {
  Serial.println("In moveRobot");
  if(pos < 0 || pos > 3) {
    Serial.println("Error: moveRobot incorrect usage of pos");
  }
  if(isLeg == 0) {
     currLegPos[pos]++;
     if(currLegPos[pos] >= nPositions) {
      currLegPos[pos] = 0;
     }
     Serial.print("Attempting to move leg ");
     Serial.print(pos);
     Serial.print(" to position ");
     Serial.println(currLegPos[pos]);
     moveLeg(pos, currLegPos[pos]);
  }
  else if(isLeg == 1) {
    currKneePos[pos]++;
     if(currKneePos[pos] >= nPositions) {
      currKneePos[pos] = 0;
     }
     moveKnee(pos, currKneePos[pos]);
  }
  else {
    Serial.println("Error: moveRobot incorrect usage of isLeg");
  }
}

void moveMotor(int motoNumber, int pos){//pos is in degrees and will move TO the value given
  servoArray[motoNumber].write(pos); 
}

void moveKnee(int knee, int posIdx){
   moveMotor(knee*2,kneePositions[knee][posIdx]); 
}

void moveLeg(int leg, int posIdx){
   moveMotor(leg*2+1,legPositions[leg][posIdx]); 
}

void step(int legNumber){
    //walk the leg and knee through the full range of motion
    int j;
    for(j=0;j<nPositions;j++){
        moveLeg(legNumber,j);
        moveKnee(legNumber,j); 
        delay(1000);
    } 
    moveLeg(legNumber,0);
    moveKnee(legNumber,0);
}

//Takes a full step with all four legs
void walk() {
  /*step(0);
  step(1);
  step(2);
  step(3);*/
  for(int pos=0;pos<nPositions;pos++){
    for(int legNumber = 0; legNumber < 4; legNumber++) {
      moveLeg(legNumber,pos);
      moveKnee(legNumber,pos);
    }   
      delay(100);
  }
  
}

void setup() {
  
  // turn on serial protocol
  Serial.begin(9600);
  
  Serial.println("Setup started");
  
  for(int x = 0; x < numberOfServos; x++){//Assign each servo to a pin, array[0] = pinarray[0]
    servoArray[x].attach(pinArray[x]);
  }

  getShitSetUp();

  delay(3000);
  Serial.println("Setup finished");
}

void getShitSetUp(){ //comments are for communists

  for(int i=0; i<=3; i++){
    moveLeg(i, 0);
    moveKnee(i, 0);
    delay(100);
  }
}

//wait for keyboard input and respond
void loop() {  
}

void serialEvent() {
  Serial.println("Serial event occured!");
  //check for key input
  while(Serial.available() > 0) {
    
    //read most recent byte
    byteRead = Serial.read();
    keyRead = byteRead;
    //Serial.print("Key pressed: ");
    //Serial.println(keyRead);
    
    if(keyRead == 'a'){
      moveRobot(0, 1);
    }
    else if(keyRead == 's') {
      moveRobot(1, 1);
    }
    else if(keyRead == 'd') {
      moveRobot(2, 1);
    }
    else if(keyRead == 'f') {
      moveRobot(3, 1);
    }
    else if(keyRead == 'h') {
      moveRobot(0, 0);
    }
    else if(keyRead == 'j') {
      moveRobot(1, 0);
    }
    else if(keyRead == 'k') {
      moveRobot(2, 0);
    }
    else if(keyRead == 'l') {
      moveRobot(3, 0);
    }
    else{
      //Unsupported key
      Serial.print("Unsupported key: ");
      Serial.println(keyRead);
    }
    delay(100);
  }
}

