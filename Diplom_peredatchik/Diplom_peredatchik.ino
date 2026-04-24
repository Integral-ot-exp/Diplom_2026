//#include <stdint.h>
/*#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
#define SCK 5
#define MISO 6
#define MOSI 7
#define CE 16
#define CS 4

//RF24 radio(CE, CS);
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
  CE_RESET;
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
}*/



#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(PB0, PA4); // CE, CSN on Blue Pill 
const uint64_t address = 0xF0F0F0F0E1LL;
int counter = 0;
 
void setup() 
{
Serial.begin(9600);
radio.begin();                  //Starting the Wireless communication
radio.openWritingPipe(address); //Setting the address where we will send the data
radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
radio.stopListening();          //This sets the module as transmitter
}
 
void loop()
{
char text[] = " Hello World";
char str[50];
sprintf(str,"%s %d",text,counter);
radio.write(&str, sizeof(str));  
 
Serial.println(str);
counter++;
delay(2000);
}