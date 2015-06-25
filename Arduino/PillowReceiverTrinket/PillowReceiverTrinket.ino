#include <SoftwareSerial.h>


SoftwareSerial sSerial(0,1); 

const int rxPin = 0; 
const int ledPin = 1; 
const int sigLedPin = 3; 
const int triggerPin = 2;
unsigned long lastSignal = 0;
unsigned long lastOK = 0; 

char * okString = "OK"; 
char * playString = "PLAY"; 
int okStringMatchCount=0; 
int playStringMatchCount=0; 

int signalCount = 0; 

void setup()
{
  //  Serial.begin(115200); 


  pinMode(ledPin, OUTPUT);
  // pinMode(sigLedPin, OUTPUT); 
  pinMode(triggerPin, OUTPUT); 
  digitalWrite(triggerPin, HIGH); 
  // digitalWrite(sigLedPin, LOW); 

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
    //  digitalWrite(sigLedPin, HIGH); 
    //
    //    for(int i = 0; i<2; i++) { 
    //        digitalWrite(ledPin, (i%2 == 0));
    //        delay(100); 
    //      } 




    char readChar =  sSerial.read(); 

    if(readChar == okString[okStringMatchCount]) { 

      okStringMatchCount++; 

      if(okStringMatchCount == sizeof(okString)) {
        okReceived();  
        reset(); 
      }

      lastSignal = millis(); 

    } 
    else if(readChar == playString[playStringMatchCount]) { 

      playStringMatchCount++; 

      if(playStringMatchCount == sizeof(playString)) {
        playReceived();  
        reset();
      }

      lastSignal = millis(); 

    } 
    else { 
      //reset();  

      for(int i = 0; i<2; i++) { 
        analogWrite(ledPin, (i%2)*10);
        delay(10); 
      }
    }

    //
    //    if(sSerial.read()==32){
    //
    //      signalCount ++; 
    ////      for(int i = 0; i<2; i++) { 
    ////        digitalWrite(ledPin, (i%2 == 0));
    ////        delay(50); 
    ////      }
    //
    //      if(signalCount == 10) { 
    //        digitalWrite(triggerPin, LOW); 
    //  
    //        for(int i = 0; i<6; i++) { 
    //          digitalWrite(ledPin, (i%2 == 0));
    //          delay(50); 
    //        }
    //        digitalWrite(triggerPin, HIGH); 
    //  
    //        signalCount = 0;  
    //      }
    //    //   digitalWrite(sigLedPin, LOW); 
    //    } 
    //    else signalCount = 0; 


  }

  if((unsigned long) (millis()-lastSignal > 500)) reset(); 
  //digitalWrite(ledPin, !digitalRead(0));
}


void okReceived() { 
  for(int i = 0; i<4; i++) { 
    digitalWrite(ledPin, (i%2 == 0));
    delay(50); 
  }

}

void playReceived() { 
  digitalWrite(triggerPin, LOW); 

  for(int i = 0; i<6; i++) { 
    digitalWrite(ledPin, (i%2 == 0));
    delay(50); 
  }
  digitalWrite(triggerPin, HIGH); 

}

void reset() { 

  okStringMatchCount = 0; 
  playStringMatchCount = 0; 

}





