/*#include <stdint.h>
//#include "main.h"
#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
//RF24 radio(10,4);
#define SCK 15
#define MISO 16
#define MOSI 17
#define CE 31
#define CS 14

RF24 radio(CE, CS);
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
  //digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);
  radio.read(msg, 6);
  if (msg == "1")
  {
    digitalWrite(PC13, LOW);
  }
  else {
    digitalWrite(PC13, HIGH);
  }    // turn the LED off by making the voltage LOW
  //delay(300);
 }
 else{
  //digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  /*delay(300);
  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);*/
  //Serial.println("No radio available");
// }
//}*/

//Arduino
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(PB0, PA4); // CE, CSN on Blue Pill
const uint64_t address = 0xB3B4B5B6F1;
boolean button_state = 0;
 
void setup() 
{
 // SPI.begin(SCK, MISO, MOSI, SS);
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
  radio.begin();
  radio.setAutoAck(false);

  //Serial.print("ADDRESS :");
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  //radio.setChannel(2);
  radio.startListening();              //This sets the module as receiver
}

void loop()
{
  if (radio.available())              //Looking for the data.
  {
   // Serial.println("Radio is sniffing");
  
    char text[6] = "";                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data
   //Serial.println(text);
    if (text == "1")
    {
      digitalWrite(PC13, LOW);
      delay(200);
    }
    if (text == "0")
    {
      digitalWrite(PC13, HIGH);
      delay(200);
    }
  }
}