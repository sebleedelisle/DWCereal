/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
#include <HSBColor.h>


#define MODE_SELECT 0
#define MODE_ACTIVE 1
// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 3);
//   avoid using pins with LEDs attached


// Pin definitions - Shift registers:
int enPin = 13;  // Shift registers' Output Enable pin
int latchPin = 12;  // Shift registers' rclk pin
int clkPin = 11;  // Shift registers' srclk pin
int clrPin = 10;  // shift registers' srclr pin
int datPin = 8;  // shift registers' SER pin

int redPin = 5;  // Encoder's red LED - D5 is PWM enabled
int bluePin = 6;  // Encoder's blue LED- D6 is PWM enabled
int greenPin = 9;  // Encoder's green LED - D9 is PWM enabled
int buttonPin = 7;  // Encoder's switch pin

unsigned int ledBitMask = 0;

int rgb[] = {
  0, 0, 0
};

int mode = MODE_SELECT;

int selectedOption = 0;



void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");



  // Setup shift register pins
  pinMode(enPin, OUTPUT);  // Enable, active low, this'll always be LOW
  digitalWrite(enPin, LOW);  // Turn all outputs on
  pinMode(latchPin, OUTPUT);  // this must be set before calling shiftOut16()
  digitalWrite(latchPin, LOW);  // start latch low
  pinMode(clkPin, OUTPUT);  // we'll control this in shiftOut16()
  digitalWrite(clkPin, LOW);  // start sck low
  pinMode(clrPin, OUTPUT);  // master clear, this'll always be HIGH
  digitalWrite(clrPin, HIGH);  // disable master clear
  pinMode(datPin, OUTPUT);  // we'll control this in shiftOut16()
  digitalWrite(datPin, LOW);  // start ser low

  // To begin, we'll turn all LEDs on the circular bar-graph OFF
  digitalWrite(latchPin, LOW);  // first send latch low
  shiftOut16(0x0000);
  digitalWrite(latchPin, HIGH);  // send latch high to indicate data is done sending

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT);




}

long oldPosition  = -999;

void loop() {



  if (mode == MODE_SELECT) {
    setHSBLedColor(90, 100, 100);

    long newPosition = myEnc.read();
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
      Serial.println(newPosition);
      //ledRingFollower(newPosition);

      selectedOption = newPosition / 8;

      while (selectedOption >= 5) selectedOption -= 5;
      while (selectedOption < 0) selectedOption += 5;
      clearLed();
      showLedsForOption();
      showLed();
    }
  } else {
    setHSBLedColor(60, 100, (sin(millis() * 0.02) + 1) * 50);
    clearLed();
    setLed((long)(millis() * 0.08) % 16, true);
    //setLed(15 - ((long)(millis() * 0.08) % 16), true);
    if (millis() % 200 < 180) showLedsForOption();
    showLed();
    myEnc.write(oldPosition);
  }

  if (digitalRead(buttonPin)) {

    mode = (mode + 1) % 2;
    clearLed();
    showLedsForOption();
    showLed();
    // debounce button
    while (digitalRead(buttonPin)) {
      delay(10);
    }

  }

}

void showLedsForOption() {


  setLed((selectedOption * 3), true);
  setLed((selectedOption * 3 + 1), true);
  setLed((selectedOption * 3 + 2), true);



}
void ledRingFollower(long encoderPosition)
{
  byte rotationSpeed = 1;
  // ledShift stores the bit position of the upper-most LED
  // this value should be between 0 and 15 (shifting a 16-bit vaule)
  unsigned int ledShift = 0;
  // each bit of ledOutput represents a single LED on the ring
  // this should be a value between 0 and 0xFFFF (16 bits for 16 LEDs)
  unsigned int ledOutput = 0;

  // Only do this if encoderPosition = 0, if it is 0, we don't
  // want any LEDs lit up
  if (encoderPosition != 0)
  {
    // First set ledShift equal to encoderPosition, but we need
    // to compensate for rotationSpeed.
    ledShift = encoderPosition & 0xf;// & (0xFF >> (rotationSpeed-1));
    // Now divide ledShift by 16, also compensate for rotationSpeed
    //ledShift /= 0x10>>(rotationSpeed-1);
    // Now just use ledShift to calculate ledOutput.
    // ledOutput will only have 1 bit set
    ledOutput = 1 << ledShift;
  }

  // Now we just need to write to the shift registers. We have to
  // control latch manually, but shiftOut16 will take care of
  // everything else.
  digitalWrite(latchPin, LOW);  // first send latch low
  shiftOut16(ledOutput);  // send the ledOutput value to shiftOut16
  digitalWrite(latchPin, HIGH);  // send latch high to indicate data is done sending
}


void clearLed() {
  ledBitMask = 0;

}

void setLed(int lednum, bool on) {
  lednum = lednum % 16;
  while (lednum < 0) lednum += 16;

  ledBitMask |= 1 << lednum;



}

void showLed() {

  digitalWrite(latchPin, LOW);  // first send latch low
  shiftOut16(ledBitMask);  // send the ledOutput value to shiftOut16
  digitalWrite(latchPin, HIGH);  // send latch high to indicate data is done sending


}





void shiftOut16(uint16_t data)
{
  byte datamsb;
  byte datalsb;

  // Isolate the MSB and LSB
  datamsb = (data & 0xFF00) >> 8; // mask out the MSB and shift it right 8 bits
  datalsb = data & 0xFF;  // Mask out the LSB

  // First shift out the MSB, MSB first.
  shiftOut(datPin, clkPin, MSBFIRST, datamsb);
  // Then shift out the LSB
  shiftOut(datPin, clkPin, MSBFIRST, datalsb);
}



void setHSBLedColor(int hue, int saturation, int brightness) {

  // ranges  = hue 0-360
  // sat + brightness 0 - 100
  while (hue < 0) hue += 360;
  while (hue > 360) hue -= 360;
  H2R_HSBtoRGB( hue, saturation, brightness, &rgb[0]);
  //strip.setPixelColor(pixelnum, strip.Color(rgb[0], rgb[1], rgb[2]));
  analogWrite(redPin, 255 - rgb[0]);
  analogWrite(greenPin, 255 - rgb[1]);
  analogWrite(bluePin, 255 - rgb[2]);

}



