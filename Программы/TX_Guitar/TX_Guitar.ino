/* 
 * GUITAR_RX_STM32.ino
 * Приёмник аудиосигнала. Вывод через ШИМ (PA4) + обязательный ФНЧ на выходе
 * Подключение:
 *   nRF24L01: CE=PB0, CSN=PB1
 *   Выход: PA4 (PWM/DAC) -> RC-фильтр -> УНЧ/Наушники
 */

#include <SPI.h>
#include <RF24.h>

#define CE_PIN  PB0
#define CSN_PIN PB1
#define OUT_PIN PA4

RF24 radio(CE_PIN, CSN_PIN);
const byte pipeAddr[] = "GUITAR";

const uint8_t SAMPLES_PER_PKT = 16;
uint16_t audioBuffer[SAMPLES_PER_PKT];

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN, OUTPUT);

  radio.begin();
  radio.setChannel(100);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, pipeAddr);
  radio.startListening(); // Режим RX

  Serial.println("RX Ready");
}

void loop() {
  if (radio.available()) {
    radio.read(&audioBuffer, sizeof(audioBuffer));
    
    // Вывод отсчётов
    for (uint8_t i = 0; i < SAMPLES_PER_PKT; i++) {
      // map() преобразует 12-бит (0-4095) в 8-бит ШИМ (0-255)
      uint8_t pwmVal = map(audioBuffer[i], 0, 4095, 0, 255);
      analogWrite(OUT_PIN, pwmVal);
      
      // Небольшая задержка для стабилизации ШИМ-периода
      // В реальном проекте заменяется на таймерный DMA или аппаратный ЦАП
      delayMicroseconds(60); 
    }
  }
}