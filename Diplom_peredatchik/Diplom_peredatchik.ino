//#include <stdint.h>
#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
/*#define SCK 48
#define MISO 47
#define MOSI 38
#define CE 43
#define CS 0

RF24 radio(CE, CS);*/
RF24 radio(PB0, PA4);

//char msg[6] = "1";
//char msg2[6] = "0";
const uint64_t pope = 0xE8E8F0F0E1LL;

//int a = 1, b = 0;  

//struct MyData {
  int throttle; 
  //int throttle0; 
//};
//MyData data;

void setup(void)
{ 

  Serial.begin(9600);
  radio.powerUp();
  radio.begin();
  radio.setAutoAck(0);
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.setDataRate(RF24_1MBPS);
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(4);   // размер пакета, в байтах
  radio.openWritingPipe(pope);
  radio.setChannel(0x80);
  radio.stopListening();          //This sets the module as transmitter
  pinMode(PC13, OUTPUT);
}

void loop()
{
  throttle = 128;
  radio.write(&throttle, sizeof(throttle));
  digitalWrite(PC13, HIGH);
  delay(300);
  
  throttle = 0;
  radio.write(&throttle, sizeof(throttle));
  digitalWrite(PC13, LOW);
  delay(300);
}