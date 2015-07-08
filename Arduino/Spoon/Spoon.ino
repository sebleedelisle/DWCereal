
#include <known_16bit_timers.h>
#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>


#define XAXIS_INPUT A1
#define YAXIS_INPUT A2
#define ZAXIS_INPUT A3

#define MODE_EAT 0
#define MODE_DISSIPATE 1

#define LED_STRIP_PIN 7
#define NUM_LEDS 8
#define MAX_ENERGY 14
#define DEMO_SWITCH_PIN 13
#define LED_STRIP_BRIGHTNESS 20 // from 0 to 255
#define DEMO_DISSIPATE_TIME 1000 // 3 seconds to dissipate each energy bar in demo mode
#define DISSIPATE_TIME 60000 * 30 // 30 minutes to dissipate in normal mode
#define TIME_BETWEEN_SPOONFULS 100 // in mils

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

int timer = 0; // timer goes from 0 to 1000 (arbitrary)
unsigned long startTime = 0;
unsigned long lastSpoonMoveTime =  0;
long dissipateTimeMils = DISSIPATE_TIME; // 30 seconds per energy drop
bool spoonDown = false;

// energy goes from 0 to 14 - starting at 2 ensures first light is always on
int energy = 0;
int mode = MODE_EAT;

int rgb[] = {
  0, 0, 0
};

void setup() {

  pinMode(XAXIS_INPUT, INPUT);
  pinMode(YAXIS_INPUT, INPUT);
  pinMode(ZAXIS_INPUT, INPUT);
  pinMode(DEMO_SWITCH_PIN, INPUT_PULLUP);

  Serial.begin(115200);

  initEyes(); 

  strip.begin();
  updateLedStrip();

}




void loop() {

  updateEyes();

  if (mode == MODE_DISSIPATE) {
    dissipateTimeMils = digitalRead(DEMO_SWITCH_PIN) ? DEMO_DISSIPATE_TIME : DISSIPATE_TIME;
    if ((unsigned long)(millis() - startTime) > dissipateTimeMils) {
      startTime = millis();
      energy--;
      if (energy < 0) energy = 0;

    }
    updateLedStrip();
  }

  if ((mode == MODE_EAT) && checkAccelerometer()) updateLedStrip();

}


void startDissipating() {

  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
      strip.show();
      delay(10);
       updateEyesAnimation(true);
      delay(10);
    
    }
    for (int i = 0; i < NUM_LEDS; i++ ) {
      strip.setPixelColor(i, strip.Color(0, LED_STRIP_BRIGHTNESS, 0));
      strip.show();
      delay(10);
      updateEyesAnimation(true);
  delay(10);
    
    }
  }
  startTime = millis();
  mode = MODE_DISSIPATE;

}
// checks the accelerometer and returns true if it's changed.

boolean checkAccelerometer() {
  int yAxis = analogRead(YAXIS_INPUT);
  
  Serial.println(yAxis); 
  
  // debounce
  if ((unsigned long)(millis() - lastSpoonMoveTime) > TIME_BETWEEN_SPOONFULS) {

    if (spoonDown && (yAxis < 320)) {
      spoonDown = false;
      energy++;
      if (energy >= MAX_ENERGY) {
        startDissipating();
      }
      lastSpoonMoveTime = millis();
      Serial.println("ENERGY ++ ");
      return true;
    } else if ((!spoonDown) && (yAxis > 400)) {
      spoonDown = true;
      lastSpoonMoveTime = millis();
    }
  }
  return false;

}


void updateLedStrip() {

  Serial.println("updateLedStrip");

  int currentLevelPixel = constrain(energy + 1, 0, MAX_ENERGY + 2);

  Serial.println(currentLevelPixel);
  for (uint16_t i = 0; i < NUM_LEDS * 2; i++) {
    if (i <= currentLevelPixel) strip.setPixelColor(i / 2, strip.Color((i % 2 == 0) ? LED_STRIP_BRIGHTNESS : 0, LED_STRIP_BRIGHTNESS, 0));
    else if (i % 2 == 0)strip.setPixelColor(i / 2, strip.Color(0, 0, 0));
  }

  strip.show();


}


void setPixelHSB(int pixelnum, int hue, int saturation, int brightness) {

  while (hue < 0) hue += 360;
  while (hue > 360) hue -= 360;
  // H2R_HSBtoRGB( hue, saturation, brightness, &rgb[0]);
  // strip.setPixelColor(pixelnum, strip.Color(rgb[0], rgb[1], rgb[2]));

}

