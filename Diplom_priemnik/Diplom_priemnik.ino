#include <SPI.h>
#include <RF24.h>

#define CE_PIN  PA4
#define CSN_PIN PB0
#define LED_PIN PC13 // Встроенный светодиод на Blue Pill

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001"; // Тот же адрес, что и у передатчика

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED выключен (на Blue Pill светодиод активный низкий)
  
  Serial.begin(115200);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);            // Должно совпадать с передатчиком
  radio.startListening();             // Режим приёма
  
  Serial.println("RX: Ожидание данных...");
}

void loop() {
  if (radio.available()) {
    char message[32] = {0};
    radio.read(&message, sizeof(message));
    
    Serial.print("RX Получено: ");
    Serial.println(message);
    
    if (strcmp(message, "LED_ON") == 0){
    digitalWrite(LED_PIN, LOW);}
    if (strcmp(message, "LED_OFF") == 0){
      digitalWrite(LED_PIN, HIGH);
    }
     // Включаем светодиод
    // Примечание: светодиод останется включённым до перезагрузки или явного выключения.
    // При необходимости добавьте таймер выключения через millis().
  }
}