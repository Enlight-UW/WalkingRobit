#include <Servo.h>

#define numberOfServos 8 //Does not go higher than 8
#define Centered 90 //Hip is centered
#define minROM 10 //Minimum Range of Motion
#define maxROM 170 //Maximum Range of Motion
#define END -1
int pinArray[numberOfServos] = {2,3, 4,5, 6,7, 8,9}; //Add more pins as needed
Servo servoArray[numberOfServos];//Init all servo motors
int Positions[] = {10, 50, 90, 130, 170};

int moveInstructions[] = {0,0, 1,0, 0,4, //knee1 up, leg1 forward, knee1 down 
                          6,0, 7,0, 6,4, //knee3 up, leg3 forward, knee3 down
                          0,0, 1,4, 0,4, //knee1 up, leg1 back, knee1 down
                          6,0, 7,4, 6,4, //knee3 up, leg3 back, knee3 down
                          END,END};
int i = 0;

void moveMotor(int motoNumber, int pos){//pos is in degrees and will move TO the value given
  servoArray[motoNumber].write(Positions[pos]); 
 //test
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
  moveMotor(moveInstructions[i],moveInstructions[i+1]);
  i += 2;
  if(moveInstructions[i]==END)
    i=0; 
}
