#include <SoftwareSerial.h>

/**
 * 434MHz Transmitter Temperature Sensor on the Trinket using Software Serial 
 */
SoftwareSerial sSerial(0,1); 

const int ledPin = 4; 
const int buttonPin = 2; 

//UsiSerial sSerial(0,1); // RX, TX Pins
void setup()
{
  pinMode(ledPin, OUTPUT); 

  for(int i = 0; i<6; i++) { 
    digitalWrite(ledPin, (i%2 == 0));
    delay(100); 
  }  

  sSerial.begin(1200); 

  for(int i = 0; i<6; i++) { 
    digitalWrite(ledPin, (i%2 == 0));
    delay(100); 
  }

  pinMode(buttonPin, INPUT_PULLUP); 

  //
}

void loop()
{

  if(sSerial.available() > 0) sSerial.read();

  if(!digitalRead(buttonPin)) { 

    //    sendNulls(); 
    //    for(int i = 0; i<15; i++) { 
    //      sSerial.write(32); 
    //    }
    digitalWrite(ledPin, HIGH); 
    sendNulls();
    sSerial.print("PPPPPPPPPP"); 


    digitalWrite(ledPin, LOW); 
    while(!digitalRead(buttonPin)) { 
      delay(10);  

    }

  } 
  else {                                                                                                                                                                                                 
    //sSerial.print("\0\0\0\0\0\0\0\0\0OK"); 


  }
  //digitalWrite(ledPin, digitalRead(buttonPin)); 

}

void sendNulls()
{
  for(int i=0; i<50; i++)
  {
    sSerial.write((uint8_t)0);
  }
}


