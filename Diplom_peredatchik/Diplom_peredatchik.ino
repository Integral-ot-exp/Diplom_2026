#include <SPI.h>
#include <RF24.h>

// Пины управления (адаптированы под официальное ядро STM32)
#define CE_PIN  PA4
#define CSN_PIN PB0
#define LED_PIN PC13 // Встроенный светодиод на Blue Pill


RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001"; // Адрес канала (должен совпадать на приёмнике)

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);      // Низкая мощность для ближней связи (меньше помех)
  radio.setAutoAck(false);            // Отключаем ACK для строгого соблюдения интервала 300 мс
  radio.stopListening();              // Режим передачи
  
  Serial.println("TX: Готов к отправке");
}

void loop() {
  const char message[] = "LED_OFF";
  const char message2[] = "LED_ON";

  
  // Отправка сообщения
  radio.write(&message, sizeof(message));
  digitalWrite(LED_PIN, HIGH);
  
  // Строгая задержка 300 мс между отправками
  delay(300);

  // Отправка сообщения
  radio.write(&message2, sizeof(message2));
  digitalWrite(LED_PIN, LOW);

  // Строгая задержка 300 мс между отправками
  delay(300);
}