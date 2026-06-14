/* 
 * GUITAR_TX_STM32.ino
 * Передатчик аудиосигнала гитары через nRF24L01
 * Подключение:
 *   nRF24L01: CE=PB0, CSN=PB1, SPI=PA5/6/7 (SPI1)
 *   АЦП: PA0 (сигнал с выхода ОУ со смещением 1.65В)
 */

#include <SPI.h>
#include <RF24.h>

#define CE_PIN  PB0
#define CSN_PIN PB1
#define ADC_PIN PA0

RF24 radio(CE_PIN, CSN_PIN);
const byte pipeAddr[] = "GUITAR";

const uint8_t SAMPLES_PER_PKT = 16; // 16 отсчётов * 2 байта = 32 байта (макс. пакет)
uint16_t audioBuffer[SAMPLES_PER_PKT];
uint8_t sampleIdx = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ADC_PIN, INPUT_ANALOG);

  // Настройка nRF24L01
  radio.begin();
  radio.setChannel(100);            // 2.500 ГГц (избегаем Wi-Fi)
  radio.setDataRate(RF24_2MBPS);    // Макс. скорость для минимизации задержки
  radio.setPALevel(RF24_PA_LOW);    // Экономия питания, меньше помех
  //radio.setRetries(2, 15);          // 2 интервала, до 15 повторов (баланс надёжности/задержки)
  radio.openWritingPipe(pipeAddr);
  radio.stopListening();            // Режим TX
  
  Serial.println("TX Ready");
}

void loop() {
  // Сэмплируем АЦП максимально быстро
  // Примечание: analogRead() на STM32duino ~5-8 мкс → ~10-15 кГц 
  uint16_t sample = analogRead(ADC_PIN);
  audioBuffer[sampleIdx++] = sample;

  if (sampleIdx >= SAMPLES_PER_PKT) {
    // Отправляем пакет (блокирующая отправка с ACK)
    bool ok = radio.write(&audioBuffer, sizeof(audioBuffer));
    if (!ok) Serial.println("TX Failed");
    sampleIdx = 0;
  }
}