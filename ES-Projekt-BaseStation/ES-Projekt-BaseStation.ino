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

const short minDataPackage = 100; // above is a Spike or a Data Package
const short minSpike = 160; // above is a Spike
const short inputPinNumber = 0;
const short ledPin = 6;

short framesWithDataPackage = 0; // Zähler für Anzahl von Durchläufen, in welchen (vermutlich) Daten übertragen werden
short dataPackageCounter = 0; // Zähler für Anzahl von Durchläufen mit Datenübertragung zwischen Spikes (01 & 10 wird in dieser Zeit übertragen).
short periodWithDataPackage = 0; // Zähler für die Anzahl von Durchläufe in einer Periode in welchen Daten übertragen wurden (mind. 3 damit phoneActive = true)
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
  Serial.println(analogInput);

  /* Testen ob mind. zwei mal Daten zwischen Spikes gesendet wurden*/
  if (analogInput > minSpike) { // Spike entdeckt
    if (dataPackageCounter > 2) { // mind. 3 x Daten gemessen innerhalb eines Frames (zw. zwei Spikes)
      framesWithDataPackage++;
    } else {
      framesWithDataPackage = 0; // wenn keine Daten zwischen den Spikes gesendet wurden, Zähler der Frames zurücksetzten
    }
    dataPackageCounter = 0;
  } else if (analogInput > minDataPackage) { // kein Spike, aber Daten
    dataPackageCounter++;
  }

  if (framesWithDataPackage > 2) { // Prüfen ob mindestens 3 Frames Daten gesendet haben (mitteln)
    periodWithDataPackage++;
  }

  if (oldMillis + 1000 < millis()) { // every 1000ms the LED can be changed
    phoneLifted = periodWithDataPackage > 3; // mind. 4 x 4 Frames, than phone is active
    if (phoneLifted != ledActive) {
      digitalWrite(ledPin, phoneLifted ? HIGH : LOW);
      if (phoneLifted) {
        //mySwitch.send(startSignal);
        delay(1);
      } else {
        //mySwitch.send(stopSiganl);
        delay(1);
      }
      ledActive = phoneLifted;
    }
    periodWithDataPackage = 0;
    oldMillis = millis();
  }
}
