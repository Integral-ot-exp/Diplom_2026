//#include <stdint.h>
#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
#define SCK 48
#define MISO 47
#define MOSI 38
#define CE 43
#define CS 0

RF24 radio(CE, CS);
char msg[6] = "1";
char msg2[6] = "0";
uint64_t pope = 0xB3B4B5B6F1;

//int a = 1, b = 0;  


void setup(void)
{
  //Serial.println(“Starting SPI”);
  //Serial.flush();
  //SPI.begin(SCK, MISO, MOSI, CS);
  /*Serial.println(“Started”);
  Serial.flush();

  while (!radio.begin(&SPI)) //активировать модуль
  {
    Serial.println(“Can’t find RF24”);
    Serial.flush();
    delay(1);
  }*/

  //Serial.println(“OK”);
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(pope);
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  //radio.setChannel(2);
  //radio.setPayloadSize(7);
  radio.setDataRate(RF24_250KBPS);
  

  radio.stopListening();          //This sets the module as transmitter

}



//RF24 radio(43, 6, 26, 48, 38);

//RF24 radio(43, 6);
 
/*void setup(void) {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(2);
  radio.setPayloadSize(7);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pope);
}*/
/*void loop(void) {
  //Serial.println("send ...");
  radio.write(msg, 6);
  delay(300);
  radio.write(msg2, 6);
  delay(100);
}*/

void loop()
{
//int counter = 0;
//char text[] = "1";
//char str[6];
//sprintf(str,"%s %d",text,counter);
radio.write(msg, sizeof(msg));  
 
//Serial.println(str);
//counter++;
delay(300);

radio.write(msg2, sizeof(msg2));
delay(300);
}