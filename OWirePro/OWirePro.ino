/*
  SISTEMA DE CONTROLE OWIRE LED - ESP32-C3 SUPER MINI
  VERSÃO DE DIAGNÓSTICO E SEGURANÇA

  PASTA DO PROJETO: OWirePro (O arquivo deve se chamar OWirePro.ino)
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_OWire_Arduino_Library.h>
#include <Preferences.h>

// PINOS SUPER MINI C3
#define OLED_SDA 0
#define OLED_SCL 1
#define PIN_OWIRE 6
#define PIN_ONBOARD_LED 8
#define PIN_DMX_RX 20

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OWIRE meusLeds;
Preferences preferences;

const char* color_names[] = {"OFF", "RED", "GREEN", "YELLOW", "BLUE", "VIOLET", "CYAN", "WHITE", "RANDOM", "RGW", "RBW", "6COLOR"};
uint8_t dmx_val_ch1 = 0, dmx_val_ch2 = 0;
uint8_t current_color = OW_WHITE, current_mode = OW_SOLID;

void setup() {
  Serial.begin(115200);

  // 1. LED de Status
  pinMode(PIN_ONBOARD_LED, OUTPUT);
  digitalWrite(PIN_ONBOARD_LED, LOW); // Liga

  // 2. OLED em pinos alternativos
  Wire.begin(OLED_SDA, OLED_SCL);
  if(display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("OWIRE DIAGNOSTICO");
    display.display();
  }

  // 3. Recuperar Memoria
  preferences.begin("led-settings", true);
  current_color = preferences.getUChar("color", OW_WHITE);
  current_mode = preferences.getUChar("mode", OW_SOLID);
  preferences.end();

  // 4. Teste LED OWire
  if(meusLeds.begin(PIN_OWIRE, false)) {
    meusLeds.setModeAndColor(current_mode, current_color);
  }

  // 5. DMX
  Serial1.begin(250000, SERIAL_8N2, PIN_DMX_RX, -1);

  digitalWrite(PIN_ONBOARD_LED, HIGH); // Desliga apos iniciar
}

unsigned long last_dmx_packet = 0;
void loop() {
  if (Serial1.available()) {
    // Se houver uma pausa no sinal DMX, limpamos o buffer para tentar sincronizar
    if (millis() - last_dmx_packet > 50) {
      while(Serial1.available() > 0) Serial1.read();
    }

    // Leitura simplificada: Canal 0 (Start), Canal 1, Canal 2
    if (Serial1.available() >= 3) {
      uint8_t startCode = Serial1.read();
      if (startCode == 0) { // DMX Standard data packet
        uint8_t c1 = Serial1.read();
        uint8_t c2 = Serial1.read();

        bool changed = false;
        if (c1 != dmx_val_ch1) {
          dmx_val_ch1 = c1;
          current_color = map(c1, 0, 255, 0, 11);
          changed = true;
        }
        if (c2 != dmx_val_ch2) {
          dmx_val_ch2 = c2;
          current_mode = map(c2, 0, 255, 0, 7) << 4;
          changed = true;
        }

        if (changed) {
          meusLeds.setModeAndColor(current_mode, current_color);

          display.clearDisplay();
          display.setCursor(0,0);
          display.print("DMX CH1: "); display.println(dmx_val_ch1);
          display.print("DMX CH2: "); display.println(dmx_val_ch2);
          display.print("COR: "); display.println(color_names[current_color % 12]);
          display.display();

          preferences.begin("led-settings", false);
          preferences.putUChar("color", current_color);
          preferences.putUChar("mode", current_mode);
          preferences.end();
        }
      }
    }
    last_dmx_packet = millis();
  }

  // Heartbeat LED
  static unsigned long last_blink = 0;
  if (millis() - last_blink > 500) {
    digitalWrite(PIN_ONBOARD_LED, !digitalRead(PIN_ONBOARD_LED));
    last_blink = millis();
  }
}
