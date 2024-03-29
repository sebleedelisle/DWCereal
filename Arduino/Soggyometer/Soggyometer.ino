#include <Adafruit_TiCoServo.h>
#include <known_16bit_timers.h>
#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>

// adjust this to make the overall time shorter or longer (millis)
#define TOTAL_DURATION 20000

#define DURATION_CEREAL_A 50000
#define DURATION_CEREAL_B 60000
#define DURATION_CEREAL_C 70000
#define DURATION_CEREAL_D 80000
#define DURATION_CEREAL_E 5000

#define POT_PIN A5
#define BUTTON_PIN 4
#define LED_PIN 3
#define BUZZER_PIN A0
#define SERVO_PIN 9
#define SERVO_MIN 600 // 1 ms pulse
#define SERVO_MAX 2400 // 2 ms pulse
#define SNDPIN 8 // Trigger pin for FX Sound board
#define SELEC_PIN1 10 // 5-way selector switch Pin1
#define SELEC_PIN2 11 // 5-way selector switch Pin2
#define SELEC_PIN3 12 // 5-way selector switch Pin3

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

// sound trigger counter
int soundTrigger = 0; 

int rgb[] = {
  0,0,0}; 


//For the 5way switch, these variables will hold our digital read values
int pinState1 = 0;
int pinState2 = 0;
int pinState3 = 0;

//For the 5way switch, these variables will hold our digital read values, this will keep the last number sent to the terminal 
//in this way we'll keep from cramming the terminal with a continuous stream of numbers.
int lastvalue = 0;

//For the 5way switch, this variable will hold the current switch position
int switchpos = 0;


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

  //5way switch, 
  pinMode(SELEC_PIN1, INPUT);
  pinMode(SELEC_PIN2, INPUT);
  pinMode(SELEC_PIN3, INPUT);

  //First we read the pins:
  pinState1 = digitalRead(SELEC_PIN1);
  pinState2 = digitalRead(SELEC_PIN2);
  pinState3 = digitalRead(SELEC_PIN3);

  if(pinState1 == HIGH){  
    if(pinState2 == HIGH){
      switchpos = 2;
    }
    else {
      switchpos = 1;
    }
  }
  else if(pinState2 == HIGH){
    if(pinState1 == HIGH){
      switchpos = 2;
    }
    else if(pinState3 == HIGH){
      switchpos = 4;
    }
    else {
      switchpos = 3;
    }
  }       
  else if(pinState3 ==HIGH){
    if(pinState2 == HIGH){
      switchpos = 4;
    }
    else {
      switchpos = 5;
    }
  }

  //Now that we know which position our switch is in, let's print it to the terminal.
  //But first, we'll make sure the switch has moved
  Serial.print("Switch Position:");
  Serial.println(switchpos);

  if(switchpos == 1){ 
    totalTimeMils = DURATION_CEREAL_A;
  }
  if(switchpos == 2){ 
    totalTimeMils = DURATION_CEREAL_B;
  }
  if(switchpos == 3){ 
    totalTimeMils = DURATION_CEREAL_C;
  }
  if(switchpos == 4){ 
    totalTimeMils = DURATION_CEREAL_D;
  }
  if(switchpos == 5){ 
    totalTimeMils = DURATION_CEREAL_E;
  }

  lastvalue = switchpos;



}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop() {


  Serial.print("Switch Position:");
  Serial.println(switchpos);


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

    //First we read the pins:
    pinState1 = digitalRead(SELEC_PIN1);
    pinState2 = digitalRead(SELEC_PIN2);
    pinState3 = digitalRead(SELEC_PIN3);

    if(pinState1 == HIGH){  
      if(pinState2 == HIGH){
        switchpos = 2;
      }
      else {
        switchpos = 1;
      }
    }
    else if(pinState2 == HIGH){
      if(pinState1 == HIGH){
        switchpos = 2;
      }
      else if(pinState3 == HIGH){
        switchpos = 4;
      }
      else {
        switchpos = 3;
      }
    }       
    else if(pinState3 ==HIGH){
      if(pinState2 == HIGH){
        switchpos = 4;
      }
      else {
        switchpos = 5;
      }
    }

    if(switchpos == 1){ 
      totalTimeMils = DURATION_CEREAL_A;
    }
    if(switchpos == 2){ 
      totalTimeMils = DURATION_CEREAL_B;
    }
    if(switchpos == 3){ 
      totalTimeMils = DURATION_CEREAL_C;
    }
    if(switchpos == 4){ 
      totalTimeMils = DURATION_CEREAL_D;
    }
    if(switchpos == 5){ 
      totalTimeMils = DURATION_CEREAL_E;
    }

    // else if the timer is currently running
  } 
  else if(mode == MODE_RUNNING) { 




    // if we're maxxed out on time, then reset
    if (currentTime>=totalTimeMils) {
      resetFunc();  //call reset
      //currentTime = 0; 
      //startTime = millis(); 
    }

    // figure out which is the current highest pixel index based on the current time
    int currentPixel = map(currentTime, 0, totalTimeMils, 0, strip.numPixels()); 

    // if the pixel is the alarm time pixel then make the buzzer sound
    if(currentPixel == alarmTimeLed) { 
      //digitalWrite(BUZZER_PIN, (millis()%400<200) );  

      // Trigger sound
      digitalWrite(8, LOW); // bring the pin low to begin the activation

      //250ms to trigger the Sound board
      Serial.println(soundTrigger);
      if ( soundTrigger < 250 ) {
        digitalWrite(SNDPIN, LOW); // bring the pin high again to end the activation
        soundTrigger = soundTrigger++;
      }     
    } 

    else { 
      digitalWrite(BUZZER_PIN, LOW);  
      digitalWrite(SNDPIN, HIGH);
      soundTrigger = 0;
    }

    // if we're over time then flash the timer indicator pixel
    if(currentPixel >= alarmTimeLed) { 
      analogWrite(LED_PIN, sin(millis()/100.0f) * 127.0f + 127.0f );  
    } 
    else {
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
          //setPixelHSB(i, i*10 - (millis()/10), 100,3); 
          
         // CHANGE THE COLOUR OF THE PIXELS OVER THE LINE HERE : 
         
          strip.setPixelColor(i, strip.Color(0, 20, 0)); 
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

  // ranges  = hue 0-360
  // sat + brightness 0 - 100 
  while(hue<0) hue+=360; 
  while(hue>360) hue-=360; 
  H2R_HSBtoRGB( hue, saturation, brightness, &rgb[0]); 
  strip.setPixelColor(pixelnum, strip.Color(rgb[0], rgb[1], rgb[2]));

}



