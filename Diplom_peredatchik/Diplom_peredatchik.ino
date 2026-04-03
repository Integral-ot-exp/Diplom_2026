#include <stdint.h>
#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
char msg[6] = "hello";
RF24 radio(43, 6, 26, 48, 38);
uint64_t pope = 0xB3B4B5B6F1;
//RF24 radio(43, 6);
 
void setup(void) {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(2);
  radio.setPayloadSize(7);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pope);
}
void loop(void) {
  //Serial.println("send ...");
  radio.write(msg, 6);
  delay(3000);
}