/*
  Exemplo de controle de LEDs RGB de 2 pinos (OWire) usando ESP32-C3 Super Mini.

  Este código utiliza a biblioteca SparkFun OWire Arduino Library.

  Conexões:
  - Se estiver usando apenas UM LED:
    - LED Anodo (perna longa/positiva) -> GPIO 2 do ESP32-C3
    - LED Catodo (perna curta/negativa) -> GND do ESP32-C3
    - *Nota: O ESP32-C3 opera a 3.3V, o que pode ser baixo para alguns LEDs RGB,
      mas geralmente funciona para testes simples. Para fitas ou muitos LEDs,
      use um MOSFET.*

  - Se estiver usando vários LEDs ou fonte externa:
    - Verifique o esquema recomendado no README da biblioteca (uso de MOSFET P-Channel).
*/

#include <SparkFun_OWire_Arduino_Library.h>

// Pino de controle
const int PIN_LED = 2;

// Cria uma instância do objeto OWIRE
OWIRE meusLeds;

void setup() {
  Serial.begin(115200);

  // Inicializa o pino.
  // O segundo parâmetro é 'true' se estiver usando lógica invertida (ex: MOSFET P-Channel)
  // Como estamos ligando direto no pino (para 1 LED), usamos 'false'.
  if(meusLeds.begin(PIN_LED, false)) {
    Serial.println("Biblioteca OWire inicializada com sucesso!");
  }

  // Define um estado inicial: Cor Sólida e Ciano
  meusLeds.setModeAndColor(OW_SOLID, OW_CYAN);
  delay(2000);
}

void loop() {
  // Ciclo de Cores
  Serial.println("Trocando cores...");
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
  Serial.println("Trocando modos...");

  // Fade de 8 segundos
  meusLeds.setMode(OW_8SECONDFADE);
  delay(8000);

  // Piscada branca rápida
  meusLeds.setMode(OW_WHITESPARKLES_FAST);
  delay(4000);

  // Arco-íris (Full Color)
  meusLeds.setModeAndColor(OW_SOLID, OW_FULLCOLOR);
  delay(5000);
}
