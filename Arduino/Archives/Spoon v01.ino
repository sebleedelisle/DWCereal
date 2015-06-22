
#include <known_16bit_timers.h>
#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>

#define XAXIS_INPUT A1
#define YAXIS_INPUT A2
#define ZAXIS_INPUT A3

#define MODE_WAIT 0
#define MODE_RUNNING 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, 6, NEO_GRB + NEO_KHZ800);

int timer = 0; // timer goes from 0 to 1000 (arbitrary)
unsigned long startTime = 0; 
unsigned long lastChange =  0; 
long totalDissipateTimeMils = 30000; 
bool spoonDown = false; 
int energy = 0; 

int mode = MODE_WAIT;




int rgb[] = {
  0,0,0}; 

void setup() {
  
  pinMode(XAXIS_INPUT, INPUT); 
  pinMode(YAXIS_INPUT, INPUT); 
  pinMode(ZAXIS_INPUT, INPUT); 
  
  Serial.begin(115200); 
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //strip.clear();
}

void loop() {

  unsigned long currentTime = millis() - startTime; 

  if (currentTime>=totalDissipateTimeMils) {
    currentTime = 0; 
    startTime = millis(); 
  }
  int currentPixel = map(currentTime, 0, totalDissipateTimeMils, strip.numPixels(),0); 

  int currentLevelPixel = energy; // map(analogRead(A0), MIN_WATER,MAX_WATER,strip.numPixels(),0); 
  currentLevelPixel = constrain(currentLevelPixel, 0, strip.numPixels()-1); 
  for(uint16_t i=0; i<strip.numPixels(); i++) {


    if(i<=currentLevelPixel) strip.setPixelColor(i, strip.Color(0, 20, 0));
    else strip.setPixelColor(i, strip.Color(0, 0, 0));

  } 

  int yAxis = analogRead(YAXIS_INPUT); 
  // debounce
  if((unsigned long)(millis() - lastChange) > 500) { 
    
    if(spoonDown && (yAxis<320)) { 
       spoonDown = false; 
      energy++;  
      if(energy>16) energy = 0; 
      lastChange = millis(); 
    } else if((!spoonDown) && (yAxis>400)) { 
      spoonDown = true; 
      lastChange = millis(); 
    } 
  }
  strip.show();
  

  Serial.println(analogRead(YAXIS_INPUT)); 
  
}

void setPixelHSB(int pixelnum, int hue, int saturation, int brightness) { 

  while(hue<0) hue+=360; 
  while(hue>360) hue-=360; 
  // H2R_HSBtoRGB( hue, saturation, brightness, &rgb[0]); 
  // strip.setPixelColor(pixelnum, strip.Color(rgb[0], rgb[1], rgb[2]));

}


