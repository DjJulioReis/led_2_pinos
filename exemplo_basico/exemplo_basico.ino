/*
  Exemplo de controle de LEDs RGB de 2 pinos (OWire)
  PASTA: exemplo_basico
*/

#include <SparkFun_OWire_Arduino_Library.h>

// Pino de controle (Ajuste conforme seu hardware)
const int PIN_LED = 32;

// Nome da instância corrigido para myLED
OWIRE myLED;

void setup() {
  Serial.begin(115200);

  if(myLED.begin(PIN_LED, false)) {
    Serial.println("Biblioteca OWire inicializada!");
  }

  myLED.setModeAndColor(OW_SOLID, OW_CYAN);
  delay(2000);
}

void loop() {
  // Ciclo de Cores
  myLED.setColor(OW_RED);
  delay(1000);
  myLED.setColor(OW_GREEN);
  delay(1000);
  myLED.setColor(OW_BLUE);
  delay(1000);
  myLED.setColor(OW_WHITE);
  delay(1000);

  // Ciclo de Modos
  myLED.setMode(OW_8SECONDFADE);
  delay(8000);

  myLED.setModeAndColor(OW_SOLID, OW_FULLCOLOR);
  delay(5000);
}
