/*
  SISTEMA PROFISSIONAL DE CONTROLE OWIRE LED - ESP32-C3 SUPER MINI

  Recursos:
  - Controle de LEDs RGB 2 pinos (OWire)
  - Seleção de Fonte: MANUAL ou DMX
  - Display OLED (I2C) para interface visual
  - 4 Botões para navegação de menu (UP, DOWN, SELECT, BACK)
  - Controle via DMX512 (Requer módulo RS485)
  - Memória Permanente (Preferences) para salvar último estado

  PINOUT ESP32-C3 SUPER MINI:
  - GPIO 10: Saída LED OWire
  - GPIO 8: SDA (OLED)
  - GPIO 9: SCL (OLED)
  - GPIO 3: Botão UP
  - GPIO 4: Botão DOWN
  - GPIO 5: Botão SELECT
  - GPIO 6: Botão BACK
  - GPIO 20: DMX RX
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_OWire_Arduino_Library.h>
#include <Preferences.h>

// Configurações OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Configurações OWire
OWIRE meusLeds;
#define PIN_OWIRE 10

// Botões
#define BTN_UP 3
#define BTN_DOWN 4
#define BTN_SELECT 5
#define BTN_BACK 6

// Fontes de Controle
enum ControlSource { SOURCE_MANUAL, SOURCE_DMX };
const char* source_names[] = {"MANUAL", "DMX"};

// Estados e Memória
Preferences preferences;
uint8_t current_color = OW_WHITE;
uint8_t current_mode = OW_SOLID;
ControlSource current_source = SOURCE_MANUAL;
int dmx_address = 1;

// Menu
bool is_menu_mode = false;
int menu_index = 0; // 0: Fonte, 1: Cor, 2: Modo

const char* color_names[] = {"OFF", "RED", "GREEN", "YELLOW", "BLUE", "VIOLET", "CYAN", "WHITE", "RANDOM", "RGW", "RBW", "6COLOR"};
const char* mode_names[] = {"SOLID", "FADE 8S", "SPARKLE W-S", "WAVE FADE", "SPARKLE C-S", "SPARKLE W-F", "COLOR BLINK", "SPARKLE C-F"};

// Debounce não-bloqueante
unsigned long last_btn_press = 0;
const int debounce_delay = 200;

void saveState() {
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
  display.println("OWIRE PRO SYSTEM");
  display.drawLine(0, 10, 128, 10, WHITE);

  if (!is_menu_mode) {
    display.setCursor(0, 20);
    display.print("FONTE: ");
    display.println(source_names[current_source]);

    display.print("COR  : ");
    display.println(color_names[current_color > 11 ? 0 : current_color]);

    display.print("MODO : ");
    display.println(mode_names[(current_mode >> 4) > 7 ? 0 : (current_mode >> 4)]);

    display.setCursor(0, 50);
    display.println("PRESS SELECT P/ MENU");
  } else {
    display.setCursor(0, 15);
    display.println("> MENU CONFIG");

    display.print(menu_index == 0 ? "[X] FONTE: " : "    FONTE: ");
    display.println(source_names[current_source]);

    display.print(menu_index == 1 ? "[X] COR:   " : "    COR:   ");
    display.println(color_names[current_color]);

    display.print(menu_index == 2 ? "[X] MODO:  " : "    MODO:  ");
    display.println(mode_names[current_mode >> 4]);
  }

  display.display();
}

// DMX Logic
unsigned long last_dmx_packet = 0;
void handleDMX() {
  if (current_source != SOURCE_DMX) return;

  if (Serial1.available()) {
    // Reduzido gap para 5ms para melhor detecção de fim de frame DMX
    if (millis() - last_dmx_packet > 5) {
       int count = 0;
       while(Serial1.available() && count < 512) {
         uint8_t val = Serial1.read();
         if (count == dmx_address) {
            current_color = map(val, 0, 255, 0, 11);
         }
         if (count == dmx_address + 1) {
            current_mode = map(val, 0, 255, 0, 7) << 4;
            updateLEDs();
         }
         count++;
       }
    }
    last_dmx_packet = millis();
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
  current_source = (ControlSource)preferences.getInt("source", (int)SOURCE_MANUAL);
  preferences.end();

  // DMX RX em Serial1
  Serial1.begin(250000, SERIAL_8N2, 20, -1);

  updateLEDs();
  updateDisplay();
}

void loop() {
  handleDMX();

  // Navegação de Menu Não-Bloqueante
  if (millis() - last_btn_press > debounce_delay) {
    if(digitalRead(BTN_SELECT) == LOW) {
      is_menu_mode = !is_menu_mode;
      updateDisplay();
      last_btn_press = millis();
    }

    if(is_menu_mode) {
      if(digitalRead(BTN_UP) == LOW) {
        menu_index = (menu_index + 1) % 3;
        updateDisplay();
        last_btn_press = millis();
      }

      if(digitalRead(BTN_DOWN) == LOW) {
        if(menu_index == 0) { // Fonte
          current_source = (ControlSource)((current_source + 1) % 2);
        }
        else if(menu_index == 1) { // Cor
          current_color = (current_color + 1) % 12;
        }
        else if(menu_index == 2) { // Modo
          uint8_t m = current_mode >> 4;
          m = (m + 1) % 8;
          current_mode = m << 4;
        }

        if (current_source == SOURCE_MANUAL || menu_index == 0) {
          updateLEDs();
        }
        saveState();
        updateDisplay();
        last_btn_press = millis();
      }
    }

    if(digitalRead(BTN_BACK) == LOW) {
      is_menu_mode = false;
      updateDisplay();
      last_btn_press = millis();
    }
  }

  // Pequeno delay para estabilidade do sistema sem bloquear DMX
  delay(1);
}
