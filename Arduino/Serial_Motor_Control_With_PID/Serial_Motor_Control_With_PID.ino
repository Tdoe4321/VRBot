/* 
*  For: VRBot Personal Project
*  Version: 2.0
*  By: Tyler Gragg
*  Start Date: 10/01/2019
*/
#include <string.h>
#include <AFMotor.h>

// Pin inputs
int hornPin = A0;
int lightPin = A1;
int encoderPin = A2;

// Motors
AF_DCMotor FL(2);
AF_DCMotor FR(1);
AF_DCMotor BL(3);
AF_DCMotor BR(4);

// Data from Serial Connection
int leftData = 0;
int rightData = 0;
int hornData = 0;
int lightData = 0;
int rotaryData = 0;
int lastRotaryData = 0; //(Not serial, but used for rotary calculations)

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

//Encoder
const int numReadings = 10;

float readings[numReadings];
float average = 0;
float total = 0;
int readingIndex = 0;

bool curState = 0;
bool prevState = 0;

float timeDiff = 0;
float lastSwitchTime = 0;

float revPerSec = 0;

float timeoutThresh = 300;

// PID
unsigned long currentTime;
double elapsedTime;
unsigned long previousTime;

float error;
float cumError;

float rateError;
float lastError;

float kp = 12;
float ki = 0.007;
float kd = .02;

float output;
float pidControl;

void setup() {
  Serial.begin(115200);

  // Motor Setup
  FL.setSpeed(200);
  FR.setSpeed(200);
  BL.setSpeed(200);
  BR.setSpeed(200);

  FL.run(RELEASE);
  FR.run(RELEASE);
  BL.run(RELEASE);
  BR.run(RELEASE);

  // Horn and Light setup
  pinMode(hornPin, OUTPUT);
  pinMode(lightPin, OUTPUT);

  // Encoder Setup
  pinMode(encoderPin, INPUT);
  digitalWrite(encoderPin, HIGH);

  prevState =  digitalRead(encoderPin);
  lastSwitchTime = millis();
  currentTime = millis();
  previousTime = currentTime;

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
    rotaryData = getValue(receivedString, ':', 4).toInt();
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

void runningEncoderAverage(){
  curState =  digitalRead(encoderPin);

  if (prevState != curState){
    timeDiff = ((float)millis()) - lastSwitchTime;
    lastSwitchTime = (float)millis();
    revPerSec = (1.0/8.0) / (timeDiff / 1000.0);
    readings[readingIndex] = revPerSec;
    total = 0;
    for(int i = 0; i < numReadings; i++){
      total = total + readings[i];
    }
    average = total / numReadings;
    readingIndex++;
    if(readingIndex >= numReadings) readingIndex = 0;
  }

  else if(((float)millis()) - lastSwitchTime > timeoutThresh){
    for(int i = 0; i < numReadings; i++){
      readings[i] = 0.0;
      average = 0.0;
      total = 0.0;
    }
  }

  prevState = curState;
}

void calculatePID(double setPoint) {
    currentTime = millis();
    elapsedTime = (currentTime - previousTime);
    
    error = setPoint - average;
    cumError += error * elapsedTime;

    rateError = (error - lastError) / elapsedTime;

    output = (kp * error) + (ki * cumError)+ (kd * rateError);

    lastError = error;
    previousTime = currentTime;

    pidControl = output + 10;
    pidControl = constrain(pidControl, 0, 255);
}

void loop() {
  loadSerialInput();

  runningEncoderAverage();

  // First time, reset data in errors
  if(rotaryData == 0 && lastRotaryData != 0){
    currentTime = millis();
    elapsedTime = 0.0;
    previousTime = millis();

    error = 0.0;
    cumError = 0.0;

    rateError = 0.0;
    lastError = 0.0;

    output = 0.0;
    pidControl = 0.0;
  }

  // If we are in normal drive mode
  if(rotaryData == 0){
    setRightDirection();
    setLeftDirection();
     
    FR.setSpeed(rightData);
    FL.setSpeed(leftData);
    BR.setSpeed(rightData);
    BL.setSpeed(leftData);
  }

  // If we are in speed controlled mode
  else{
    rightF();
    leftF();

    calculatePID(rotaryData);
    
    FR.setSpeed(pidControl);
    FL.setSpeed(pidControl);
    BR.setSpeed(pidControl);
    BL.setSpeed(pidControl);
  }

  lastRotaryData = rotaryData;


  // Always check these
  if(hornData == 1) digitalWrite(hornPin, HIGH);
  else digitalWrite(hornPin, LOW);
  if(lightData == 1) digitalWrite(lightPin, HIGH);
  else digitalWrite(lightPin, LOW);
    
}
