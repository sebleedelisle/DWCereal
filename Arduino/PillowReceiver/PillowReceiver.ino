
const int rxPin = A0; 
const int ledPin = 13; 


void setup()
{
  Serial.begin(115200); 
  pinMode(ledPin, OUTPUT);
  //pinMode(5,OUTPUT); 
  //digitalWrite(5,HIGH); 
  //Serial.begin(115200); 

}

void loop()
{
  

  analogWrite(ledPin, analogRead(rxPin)/4);
Serial.println(  analogRead(rxPin)); 

}
