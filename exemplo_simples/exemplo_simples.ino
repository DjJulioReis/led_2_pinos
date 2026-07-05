/*
  SISTEMA OWIRE LED - VERSÃO SIMPLES (AUTO-CICLO)
  Este código apenas troca as cores fixas automaticamente, sem controles externos.

  PASTA DO PROJETO: exemplo_simples
*/

#include <SparkFun_OWire_Arduino_Library.h>

// Pino de saída (GPIO 32 conforme solicitado)
#define PIN_OWIRE 32

OWIRE myLed;

void setup() {
  Serial.begin(115200);

  // Inicializa a biblioteca
  if(myLed.begin(PIN_OWIRE, false)) {
    Serial.println("OWire Iniciado - Modo Auto-Ciclo no GPIO 32");
  }

  // Define modo solido inicial
  myLed.setMode(OW_SOLID);
}

void loop() {
  // Cicla por todas as 12 cores da biblioteca
  for (int i = 0; i < 12; i++) {
    Serial.print("Cor atual: ");
    Serial.println(i);

    myLed.setColor(i);
    delay(3000); // Fica 3 segundos em cada cor
  }
}
