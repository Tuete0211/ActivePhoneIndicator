/**
 * Simple Receiver
 * 
 * Purpose:
 *    Move a Servo up or down
 *    Turn on an off LED Strip
 *    Give the LED Strip a simple color pattern
 *  
 * How it works:
 *    By checking if a message from an 433 MHz receiver is available a global variable is set.
 *    It the variable is true the servo turns and the LED Strip is activated.
 *    If the variable is false the servo is turning in th eopposite direction and the LED Strip is reset.
 *    To secure the servo maximal and minimal values have to be set
 *    To control the LED Strip the library FastLED is used
 * 
 * Todo:
 *    FastLED library
 *    connecting an LED Strip
 * 
 */

#include <Servo.h>
#include <RCSwitch.h>

Servo myServo;  // create servo object to control a servo
RCSwitch mySwitch = RCSwitch();

int myServoPos = 0;    // variable to store the servo position
int maxPos = 90;
int minPos = 10;
bool activeCall = false;

void setup() {
  myServo.attach(9);  // attaches the servo on pin 9 to the servo object
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  // check status and maybe update it.
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else if (value == startSignal) {
      activeCall = true;
    } else if (value == stopSignal) {
      activeCall == false;
    } else {
      Serial.println("Unknown message");
    }
    mySwitch.resetAvailable();
  }
  
  if (activeCall) {
    moveHandleUp();
    lightOn();
  } else {
   moveHandleDown();
   lightOff();
  }
}

void moveHandleUp() {
  myServoPos++;
    if (myServoPos > maxPos) {
      myServoPos = mxPos;
    }
    myServo.write(myservoPos);
    delay(1);
}

void moveHandleDown() {
   myServoPos--;
    if (myservoPos < minPos) {
      myServoPos = minPos;
    }
    myServo.write(myServoPos);
    delay(1);
}

void lightOn() {
  Serial.println("Start light from LED Strip");
}

void lightOff() {
  Serial.println("Reset LED strip");
}
