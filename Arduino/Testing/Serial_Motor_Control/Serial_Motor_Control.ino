/* 
*  For: VRBot Personal Project
*  Version: 0.3
*  By: Tyler Gragg
*  Start Date: 2/01/2019
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
int hornData = 0;
int lightData = 0;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

int hornPin = A0;
int lightPin = A1;


void setup() {
  Serial.begin(115200);

  FL.setSpeed(200);
  FR.setSpeed(200);
  BL.setSpeed(200);
  BR.setSpeed(200);

  FL.run(RELEASE);
  FR.run(RELEASE);
  BL.run(RELEASE);
  BR.run(RELEASE);

  pinMode(hornPin, OUTPUT);
  pinMode(lightPin, OUTPUT);

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

void loadSerialInput(){
    String receivedString(receivedChars);

    leftData = getValue(receivedString, ':', 0).toInt();
    rightData = getValue(receivedString, ':', 1).toInt();
    hornData = getValue(receivedString, ':', 2).toInt();
    lightData = getValue(receivedString, ':', 3).toInt();
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
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

void serialEvent(){
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

    if (newData == true) {
        newData = false;
    }
}

void setLeftDirection(){
  if(leftData > 0){
    leftF();
  }
  else{
    leftB();
    leftData = leftData * -1;
  }
}

void setRightDirection(){
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
  loadSerialInput();
    
  setRightDirection();
  setLeftDirection();
  
  motorEnabled = 1;

  if (motorEnabled != 1) {
    stopAll();
  }

  else {
     
    FR.setSpeed(rightData);
    FL.setSpeed(leftData);
    BR.setSpeed(rightData);
    BL.setSpeed(leftData);

    if(hornData == 1) digitalWrite(hornPin, HIGH);
    else digitalWrite(hornPin, LOW);
    if(lightData == 1) digitalWrite(lightPin, HIGH);
    else digitalWrite(lightPin, LOW);
    
  }
}
