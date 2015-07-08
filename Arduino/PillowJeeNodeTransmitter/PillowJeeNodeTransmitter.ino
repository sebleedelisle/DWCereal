/// @dir test1
/// Simple node-alive check, sends out one test packet per second.
// 2013-02-02 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <JeeLib.h>

const byte LED = 7;
const int BUTTON_PIN = 3;

unsigned long lastSend = 0;

byte counter;

void setup () {

  Serial.begin(57600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // this is node 1 in net group 100 on the 868 MHz band
  rf12_initialize(1, RF12_868MHZ, 100);
}

void loop () {

  if ((unsigned long)(millis() - lastSend) > 1000) {
    led(true);

    // actual packet send: broadcast to all, current counter, 1 byte long
    rf12_sendNow(0, &counter, 1);;
    rf12_sendWait(1);

    led(false);
    lastSend = millis();
      ++counter;
  }
  if (!digitalRead(BUTTON_PIN)) {
    led(true);

    Serial.println("BUTTON PRESSED");
    rf12_sendNow(0, "ON", 2);;
    rf12_sendWait(1);
    // DEBOUNCE
    while (!digitalRead(BUTTON_PIN)) {
      delay(10);
    }
    led(false);
  }

  // increment the counter (it'll wrap from 255 to 0)

  // let one second pass before sending out another packet
  //delay(1000);
}


// turn the on-board LED on or off
static void led (bool on) {
  digitalWrite(LED, on ? 1 : 0); // inverted logic
}

