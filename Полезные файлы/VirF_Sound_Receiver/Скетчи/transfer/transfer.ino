/* Спец проект youtube - канала VirF Live Productions
Адрес канала www.youtube.com/c/VirFLiveProductions
Группа посвященная каналу https://vk.com/virf_live_productions
version 1.0 от 24.02.17
*/
#include <RF24.h>   
#include <SPI.h>
#include <RF24V.h>
RF24 radio(7,8);
RF24V Sound(radio,0);
void setup() {   
radio.begin(); 
radio.setChannel(0x4b);
radio.setDataRate(RF24_250KBPS); //250KBPS, 1MBPS, 2MBPS (250KBPS на nRF24L01 (без +) неработает)
Sound.begin();
Sound.transfer();} // - Включаем передачу 
void loop() {}

