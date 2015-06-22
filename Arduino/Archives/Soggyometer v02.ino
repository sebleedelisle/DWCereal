#include <Adafruit_TiCoServo.h>
#include <known_16bit_timers.h>
#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>

// adjust this to make the overall time shorter or longer (millis)
#define TOTAL_DURATION 20000

#define POT_PIN A5
#define BUTTON_PIN 4
#define LED_PIN 3
#define BUZZER_PIN A0
#define SERVO_PIN 9
#define SERVO_MIN 600 // 1 ms pulse
#define SERVO_MAX 2400 // 2 ms pulse
#define SNDPIN 8 // Trigger pin for FX Sound board

#define MODE_WAIT 0
#define MODE_RUNNING 1

//initialise LED strip and servo. 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, 6, NEO_GRB + NEO_KHZ800);
// We can't use the standard servo library due to conflicts with the neopixel strip
Adafruit_TiCoServo servo;

// timer variable is used to record the progress of our timer. 0 to 1000
int timer = 0; // timer goes from 0 to 1000 (arbitrary)

// mode dictates whether we are actively running the timer (MODE_RUNNING)
// or in timer setting mode (MODE_WAIT)
int mode = MODE_WAIT; 

// this is used to store the alarm time. It relates to a particular led index.
int alarmTimeLed = 0; 

// startTime stores the number of elapsed ms when the start button is pressed
unsigned long startTime = 0; 

// the total length of the timer in mils. 
long totalTimeMils = TOTAL_DURATION; 

int rgb[] = {
  0,0,0}; 

void setup() {
  Serial.begin(115200); 
  servo.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // set the pin modes for anything that isn't a standard input
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(BUZZER_PIN, OUTPUT); 
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(BUZZER_PIN, LOW);  
  
  pinMode(SNDPIN, OUTPUT);
  digitalWrite(SNDPIN, HIGH); // Set the pin high as the default state

}

void loop() {

  unsigned long currentTime = millis() - startTime; 

  // if we're in wait mode then we read the potentiometer to set the alarm time
  if(mode == MODE_WAIT) { 
    
    alarmTimeLed = map(analogRead(POT_PIN), 0, 1024, 0, strip.numPixels()); 
    
    // iterate through all the neopixels
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      // if the pixel is less than the alarmTimeLed pixel then colour this as the rainbow
      // if it is the alarmTimeLed pixel, colour it white
      // otherwise set it to black
      if(i<alarmTimeLed) setPixelHSB(i, 180+((i-alarmTimeLed)*15) , 100,10);  //strip.setPixelColor(i, strip.Color(0, 0, 50));
      else if(i==alarmTimeLed) setPixelHSB(i, 180+(i*10) , 0,10);
      else strip.setPixelColor(i, strip.Color(0, 0, 0));
    } 

    strip.show(); 

   
  // else if the timer is currently running
  } else if(mode == MODE_RUNNING) { 
  
    // if we're maxxed out on time, then reset to zero. This is just for test purposes
    if (currentTime>=totalTimeMils) {
      currentTime = 0; 
      startTime = millis(); 
    }
    
    // figure out which is the current highest pixel index based on the current time
    int currentPixel = map(currentTime, 0, totalTimeMils, 0, strip.numPixels()); 

    // if the pixel is the alarm time pixel then make the buzzer sound
    if(currentPixel == alarmTimeLed) { 
      digitalWrite(BUZZER_PIN, (millis()%400<200) );  
      // Trigger sound
  digitalWrite(8, LOW); // bring the pin low to begin the activation
  /*
  According to the documentation, the Audio FX board needs 50ms to trigger. However,
  I've found that coming from my 3.3v Arduino Pro, it needs 100ms to get the trigger
  the going
  */
  delay(250); // hold the pin low long enough to trigger the board; may need to be longer for consistent triggering
  digitalWrite(SNDPIN, HIGH); // bring the pin high again to end the activation
  delay(500);
      
    } 
    else { 
      digitalWrite(BUZZER_PIN, LOW);  
    }

    // if we're over time then flash the timer indicator pixel
    if(currentPixel >= alarmTimeLed) { 
      analogWrite(LED_PIN, sin(millis()/100.0f) * 127.0f + 127.0f );  
    } else {
      digitalWrite(LED_PIN, LOW); 
    }


    // go through all the pixels
    for(uint16_t i=0; i<strip.numPixels(); i++) {

      // make the pixels beneath the timer indicator pixel rainbow coloured
      if(i<=currentPixel)  {
        if(i<alarmTimeLed) {
          setPixelHSB(i, i*10 - (millis()/10), 100,10); 
          // strip.setPixelColor(i, strip.Color(50, 0, 0));

        } 
        // make the actual timer pixel pulse white
        else if(i==alarmTimeLed) {
          setPixelHSB(i, 0, 0,sin(millis()/50.0f) * 5.0f + 5.0f); 
          //strip.setPixelColor(i, strip.Color(50, 50, 50));
        }
        // make pixels above the timer pixel dimmer rainbows
        else if(i>alarmTimeLed) {
          setPixelHSB(i, i*10 - (millis()/10), 100,3); 
          //dark red : 
          //setPixelHSB(i, 0, 100, 3); 
          //strip.setPixelColor(i, strip.Color(50, 50, 50));
        }

      // else if we are above the current pixel
      }
      else {
        // then light the timer indicator 
        if(i==alarmTimeLed) {
          setPixelHSB(i, 0, 0, 10.0f); 
        // and turn all the other LEDs off
        } 
        else { 
          strip.setPixelColor(i, strip.Color(0, 0, 0));
        }
      }

    } 

    // move the motor to the current position
    servo.write(map(currentTime, 0, totalTimeMils, SERVO_MIN, SERVO_MAX));                               // Move servo
    // update the strip
    strip.show();

  }

  // check the button
  if(digitalRead(BUTTON_PIN) == LOW ) { 
    // the button is pushed so change mode!
    if (mode == MODE_WAIT) { 
      mode = MODE_RUNNING; 
      startTime = millis(); 
    } 
    else if(mode == MODE_RUNNING) { 
      mode = MODE_WAIT; 

    } 
    // wait a bit as long as the button is pressed (debounce technique) 
    do {
      delay(100); 
    } 
    while(digitalRead(BUTTON_PIN) == LOW); 

  }  

}


void setPixelHSB(int pixelnum, int hue, int saturation, int brightness) { 

  while(hue<0) hue+=360; 
  while(hue>360) hue-=360; 
  H2R_HSBtoRGB( hue, saturation, brightness, &rgb[0]); 
  strip.setPixelColor(pixelnum, strip.Color(rgb[0], rgb[1], rgb[2]));

}




