#include <AFMotor.h>

#define mag_pin A2

//Motors
AF_DCMotor FL(2);
AF_DCMotor FR(1);
AF_DCMotor BL(3);
AF_DCMotor BR(4);

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
float setPoint = 1.0;
float cumError;

float rateError;
float lastError;

float kp = 12;
float ki = 0.007;
float kd = .02;

float output;

void setup() {
  pinMode(mag_pin, INPUT);
  digitalWrite(mag_pin, HIGH);

  prevState =  digitalRead(mag_pin);
  lastSwitchTime = millis();

  FL.setSpeed(200);
  FR.setSpeed(200);
  BL.setSpeed(200);
  BR.setSpeed(200);

  FL.run(RELEASE);
  FR.run(RELEASE);
  BL.run(RELEASE);
  BR.run(RELEASE);

  currentTime = millis();
  previousTime = currentTime;
  
  Serial.begin(115200);
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

void loop() {
  curState =  digitalRead(mag_pin);

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

    rightF();
    leftF();

    String information;

    //PID
    currentTime = millis();
    elapsedTime = (currentTime - previousTime);
    
    error = setPoint - average;
    cumError += error * elapsedTime;

    rateError = (error - lastError) / elapsedTime;

    output = (kp * error) + (ki * cumError)+ (kd * rateError);

    lastError = error;
    previousTime = currentTime;

    double pidControl = output + 50;
    pidControl = constrain(pidControl, 0, 255);
    //End PID

    information += String(error) + " ";
    information += String(kp * error) + " " ;
    information += String(ki * cumError) + " ";
    information += String(kd * rateError) + " ";
    information += String(pidControl) + " ";
    information += String(average); 

    float testNum = 50;

    //FR.setSpeed(pidControl);
    //FL.setSpeed(pidControl);
    //BR.setSpeed(pidControl);
    //BL.setSpeed(pidControl);

    FR.setSpeed(testNum);
    FL.setSpeed(testNum);
    BR.setSpeed(testNum);
    BL.setSpeed(testNum);

  //Serial.println(average);
  Serial.println(information);
  information = "";

  prevState = curState;
}
