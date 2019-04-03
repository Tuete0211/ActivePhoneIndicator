/**
 * Transmitter with a reed-switch as a trigger
 *
 * Purpose:
 *    Detecting if Phone is activ (Reed-switch open)
 *    Send four times a signal on reed-switch change
 *  How it works:
 *    With the handset in its place on the phone the reed-switch is closed due to a magnet on the headset.
 *    With the handset lifted the switch is open.
 */

#include <RCSwitch.h>

#define DEBUG 0
#define TRANSMITTER_PIN 10
#define INPUT_PIN 2
#define LED_PIN 6

//variables for transmitter
RCSwitch mySwitch = RCSwitch();
int startSignal = 13127467;
int stopSignal = 36653082;

long oldMillis = 0;
bool phoneLifted = false;
bool ledActive = false;

void setup() {
  delay( 3000 ); // power-up safety delay
  if (DEBUG) {
    Serial.begin(9600);
    Serial.println("Start...");
  }

  //setup indicator LED
  pinMode(LED_PIN, OUTPUT);

  //setup transmitter
  mySwitch.enableTransmit(TRANSMITTER_PIN);
}

void loop() {
  // check INPUT_PIN for current value
  short digitalInput = digitalRead(INPUT_PIN);
  if(DEBUG)Serial.println(digitalInput);

  if(digitalInput == 0) {
    phoneLifted = true;
  } else {
    phoneLifted = false;
  }

  if (oldMillis + 1000 < millis()) { // every 1000ms the LED can be changed
    if (phoneLifted != ledActive) {
      digitalWrite(LED_PIN, phoneLifted ? HIGH : LOW);
      for (int i = 0; i<5; i++) {
        if (phoneLifted) {
          mySwitch.send(startSignal, 24);
          delay(1);
          if(DEBUG)Serial.println("startSignal");
        } else {
          mySwitch.send(stopSignal, 24);
          delay(1);
          if(DEBUG)Serial.println("stopSognal");
        }
      }
      ledActive = phoneLifted;
    }
    oldMillis = millis();
  }
}
