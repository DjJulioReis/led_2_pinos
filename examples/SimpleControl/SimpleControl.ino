/*
  Exemplo de controle INDEPENDENTE de LEDs RGB de 2 pinos (OWire) usando ESP32-C3.

  Este código permite controlar duas fitas/LEDs em pinos diferentes de forma independente
  através do Monitor Serial.

  Conexões:
  - Fita 1: GPIO 2
  - Fita 2: GPIO 3

  Comandos via Serial (9600 ou 115200):
  'r' -> Vermelho (Fita 1)
  'g' -> Verde (Fita 1)
  'b' -> Azul (Fita 1)
  '1' -> Modo Sólido (Fita 1)
  '2' -> Modo Fade (Fita 1)

  'R' -> Vermelho (Fita 2)
  'G' -> Verde (Fita 2)
  'B' -> Azul (Fita 2)
  '!' -> Modo Sólido (Fita 2)
  '@' -> Modo Fade (Fita 2)
*/

#include <SparkFun_OWire_Arduino_Library.h>

// Pinos de controle
const int PIN_LED_1 = 2;
const int PIN_LED_2 = 3;

// Instâncias independentes
OWIRE fita1;
OWIRE fita2;

void setup() {
  Serial.begin(115200);
  while(!Serial); // Aguarda abrir o monitor

  Serial.println("--- Controle OWire ESP32-C3 ---");

  // Inicializa as fitas.
  // Use 'true' se estiver usando MOSFET P-Channel (Recomendado para estabilidade)
  fita1.begin(PIN_LED_1, false);
  fita2.begin(PIN_LED_2, false);

  // Estados iniciais
  fita1.setModeAndColor(OW_SOLID, OW_WHITE);
  fita2.setModeAndColor(OW_SOLID, OW_WHITE);

  Serial.println("Pronto! Digite comandos no Serial Monitor.");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch(cmd) {
      // Fita 1 - Cores
      case 'r': fita1.setColor(OW_RED); Serial.println("Fita 1: Vermelho"); break;
      case 'g': fita1.setColor(OW_GREEN); Serial.println("Fita 1: Verde"); break;
      case 'b': fita1.setColor(OW_BLUE); Serial.println("Fita 1: Azul"); break;
      case 'w': fita1.setColor(OW_WHITE); Serial.println("Fita 1: Branco"); break;

      // Fita 1 - Modos
      case '1': fita1.setMode(OW_SOLID); Serial.println("Fita 1: Solido"); break;
      case '2': fita1.setMode(OW_8SECONDFADE); Serial.println("Fita 1: Fade 8s"); break;
      case '3': fita1.setMode(OW_COLORSPARKLES_FAST); Serial.println("Fita 1: Sparkles"); break;

      // Fita 2 - Cores
      case 'R': fita2.setColor(OW_RED); Serial.println("Fita 2: Vermelho"); break;
      case 'G': fita2.setColor(OW_GREEN); Serial.println("Fita 2: Verde"); break;
      case 'B': fita2.setColor(OW_BLUE); Serial.println("Fita 2: Azul"); break;
      case 'W': fita2.setColor(OW_WHITE); Serial.println("Fita 2: Branco"); break;

      // Fita 2 - Modos
      case '!': fita2.setMode(OW_SOLID); Serial.println("Fita 2: Solido"); break;
      case '@': fita2.setMode(OW_8SECONDFADE); Serial.println("Fita 2: Fade 8s"); break;
      case '#': fita2.setMode(OW_COLORSPARKLES_FAST); Serial.println("Fita 2: Sparkles"); break;
    }
  }
}
