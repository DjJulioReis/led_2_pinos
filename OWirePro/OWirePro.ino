/*
  SISTEMA DE CONTROLE OWIRE LED - ESP32-C3 SUPER MINI

  Configuração DMX:
  - Canal 1 (Mesa): Cor (0-255 -> 12 cores)
  - Canal 2 (Mesa): Efeito (0-255 -> 8 modos)

  PASTA DO PROJETO: OWirePro
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_OWire_Arduino_Library.h>
#include <Preferences.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OWIRE myLed;
#define PIN_OWIRE 6

// Pinos I2C Padrão do ESP32-C3 Super Mini
#define SDA_PIN 8
#define SCL_PIN 9

Preferences preferences;
uint8_t dmx_val_ch1 = 0;
uint8_t dmx_val_ch2 = 0;
uint8_t current_color = OW_WHITE;
uint8_t current_mode = OW_SOLID;

const char* color_names[] = {"OFF", "RED", "GREEN", "YELLOW", "BLUE", "VIOLET", "CYAN", "WHITE", "RANDOM", "RGW", "RBW", "6COLOR"};

// Controle de gravação na memória (para evitar desgaste da Flash)
unsigned long last_change_time = 0;
bool needs_save = false;

void saveSettings() {
  preferences.begin("led-settings", false);
  preferences.putUChar("color", current_color);
  preferences.putUChar("mode", current_mode);
  preferences.end();
  needs_save = false;
  Serial.println("Configuracoes salvas na Flash.");
}

void setup() {
  Serial.begin(115200);

  // Inicializa I2C nos pinos 8 e 9
  Wire.begin(SDA_PIN, SCL_PIN);

  if(display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("OWIRE PRO DMX");
    display.display();
  }

  myLed.begin(PIN_OWIRE, false);

  // Recupera última configuração
  preferences.begin("led-settings", true);
  current_color = preferences.getUChar("color", OW_WHITE);
  current_mode = preferences.getUChar("mode", OW_SOLID);
  preferences.end();

  // DMX RX no GPIO 20 (Serial1)
  Serial1.begin(250000, SERIAL_8N2, 20, -1);

  myLed.setModeAndColor(current_mode, current_color);
}

void updateUI() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("OWIRE DMX MONITOR");
  display.print("CH1 (COR): "); display.println(dmx_val_ch1);
  display.print("CH2 (EFE): "); display.println(dmx_val_ch2);
  display.print("ST: "); display.println(color_names[current_color % 12]);
  display.display();
}

unsigned long last_dmx_packet = 0;

void loop() {
  if (Serial1.available()) {
    // Sincronia: Detecta silêncio para assumir início de frame
    if (millis() - last_dmx_packet > 10) {
      while(Serial1.available() > 0) Serial1.read();
      delay(20);

      int count = 0;
      bool changed = false;

      while (Serial1.available() && count < 5) {
        uint8_t val = Serial1.read();
        if (count == 1) { // Canal 1
          if (val != dmx_val_ch1) {
            dmx_val_ch1 = val;
            current_color = map(val, 0, 255, 0, 11);
            myLed.setColor(current_color);
            changed = true;
          }
        }
        else if (count == 2) { // Canal 2
          if (val != dmx_val_ch2) {
            dmx_val_ch2 = val;
            current_mode = map(val, 0, 255, 0, 7) << 4;
            myLed.setMode(current_mode);
            changed = true;
          }
        }
        count++;
      }

      if (changed) {
        updateUI();
        needs_save = true;
        last_change_time = millis();
      }
    }
    last_dmx_packet = millis();
  }

  // Grava na Flash apenas após 5 segundos de inatividade para evitar desgaste
  if (needs_save && (millis() - last_change_time > 5000)) {
    saveSettings();
  }
}
