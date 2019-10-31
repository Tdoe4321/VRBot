/*  Code: BattleBot
*  Version: 0.1
*  By: Tyler Gragg
*  Date: 2/01/2019
*  For: VRBot Personal Project
*/
#include <string.h>
#include <AFMotor.h>

AF_DCMotor FL(2);
AF_DCMotor FR(1);
AF_DCMotor BL(3);
AF_DCMotor BR(4);

int motorEnabled = 0;     // values for turning on system

int leftData = 0;
int rightData = 0;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;


void setup() {
  Serial.begin(9600);

  FL.setSpeed(200);
  FR.setSpeed(200);
  BL.setSpeed(200);
  BR.setSpeed(200);

  FL.run(RELEASE);
  FR.run(RELEASE);
  BL.run(RELEASE);
  BR.run(RELEASE);

  //pinMode(13, OUTPUT);
}

// Turn right motor forword
void rightF() {
  FR.run(FORWARD);
  BR.run(FORWARD);
}

// Turn left motor forword
void leftF() {
  FL.run(FORWARD);
  BL.run(FORWARD);
}

// Turn right motor backword
void rightB() {
  FR.run(BACKWARD);
  BR.run(BACKWARD);
}

// Turn left motor backword
void leftB() {
  FL.run(BACKWARD);
  BL.run(BACKWARD);
}

void stopAll(){
  FL.run(RELEASE);
  FR.run(RELEASE);
  BL.run(RELEASE);
  BR.run(RELEASE);
}

void serialInput(){
    recvWithEndMarker();
    showNewData();
    String receivedString(receivedChars);
    
    if(receivedString.charAt(0) == 'L'){
      //leftData = receivedString.substring(1).toFloat();
      //leftData = strtok_r(receivedString,'L');
      leftData = -255;
    }
    else if(receivedString.charAt(0) == 'R'){
      //rightData = receivedString.substring(1).toFloat();
      //rightData = strtok_r(receivedString,'R');
      rightData = 0;
    }
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '>';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void setDirection(){
  if(leftData > 0){
    leftF();
  }
  else{
    leftB();
    leftData = leftData * -1;
  }
  if(rightData > 0){
    rightF();
  }
  else{
    rightB();
    rightData = rightData * -1;
  }
  
}

void showNewData() {
    if (newData == true) {
        newData = false;
    }
}

void loop() {
  serialInput();
    
  setDirection();

  motorEnabled = 1;

  if (motorEnabled != 1) {
    stopAll();
  }

  else {
     
    FR.setSpeed(rightData);
    FL.setSpeed(leftData);
    BR.setSpeed(rightData);
    BL.setSpeed(leftData);
    
  }
}

