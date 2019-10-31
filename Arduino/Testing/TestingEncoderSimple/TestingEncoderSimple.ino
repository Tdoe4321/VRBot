
#define mag_pin A2

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

  Serial.println(curState);
}
