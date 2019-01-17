/**
 * ISDN Phone Sniffer
 * 
 * Purpose:
 *    Detecting if Phone is activ (dial-tone is sounding)
 *    If phone is on ISDN-Bus all Phones will have effect on the sign
 *  
 *  How it works:
 *    By measuring the voltage of the Datalines (3 & 6) a specific pattern is measurable.
 *    The signal is devided in Spikes (over 160), Data-Packages (btween 100 and 180) and noise.
 *    With the handset in its place on the phone no Data-Packages are recordable. 
 *    With the handset lifted and the dial-tone sounding Data-Packages are measurable between Spikes.
 *    The Spikes are measurable always.
 * 
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
const short transmitterPin = 10;
int startSignal = 1234;
int stopSignal = 5678;

const short inputPinNumber = 2;
const short ledPin = 6;

unsigned long oldMillis = 0;
bool phoneLifted = false;
bool ledActive = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start...");
  pinMode(ledPin, OUTPUT);
  mySwitch.enableTransmit(transmitterPin);
}

void loop() {

  // put your main code here, to run repeatedly:
  short digitalInput = digitalRead(inputPinNumber);
  Serial.println(digitalInput);

  if(digitalInput == 0) {
    phoneLifted = true;
  } else {
    phoneLifted = false;
  }

  if (oldMillis + 1000 < millis()) { // every 1000ms the LED can be changed
    if (phoneLifted != ledActive) {
      digitalWrite(ledPin, phoneLifted ? HIGH : LOW);
      for (int i = 0; i<5; i++) {
        if (phoneLifted) {
          mySwitch.send(startSignal, 24);
          delay(1);
          Serial.println("startSignal");
        } else {
          mySwitch.send(stopSignal, 24);
          delay(1);
          Serial.println("stopSognal");
        }
      }
      ledActive = phoneLifted;
    }
    oldMillis = millis();
  }
}
