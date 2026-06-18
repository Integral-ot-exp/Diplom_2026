#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>
#include <stm32f1xx_hal.h>

#define CE_PIN  PB0
#define CSN_PIN PB1
#define ADC_PIN PA0

RF24 radio(CE_PIN, CSN_PIN);
const byte pipeAddr[6] = "00001";

const uint8_t SAMPLES_PER_PKT = 16; // 16 отсчётов * 2 байта = 32 байта (макс. пакет)
uint16_t audioBuffer[SAMPLES_PER_PKT];
uint8_t sampleIdx = 0;

void setup(){
  Serial.begin(115200);
  pinMode(ADC_PIN, INPUT_ANALOG);

  radio.begin();
  radio.setChannel(100);
  radio.setDataRate(RF24_2MBPS);
  radio.openWritingPipe(pipeAddr);
  radio.setPALevel(RF24_PA_LOW);      // Низкая мощность для ближней связи (меньше помех)
  radio.setAutoAck(false);            // Отключаем ACK для строгого соблюдения интервала 300 мс
  radio.stopListening();              // Режим передачи
  
  Serial.println("TX: Готов к отправке");
}

void loop(){
  // Сэмплируем АЦП максимально быстро
  // Примечание: analogRead() на STM32duino ~5-8 мкс → ~10-15 кГц
  uint16_t sample = analogRead(ADC_PIN);
  //audioBuffer[sampleIdx++] = sample;
  
  radio.write(&sample, sizeof(sample));

  /*if (sampleIdx >= SAMPLES_PER_PKT) {
    // Отправляем пакет (блокирующая отправка с ACK)
    bool ok = radio.write(&audioBuffer, sizeof(audioBuffer));
    if (!ok) Serial.println("TX Failed");
    sampleIdx = 0;
  }*/
}

/*#include <Arduino.h>
#include <stm32f1xx_hal.h>
#include <SPI.h>
#include <RF24.h>

#define CE_PIN   PB0  // Избегаем PA4 (SPI1_NSS)
#define CSN_PIN  PB1
#define ADC_PIN  PA0
#define SAMPLES_PER_PKT 16

RF24 radio(CE_PIN, CSN_PIN);
const byte pipeAddr[6] = "00001";

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
TIM_HandleTypeDef htim3;

volatile uint16_t adcBuffer[32];
volatile bool halfReady = false, fullReady = false;

// Обязательно extern "C" для работы HAL-прерываний в C++
extern "C" {
  void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) { halfReady = true; }
  void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)    { fullReady = true; }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin(); // RF24 сам настроит SPI, дополнительный begin не нужен

  __HAL_RCC_ADC1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // TIM3 -> 50 кГц триггер
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.Period = 19;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&htim3);
  TIM_MasterConfigTypeDef sMaster = {0};
  sMaster.MasterOutputTrigger = TIM_TRGO_UPDATE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMaster);
  HAL_TIM_Base_Start(&htim3);

  // ADC
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = ADC_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);

  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  // DMA
  hdma_adc1.Instance = DMA1_Channel1;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1.Init.Mode = DMA_CIRCULAR;
  hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma_adc1);
  __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, 32);

  // nRF24L01
  radio.begin();
  delay(100); // Даем модулю время на инициализацию PLL
  radio.setChannel(100);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(0, 0);
  radio.setAutoAck(false);
  radio.setPayloadSize(32);       // КРИТИЧНО: явный размер пакета
  radio.setCRCLength(RF24_CRC_16);
  radio.openWritingPipe(pipeAddr);
  radio.stopListening();

  Serial.print("TX Ready. Baseline ADC: ");
  Serial.println(adcBuffer[0]);
}

void loop() {
  if (halfReady) {
    halfReady = false;
    radio.write((const void*)&adcBuffer[0], 32);
  }
  if (fullReady) {
    fullReady = false;
    radio.write((const void*)&adcBuffer[16], 32);
  }
}*/