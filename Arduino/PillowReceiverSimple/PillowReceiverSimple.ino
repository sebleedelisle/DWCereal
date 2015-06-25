#include <SoftwareSerial.h>


SoftwareSerial sSerial(0,1); 

const int rxPin = 0; 
const int ledPin = 1; 
const int triggerPin = 2;
unsigned long lastSignal = 0;

int signalCount = 0; 

void setup()
{
  //  Serial.begin(115200); 


  pinMode(ledPin, OUTPUT);
  pinMode(triggerPin, OUTPUT); 
  digitalWrite(triggerPin, HIGH); 
  
//  for(int i = 0; i<6; i++) { 
//    digitalWrite(ledPin, (i%2 == 0));
//    delay(100); 
//  } 

  sSerial.begin(1200); 
//
//  for(int i = 0; i<6; i++) { 
//    digitalWrite(ledPin, (i%2 == 0));
//    delay(100); 
//  }


  digitalWrite(ledPin, LOW);  
  //pinMode(5,OUTPUT); 
  //digitalWrite(5,HIGH);                                                             
  //Serial.begin(115200); 

}

void loop()
{

  if (sSerial.available() > 0) {
    //
    //    for(int i = 0; i<2; i++) { 
    //        digitalWrite(ledPin, (i%2 == 0));
    //        delay(100); 
    //      } 

    lastSignal = millis(); 

    if(sSerial.read()==32){

      signalCount ++; 
//      for(int i = 0; i<2; i++) { 
//        digitalWrite(ledPin, (i%2 == 0));
//        delay(50); 
//      }

      if(signalCount == 10) { 
        digitalWrite(triggerPin, LOW); 
  
        for(int i = 0; i<6; i++) { 
          digitalWrite(ledPin, (i%2 == 0));
          delay(50); 
        }
        digitalWrite(triggerPin, HIGH); 
  
        signalCount = 0;  
      }
    } 
    else signalCount = 0; 

    if((signalCount>0) && ( (unsigned long) (millis()-lastSignal > 500)) ) signalCount = 0; 
  }
}



