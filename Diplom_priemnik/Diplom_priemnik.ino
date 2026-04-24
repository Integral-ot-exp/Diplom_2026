//Arduino
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*#define SCK 55
#define MISO 6
#define MOSI 7
#define CE 16
#define CS 4*/
 
RF24 radio(PB0, PA4); // CE, CSN on Blue Pill
const uint64_t pope = 0xE8E8F0F0E1LL;
//boolean button_state = 0;

// The sizeof this struct should not exceed 32 bytes
/*struct received_data {
  byte throttle;
};*/

int ch1_value = 0;
//Received_data received_data;
//int throttle = received_data.throttle;   //Reading the data


 
void setup() 
{  
  Serial.begin(9600);
  //received_data.throttle = 127;
  radio.powerUp();
  pinMode(PC13, OUTPUT);
  radio.begin();
  radio.setAutoAck(0);
  radio.setDataRate(RF24_1MBPS);
  //Serial.print("ADDRESS :");
  radio.openReadingPipe(1, pope);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.setChannel(0x80);
  radio.setPayloadSize(4);   // размер пакета, в байтах

  
  radio.startListening();              //This sets the module as receiver
}

unsigned long last_Time = 0;

//We create the function that will read the data each certain time
void receive_the_data()
{
  
}

void loop()
{
  //receive_the_data();
  int throttle;
  while ( radio.available() ) {
  radio.read(&throttle, sizeof(throttle));
  //last_Time = millis(); //Here we receive the data
  }
  if (radio.available())
  {              //Looking for the data.
    if (throttle == 1)
    {
      digitalWrite(PC13, HIGH);
      delay(300);
      //Serial.println(val);  
    }
    if (throttle == 0)
    {
      digitalWrite(PC13, LOW);
      delay(300);
      //Serial.println(val);  
    }
 }
 else
  {
    digitalWrite(PC13, LOW);
    delay(50);
//    Serial.println(val);  
    digitalWrite(PC13, HIGH);
    delay(50);
    //Serial.println(val);  
  }
}