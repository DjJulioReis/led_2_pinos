/*
  SISTEMA DE CONTROLE OWIRE LED - ESP32-C3 SUPER MINI

  Operação: DMX SOMENTE (Sem botões)
  - Canal 1: Seleção de Cor (0-255 -> 12 cores)
  - Canal 2: Seleção de Efeito (0-255 -> 8 modos)

  PASTA DO PROJETO: OWirePro (O arquivo deve se chamar OWirePro.ino)
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_OWire_Arduino_Library.h>
#include <Preferences.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OWIRE meusLeds;
#define PIN_OWIRE 6 // Saída GPIO 6 conforme solicitado

// Estados
uint8_t dmx_val_ch1 = 0;
uint8_t dmx_val_ch2 = 0;
uint8_t current_color = OW_WHITE;
uint8_t current_mode = OW_SOLID;

const char* color_names[] = {"OFF", "RED", "GREEN", "YELLOW", "BLUE", "VIOLET", "CYAN", "WHITE", "RANDOM", "RGW", "RBW", "6COLOR"};
const char* mode_names[] = {"SOLID", "FADE 8S", "SPARKLE W-S", "WAVE FADE", "SPARKLE C-S", "SPARKLE W-F", "COLOR BLINK", "SPARKLE C-F"};

void updateLEDs() {
  meusLeds.setModeAndColor(current_mode, current_color);
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("OWIRE DMX MONITOR");
  display.drawLine(0, 10, 128, 10, WHITE);

  display.setCursor(0, 20);
  display.print("DMX CH 1 (COR): "); display.println(dmx_val_ch1);
  display.print(" -> "); display.println(color_names[current_color % 12]);

  display.setCursor(0, 40);
  display.print("DMX CH 2 (EFE): "); display.println(dmx_val_ch2);
  display.print(" -> "); display.println(mode_names[(current_mode >> 4) % 8]);

  display.display();
}

unsigned long last_dmx_byte = 0;
void handleDMX() {
  if (Serial1.available()) {
    // Detecção de novo frame DMX por tempo de silêncio
    if (millis() - last_dmx_byte > 10) {
       int count = 0;
       bool changed = false;
       while(Serial1.available() && count < 513) {
         uint8_t val = Serial1.read();
         if (count == 1) { // CH 1
            if (val != dmx_val_ch1) {
              dmx_val_ch1 = val;
              current_color = map(val, 0, 255, 0, 11);
              changed = true;
            }
         }
         if (count == 2) { // CH 2
            if (val != dmx_val_ch2) {
              dmx_val_ch2 = val;
              current_mode = map(val, 0, 255, 0, 7) << 4;
              changed = true;
            }
         }
         count++;
       }

       if (changed) {
         updateLEDs();
         updateDisplay();
       }
    }
    last_dmx_byte = millis();
  }
}

void setup() {
  Wire.begin(8, 9);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  meusLeds.begin(PIN_OWIRE, false);

  // DMX RX em Serial1 (GPIO 20)
  Serial1.begin(250000, SERIAL_8N2, 20, -1);

  updateDisplay();
}

void loop() {
  handleDMX();
  delay(1);
}
