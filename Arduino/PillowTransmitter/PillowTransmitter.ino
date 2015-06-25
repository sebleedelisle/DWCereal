#define LED_PIN 13
#define TX_PIN 12
#define BUTTON_PIN 8

#include <VirtualWire.h>
char *controller;
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  
  vw_set_ptt_inverted(true); //
  vw_set_tx_pin(TX_PIN);
  vw_setup(4000);// speed of data transfer Kbps
}

void loop() {
  
  
  
    
  if(!digitalRead(BUTTON_PIN)){ 
    controller = "1"  ;
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(LED_PIN, 1);
    delay(2000);
    controller = "0"  ;
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(LED_PIN, 0);
   // delay(2000);
  }
}



void SendNulls()
{
  for(int i=0; i<5; i++)
  {
    UsiSerial.write((uint8_t)0);
  }
}
