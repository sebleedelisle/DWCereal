#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x8matrix matrixLeft, matrixRight; 


unsigned long lastFrame = 0; 
unsigned long frameCount = 0; 
int energy = 14; 

int nextBlinkCountdown = 10; 
int eyeMoveCountdown = 10; 
float eyeX = 3, eyeY=3, targetEyeX = 2, targetEyeY=2; 


void setup() { 
  Serial.begin(115200);
  matrixLeft.begin(0x70);
  matrixRight.begin(0x71);
  matrixLeft.setBrightness(1); 
}


void loop() { 
  if((unsigned long) (millis() - lastFrame) > 10) { // 10 0 fps
    updateAnimation();  
  }


}

void updateAnimation() { 

  lastFrame = millis();

  // things that are affected by energy : 
  // how open the eyes are 0-
  // how fast the pupil moves
  // how fast the eyes blink
  // how frequently the eyes blink
  
  int eyeMoveProgress = -1; 
  boolean eyeMoving = false; 
   if(eyeMoveCountdown<5) { 
    eyeMoveProgress = 5-eyeMoveCountdown; 
    eyeMoving = true; 
  }
  
  
  int blinkProgress = -1; 
  int blinkFrames = 14; 
  if(nextBlinkCountdown<blinkFrames) { 
    blinkProgress = blinkFrames-nextBlinkCountdown; 
  }

  boolean blinking = false; 
  if(blinkProgress>=0) blinking = true; 

  int top = (blinkProgress<blinkFrames/2) ? blinkProgress : blinkFrames-blinkProgress; 

  energy = map(analogRead(A8),50,900,14,0); 
  int blinkspeed = map(energy, 0, 14, 6,1); 

  // sleepy determines how sleepy we are 0 to 7 (7 is asleep); 
  int sleepy = map(energy,1,14,6,0);
  if(energy == 0) sleepy = 7; 
 
  if(top<sleepy) top = sleepy; 
  //Serial.println(sleepy); 

  // so it doesn't blink if the eyes are asleep
  if(sleepy>=7) top = 7; 

  matrixLeft.clear();

  if((top<=8)&&(sleepy<6)) {

    //full screen eyeball
    matrixLeft.fillRect(0,top,8,8-top, LED_ON);

    // pupil
    int eyey = eyeY; 

    if(sleepy+1>eyey) eyey = constrain(sleepy+1,0,4); 

    matrixLeft.fillRect(eyeX,eyey,3,3,LED_OFF); 

    if(top>=1) { 
      matrixLeft.fillRect(0,top, 8,1,LED_ON); 
    }
    // for 
//    if((top>3) && (top == sleepy)) { 
//      matrixLeft.drawPixel(0,top-1,LED_ON);  
//      matrixLeft.drawPixel(7,top-1,LED_ON);  
// 
//    }

    matrixLeft.drawPixel(0,0,LED_OFF);  
    matrixLeft.drawPixel(7,0,LED_OFF);  
    matrixLeft.drawPixel(7,7,LED_OFF);  
    matrixLeft.drawPixel(0,7,LED_OFF);  

  } else { 
     //asleep! 
     matrixLeft.drawLine(0,4,0,6, LED_ON); 
     matrixLeft.drawLine(1,7,6,7, LED_ON); 
     matrixLeft.drawLine(7,4,7,6, LED_ON);
     matrixLeft.drawLine(1,5,1,6, LED_ON); 
     matrixLeft.drawLine(2,6,5,6, LED_ON); 
     matrixLeft.drawLine(6,5,6,6, LED_ON);
     
     // MORE ANGULAR SHUT EYES
//          matrixLeft.drawLine(0,5,2,7, LED_ON); 
//     matrixLeft.drawLine(2,7,5,7, LED_ON); 
//     matrixLeft.drawLine(5,7,7,5, LED_ON);
     
   
  }
  matrixLeft.writeDisplay();
  // Serial.println(top); 


  if(!blinking) { 
    nextBlinkCountdown--; 
  } 
  else { 
    if(frameCount%blinkspeed==0) nextBlinkCountdown--;  
  }
  if(nextBlinkCountdown <0) {   
    nextBlinkCountdown = random(map(energy, 0, 14, 200,30), map(energy, 0, 14, 800,200)); 
  }
  
  eyeMoveCountdown--; 
  if(eyeMoveCountdown <0) { 
     eyeMoveCountdown = random(map(energy, 0, 14, 200,30), map(energy, 0, 14, 800,200)); 
     targetEyeX = random(1,5); 
     targetEyeY = random(1,5);
//    Serial.print(eyeX);  
//     Serial.print(" " ); 
//     Serial.println(eyeY); 
  }
  
  frameCount++; 
  
}




