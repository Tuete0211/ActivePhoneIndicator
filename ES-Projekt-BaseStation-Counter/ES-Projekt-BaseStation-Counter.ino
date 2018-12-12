/**
   ISDN Phone Sniffer

   Purpose:
      Detecting if Phone is activ (dial-tone is sounding)
      If phone is on ISDN-Bus all Phones will have effect on the sign

    How it works:
      By measuring the voltage of the Datalines (3 & 6) a specific pattern is measurable.
      The signal is devided in Spikes (over 160), Data-Packages (btween 100 and 180) and noise.
      With the handset in its place on the phone no Data-Packages are recordable.
      With the handset lifted and the dial-tone sounding Data-Packages are measurable between Spikes.
      The Spikes are measurable always.

*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
const short transmitterPin = 10;
int startSignal = 1234;
int stopSignal = 5678;

const short minDataPackage = 100; // above is a Spike or a Data Package
const short minCount = 80;
const short inputPinNumber = 0;
const short ledPin = 6;

short framesWithDataPackage = 0; // Zähler für Anzahl von Durchläufen, in welchen (vermutlich) Daten übertragen werden
short dataPackageCounter = 0; // Zähler für Anzahl von Durchläufen mit Datenübertragung zwischen Spikes (01 & 10 wird in dieser Zeit übertragen).
short sendCounter = 0;
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
  short analogInput = analogRead(inputPinNumber);
  //Serial.println(analogInput);

  /* Testen ob mind. zwei mal Daten zwischen Spikes gesendet wurden*/
  if (analogInput > minDataPackage) { // Spike entdeckt
    dataPackageCounter++;
  }

  if (oldMillis + 100 < millis()) { // every 100ms the LED can be changed
    Serial.println(dataPackageCounter);
    phoneLifted = dataPackageCounter > minCount;
    if (phoneLifted != ledActive) {
      digitalWrite(ledPin, phoneLifted ? HIGH : LOW);
      if (phoneLifted) {
        mySwitch.send(startSignal, 24);
        delay(1);
      } else {
        mySwitch.send(stopSignal, 24);
        delay(1);
      }
      ledActive = phoneLifted;
    }
    if (sendCounter > 10) {
      if (phoneLifted) {
        mySwitch.send(startSignal, 24);
        delay(1);
      } else {
        mySwitch.send(stopSignal, 24);
        delay(1);
      }
      sendCounter = 0;
    } else {
      sendCounter++;
    }
    dataPackageCounter = 0;
    oldMillis = millis();
  }
}
