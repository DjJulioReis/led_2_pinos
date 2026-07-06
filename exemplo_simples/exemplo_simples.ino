/*
  SISTEMA OWIRE LED - VERSÃO SIMPLES (AUTO-CICLO)
  Corrigido para usar o nome da instância 'myLED' conforme a biblioteca.

  PASTA DO PROJETO: exemplo_simples
*/

#include <SparkFun_OWire_Arduino_Library.h>

// Pino de saída (GPIO 32 para Dev Kit)
#define PIN_OWIRE 32

// Nome da instância deve ser myLED (ou qualquer outro, mas seguiremos o exemplo oficial)
OWIRE myLED;

void setup() {
  Serial.begin(115200);

  // Inicializa a biblioteca no pino 32
  if(myLED.begin(PIN_OWIRE, false)) {
    Serial.println("OWire Iniciado com myLED no GPIO 32");
  }

  // Define modo solido inicial
  myLED.setMode(OW_SOLID);
}

void loop() {
  // Cicla por todas as 12 cores da biblioteca
  for (int i = 0; i < 12; i++) {
    Serial.print("Cor atual: ");
    Serial.println(i);

    myLED.setColor(i);
    delay(3000); // 3 segundos em cada cor
  }
}
