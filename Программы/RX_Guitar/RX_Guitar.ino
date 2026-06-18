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
#define OUT_PIN PA1

RF24 radio(CE_PIN, CSN_PIN);
const byte pipeAddr[6] = "00001";

const uint8_t SAMPLES_PER_PKT = 16;
uint16_t audioBuffer;


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
    Serial.println(audioBuffer);

    uint8_t pwmVal = map(audioBuffer, 0, 4095, 0, 255);
    analogWrite(OUT_PIN, pwmVal);
    // Вывод отсчётов
    //for (uint8_t i = 0; i < SAMPLES_PER_PKT; i++) {
      // map() преобразует 12-бит (0-4095) в 8-бит ШИМ (0-255)
      
      // Небольшая задержка для стабилизации ШИМ-периода
      // В реальном проекте заменяется на таймерный DMA или аппаратный ЦАП
      delayMicroseconds(60); 
    //}
  }

}

/*#include <Arduino.h>
#include <stm32f1xx_hal.h>
#include <SPI.h>
#include <RF24.h>

#define CE_PIN   PB0
#define CSN_PIN  PB1
#define PWM_PIN  PA1  // TIM3_CH2 (избегаем PA6 = SPI1_MISO)
#define SAMPLES_PER_PKT 16

RF24 radio(CE_PIN, CSN_PIN);
const byte pipeAddr[6] = "00001";

TIM_HandleTypeDef htim3;
uint16_t rxBuffer[SAMPLES_PER_PKT]; // volatile не нужен

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin();

  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = PWM_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.Period = 1023;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_PWM_Init(&htim3);

  TIM_OC_InitTypeDef sConfigOC = {0};
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 512;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

  radio.begin();
  delay(100);
  radio.setChannel(100);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(0, 0);
  radio.setAutoAck(false);
  radio.setPayloadSize(32);       // КРИТИЧНО: должен совпадать с TX
  radio.setCRCLength(RF24_CRC_16);
  radio.openReadingPipe(1, pipeAddr);
  radio.startListening();

  Serial.println("RX Ready");
}

void loop() {
  if (radio.available()) {
    radio.read(rxBuffer, 32);
    for (uint8_t i = 0; i < SAMPLES_PER_PKT; i++) {
      uint32_t pwmVal = rxBuffer[i] >> 2; // 12 бит -> 10 бит
      if (pwmVal > 1023) pwmVal = 1023;
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwmVal);
      delayMicroseconds(21); // ~47 кГц обновление ШИМ
    }
  }
}*/