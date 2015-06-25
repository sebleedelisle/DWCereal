#define LED_PIN 13
#define TX_PIN 12
#define BUTTON_PIN 8


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TX_PIN, OUTPUT); 
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  

}

void loop() {
  
  digitalWrite(TX_PIN, !digitalRead(BUTTON_PIN)); 
  digitalWrite(LED_PIN, !digitalRead(BUTTON_PIN)); 
}
