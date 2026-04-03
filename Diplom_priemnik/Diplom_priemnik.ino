#include <stdint.h>
#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(10,4);
const uint64_t pope = 0xb3b4b5b6f1;
void setup(void){
 pinMode(PC13, OUTPUT);
 Serial.begin(115200);
 radio.begin();
 radio.setChannel(2);
 radio.setPayloadSize(7);
 radio.setDataRate(RF24_250KBPS);
 radio.openReadingPipe(1,pope);
 radio.startListening();
}
 
void loop(void){
 if (radio.available()){  
  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);
  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
 }
 else{
  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);
  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  delay(300);
  //Serial.println("No radio available");
 }
}