/**
   Simple Receiver
   Purpose:
      Move a Servo up or down
      Turn on an off LED Strip
      Give the LED Strip a simple color pattern
   How it works:
      By checking if a message from an 433 MHz receiver is available a global variable is set.
      It the variable is true the servo turns and the LED Strip is activated.
      If the variable is false the servo is turning in th eopposite direction and the LED Strip is reset.
      To secure the servo maximal and minimal values have to be set
      To control the LED Strip the library FastLED is used
   Todo:
      FastLED library
      connecting an LED Strip
*/

#include <RCSwitch.h>
#include <Servo.h>
#include <FastLED.h>

#define RECEIVER_PIN 0
#define SERVO_PIN 9
#define LED_PIN 3
#define NUM_LEDS 58
#define UPDATES_PER_SECOND 100

//variables for receiver
RCSwitch mySwitch = RCSwitch();
int startSignal = 1234;
int stopSignal = 5678;

//variables for servo
Servo myServo;  // create servo object to control a servo
float myServoPos = 0;    // variable to store the servo position
int maxPos = 90;
int minPos = 10;

//variables for LED strip
CRGB leds[NUM_LEDS];

bool activeCall = false;

void setup() {
  delay( 3000 ); // power-up safety delay

  Serial.begin(9600);
  Serial.println("Start");
  //setup receiver
  mySwitch.enableReceive(RECEIVER_PIN);  // Receiver on interrupt 0 => that is pin #2

  // setup servo
  myServo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  myServo.write(0);
  delay(2000);
  myServo.write(50);
  delay(2000);
  myServo.write(10);
  delay(2000);

  //setup LED strip
  LEDS.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);

  Serial.println("setup finish");
}

void loop() {
  // check status and maybe update it.
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    Serial.println(value);
    if (value == 0) {
      //Serial.print("Unknown encoding");
    } else if (value == startSignal) {
      activeCall = true;
    } else if (value == stopSignal) {
      activeCall = false;
    } else {
      //Serial.println("Unknown message");
    }
    mySwitch.resetAvailable();
  }

  if (activeCall) {
    if (millis() % 100 == 0) {
      moveHandleUp();
      lightOn();
      Serial.print("active Call");
    }
  }
  else {
    moveHandleDown();
    lightOff();
  }
}

void moveHandleUp() {
 
  if (myServoPos != maxPos) { 
    myServoPos=maxPos;
    myServo.write(maxPos);
    delay(1000);
  }
  
  
}

void moveHandleDown() {
  
  if (myServoPos != minPos) {
    myServoPos=minPos;
    myServo.write(minPos);
    delay(1000);
  }
  
  
}

void lightOn() {
  static uint8_t startIndex = 0;
  startIndex = startIndex + 4; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  //FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void lightOff() {
  if (leds[0].r != 0) {
    //Serial.println("Reset LED strip");
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
      //leds[i].nscale8(200);
    }
    FastLED.show();
  }else if (leds[0].g != 0) {
    //Serial.println("Reset LED strip");
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
      //leds[i].nscale8(200);
    }
    FastLED.show();
  } else if (leds[0].b != 0) {
    //Serial.println("Reset LED strip");
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
      //leds[i].nscale8(200);
    }
    FastLED.show();
  } else {
    //Serial.println("Is Black");
    Serial.println(leds[23].r);
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
    colorIndex += 3;
  }
}
