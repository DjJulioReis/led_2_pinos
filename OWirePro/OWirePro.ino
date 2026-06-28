/*
  SISTEMA DE CONTROLE OWIRE LED - ESP32-C3 SUPER MINI

  Configuração DMX:
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
#define PIN_OWIRE 10

// Botões
#define BTN_UP 3
#define BTN_DOWN 4
#define BTN_SELECT 5
#define BTN_BACK 6

// Estados
Preferences preferences;
uint8_t current_color = OW_WHITE;
uint8_t current_mode = OW_SOLID;
enum ControlSource { SOURCE_MANUAL, SOURCE_DMX };
ControlSource current_source = SOURCE_DMX;

const char* color_names[] = {"OFF", "RED", "GREEN", "YELLOW", "BLUE", "VIOLET", "CYAN", "WHITE", "RANDOM", "RGW", "RBW", "6COLOR"};
const char* mode_names[] = {"SOLID", "FADE 8S", "SPARKLE W-S", "WAVE FADE", "SPARKLE C-S", "SPARKLE W-F", "COLOR BLINK", "SPARKLE C-F"};

void saveSettings() {
  preferences.begin("led-settings", false);
  preferences.putUChar("color", current_color);
  preferences.putUChar("mode", current_mode);
  preferences.putInt("source", (int)current_source);
  preferences.end();
}

void updateLEDs() {
  meusLeds.setModeAndColor(current_mode, current_color);
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("OWIRE DMX CONTROL");
  display.drawLine(0, 10, 128, 10, WHITE);

  display.setCursor(0, 20);
  display.print("MODO: "); display.println(current_source == SOURCE_DMX ? "DMX (CH 1,2)" : "MANUAL");

  display.print("COR : ");
  display.println(color_names[current_color % 12]);

  display.print("EFET: ");
  display.println(mode_names[(current_mode >> 4) % 8]);

  display.setCursor(0, 55);
  display.println("SELECT: Trocar Modo");
  display.display();
}

// Lógica DMX com tentativa de sincronia simples
unsigned long last_dmx_byte = 0;
void handleDMX() {
  if (current_source != SOURCE_DMX) return;

  if (Serial1.available()) {
    // Se passou muito tempo desde o último byte, assumimos novo frame
    if (millis() - last_dmx_byte > 10) {
       // Pequena limpeza para alinhar com o início do pacote
       int count = 0;
       while(Serial1.available() && count < 513) {
         uint8_t val = Serial1.read();
         if (count == 1) { // CH 1
            current_color = map(val, 0, 255, 0, 11);
         }
         if (count == 2) { // CH 2
            current_mode = map(val, 0, 255, 0, 7) << 4;
            updateLEDs();
            updateDisplay();
         }
         count++;
       }
    }
    last_dmx_byte = millis();
  }
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  Wire.begin(8, 9);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  meusLeds.begin(PIN_OWIRE, false);

  preferences.begin("led-settings", true);
  current_color = preferences.getUChar("color", OW_WHITE);
  current_mode = preferences.getUChar("mode", OW_SOLID);
  current_source = (ControlSource)preferences.getInt("source", (int)SOURCE_DMX);
  preferences.end();

  Serial1.begin(250000, SERIAL_8N2, 20, -1);

  updateLEDs();
  updateDisplay();
}

void loop() {
  handleDMX();

  // Troca de modo
  if(digitalRead(BTN_SELECT) == LOW) {
    current_source = (current_source == SOURCE_DMX) ? SOURCE_MANUAL : SOURCE_DMX;
    saveSettings();
    updateDisplay();
    delay(500);
  }

  // Controle Manual
  if (current_source == SOURCE_MANUAL) {
    bool changed = false;
    if(digitalRead(BTN_UP) == LOW) {
      current_color = (current_color + 1) % 12;
      changed = true;
      delay(200);
    }
    if(digitalRead(BTN_DOWN) == LOW) {
      uint8_t m = current_mode >> 4;
      m = (m + 1) % 8;
      current_mode = m << 4;
      changed = true;
      delay(200);
    }
    if (changed) {
      updateLEDs();
      updateDisplay();
      saveSettings();
    }
  }
}
