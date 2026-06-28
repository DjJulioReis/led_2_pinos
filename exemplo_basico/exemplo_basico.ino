/*
  Exemplo de controle de LEDs RGB de 2 pinos (OWire) usando ESP32-C3 Super Mini.

  Este código utiliza a biblioteca SparkFun OWire Arduino Library.

  Conexões:
  - LED Anodo (perna longa/positiva) -> GPIO 10 do ESP32-C3
  - LED Catodo (perna curta/negativa) -> GND do ESP32-C3
*/

#include <SparkFun_OWire_Arduino_Library.h>

// Pino de controle
const int PIN_LED = 10;

// Cria uma instância do objeto OWIRE
OWIRE meusLeds;

void setup() {
  Serial.begin(115200);

  // Inicializa o pino.
  // O segundo parâmetro é 'true' se estiver usando lógica invertida (ex: MOSFET P-Channel)
  if(meusLeds.begin(PIN_LED, false)) {
    Serial.println("Biblioteca OWire inicializada!");
  }

  // Define um estado inicial: Cor Sólida e Ciano
  meusLeds.setModeAndColor(OW_SOLID, OW_CYAN);
  delay(2000);
}

void loop() {
  // Ciclo de Cores
  meusLeds.setColor(OW_RED);
  delay(1000);
  meusLeds.setColor(OW_GREEN);
  delay(1000);
  meusLeds.setColor(OW_BLUE);
  delay(1000);
  meusLeds.setColor(OW_YELLOW);
  delay(1000);
  meusLeds.setColor(OW_WHITE);
  delay(1000);

  // Ciclo de Modos
  meusLeds.setMode(OW_8SECONDFADE);
  delay(8000);

  meusLeds.setModeAndColor(OW_SOLID, OW_FULLCOLOR);
  delay(5000);
}
