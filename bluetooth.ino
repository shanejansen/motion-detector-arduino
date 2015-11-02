#include <SoftwareSerial.h>

//constants
const int READY_LED = 5; //LED will turn off when the device is ready
const int RX_PIR = 8;
const int RX_BLUE = 3;
const int TX_BLUE = 4;

//data
boolean motionTransmitInProgress = false; //motion was detected and data is being sent/received to/from bluetooth
long motionTimeSent; //keeps track of when motion data was sent so we can resend later if needed
boolean isWaiting = false; //true if we are waiting a period of time to not send motion data (activated by client)
long waitingTime; //time when the waiting period has started

//instances
SoftwareSerial blSerial(RX_BLUE, TX_BLUE); //Bluetooth module; RX, TX, respectively

void setup()
{
  Serial.begin(9600);

  //set pins and serial
  pinMode(READY_LED, OUTPUT);
  pinMode(RX_PIR, INPUT);
  blSerial.begin(9600);

  //initialize
  digitalWrite(READY_LED, HIGH);
  Serial.print("calibrating PIR sensor");
  for (int i = 0; i < 15; i++) { //give PIR sensor 15 seconds to calibrate
    Serial.print('.');
    delay(1000);
  }
  digitalWrite(READY_LED, LOW);
  Serial.println("\nPIR sensor ready");
  delay(50);
}


void loop()
{
  //PIR sensor detected motion
  if (!motionTransmitInProgress && !isWaiting && digitalRead(RX_PIR) == HIGH) {
    Serial.println("motion detected");
    motionTransmitInProgress = true;
    blSerial.print('m'); //send "m" to bluetooth
    motionTimeSent = millis();
    delay(50);
  }
  
  //PIR sensor data sent. waiting for response
  if (motionTransmitInProgress) {
    if (millis() - motionTimeSent >= 5000) { //if we have not received a response within 5 seconds
      //resend the motion detected datum
      Serial.println("no response; resending data");
      blSerial.print('m'); //send "m" back
      motionTimeSent = millis();
      delay(50);
    }
    else if (blSerial.available()) { //data received
      if (blSerial.read() == 'm') { //connected device received data. continue normally.
        Serial.println("motion was received; continue normally");
        motionTransmitInProgress = false;
        delay(5000);
      }
      delay(50);
    }
  }
  
  //check if wait command was received
  if (!motionTransmitInProgress) {
    if (blSerial.available()) { //data received
      if (blSerial.read() == 'w') { //wait command received
        blSerial.print('w'); //send "w" back
        isWaiting = true;
        waitingTime = millis();
        delay(50);
      }
    }
  }
  
  //check if the wait time has exceeded 1 minute
  if (isWaiting && millis() - waitingTime >= 60000) {
    isWaiting = false;
  }
  
}
