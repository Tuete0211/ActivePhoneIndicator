/**
 * Receiver with indicator light and movable handle
 *
 * Purpose:
 *    Move a Servo up or down
 *    Turn on and off LED Strip
 *    Give the LED Strip a simple pattern on booting
 * How it works:
 *    By checking if a message from an 433 MHz receiver is available and right, a global variable is set.
 *    If the variable is true the servo turns and the LED Strip is activated.
 *    If the variable is false the servo is turning in the opposite direction and the LED Strip is reset.
 *    To secure the servo, maximal and minimal values have to be set
 *    To control the LED Strip the library 'FastLED' is used
 */

#include <RCSwitch.h>
#include <Servo.h>
#include <FastLED.h>

#define DEBUG 0
#define RECEIVER_PIN 0
#define SERVO_PIN 9
#define LED_PIN 6
#define NUM_LEDS 55

//variables for receiver
RCSwitch mySwitch = RCSwitch();
int startSignal = 13127467;
int stopSignal = 36653082;

//variables for servo
Servo myServo;          // create servo object to control a servo
float myServoPos = 0;   // variable to store the servo position
int upPos = 10;
int downPos = 40;

//variables for LED strip
CRGB leds[NUM_LEDS];

bool activeCall = false;
bool currentState = false;

void setup() {
  delay( 3000 ); // power-up safety delay
  if (DEBUG) {
    Serial.begin(9600);
    Serial.println("Start");
  }

  //setup receiver
  mySwitch.enableReceive(RECEIVER_PIN);  // Receiver on interrupt 0 => that is pin #2

  //setup LED strip
  LEDS.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);

  bool startUpFinished = false;
  long stopTime = millis() + 5000;
  while (!startUpFinished) {
    lightOn(true);
    startUpFinished = millis() > stopTime;
  }
  lightOff();

  // setup servo
  myServo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  myServo.write(downPos);
  delay(1000);

  if(DEBUG)Serial.println("setup finish");
}

void loop() {
  // check status and maybe update it.
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    if(DEBUG)Serial.println(value);
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else if (value == startSignal) {
      activeCall = true;
    } else if (value == stopSignal) {
      activeCall = false;
    } else {
      if(DEBUG)Serial.println("Unknown message");
    }
    mySwitch.resetAvailable();
  }

  if (activeCall != currentState) {
    if (activeCall) {
      lightOn(false);
      moveHandleUp();
      if(DEBUG)Serial.print("active Call");
    } else {
      moveHandleDown();
      lightOff();
      if(DEBUG)Serial.print("No Call");
    }
      currentState = activeCall;
  }
}

/**
 * Move phone handle up
 */
void moveHandleUp() {
  if (myServoPos != upPos) {
    myServoPos = upPos;
    myServo.write(upPos);
    delay(500);
  }
}

/**
 * Move phone handle down
 */
void moveHandleDown() {
  if (myServoPos != downPos) {
    myServoPos = downPos;
    myServo.write(downPos);
    delay(500);
  }
}

/**
 * Turn on LED Strip
 */
void lightOn(bool startup) {
  if (startup) {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors(startIndex);
  } else {
    fill_solid(leds, NUM_LEDS, CRGB(0, 155, 220));
  }
  FastLED.show();
}

/**
 * Clear whole LED Strip
 */
void lightOff() {
  FastLED.clear();
  FastLED.show();
}

/**
 * A way to make pattern move along the LEDs
 */
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( SetupBlackAndWhiteStripedPalette(), colorIndex, brightness, LINEARBLEND);
    colorIndex += 1;
  }
}

/**
 * FastLEDs way of creating color pattern
 */
CRGBPalette16 SetupBlackAndWhiteStripedPalette()
{
  CRGBPalette16 currentPalette;
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[1] = CRGB::White;
  currentPalette[2] = CRGB::White;
  currentPalette[3] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[5] = CRGB::White;
  return currentPalette;
}
