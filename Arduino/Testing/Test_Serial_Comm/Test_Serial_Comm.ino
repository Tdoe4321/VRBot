#include <AFMotor.h>

AF_DCMotor FR(1);

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;


void setup() {
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
    
  FR.setSpeed(200);
  FR.run(RELEASE);
  FR.setSpeed(0);
  FR.run(FORWARD);

}


void loop() {
    FR.run(FORWARD);
    FR.setSpeed(0);
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = 'L';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        FR.setSpeed(255);
        newData = false;
    }
}
