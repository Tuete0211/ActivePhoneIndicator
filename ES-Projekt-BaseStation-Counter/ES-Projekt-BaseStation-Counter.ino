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

#define DEBUG 0
#define TRANSMITTER_PIN 10
#define INPUT_PIN 0
#define LED_PIN 6

RCSwitch mySwitch = RCSwitch();
int startSignal = 1234;
int stopSignal = 5678;

const short spikeThreshold = 100; // above is a Spike or a Data Package
const short spikeCounterThreshold = 80;

short spikeCounter = 0; // Zähler für Anzahl von Durchläufen mit Datenübertragung zwischen Spikes (01 & 10 wird in dieser Zeit übertragen).
short sendCounter = 0;
unsigned long oldMillis = 0;

bool phoneLifted = false;
bool ledActive = false;

void setup() {
  // put your setup code here, to run once:
  if (DEBUG) {
    Serial.begin(9600);
    Serial.println("Start...");
  }
  pinMode(LED_PIN, OUTPUT);
  mySwitch.enableTransmit(TRANSMITTER_PIN);
}

void loop() {

  // put your main code here, to run repeatedly:
  short analogInput = analogRead(INPUT_PIN);
  if (DEBUG)Serial.println(analogInput);

  if (analogInput > spikeThreshold) { // Spike entdeckt
    spikeCounter++;
  }

  if (oldMillis + 100 < millis()) { // every 100ms the LED can be changed
    //if (DEBUG)Serial.println(spikeCounter);
    phoneLifted = spikeCounter > spikeCounterThreshold;
    if (phoneLifted != ledActive) {
      digitalWrite(LED_PIN, phoneLifted ? HIGH : LOW);
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
    spikeCounter = 0;
    oldMillis = millis();
  }
}
