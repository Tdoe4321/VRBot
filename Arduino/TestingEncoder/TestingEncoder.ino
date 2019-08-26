
#define mag_pin A0

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

float timeoutThresh = 100;

void setup() {
  // put your setup code here, to run once:
  pinMode(mag_pin, INPUT);
  digitalWrite(mag_pin, HIGH);

  prevState =  digitalRead(mag_pin);
  lastSwitchTime = millis();
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
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

  Serial.println(average);

  prevState = curState;
}
