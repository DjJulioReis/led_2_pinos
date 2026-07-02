/*
  SISTEMA PROFISSIONAL OWIRE LED - MILETO INDÚSTRIA

  Recursos:
  - Controle de LEDs RGB 2 pinos (OWire) via GPIO 6
  - Interface com Encoder Rotativo (Giro = Seleção, Clique = OK)
  - Display OLED Monitor (I2C nos pinos 8 e 9)
  - Controle DMX512 (Canais 1 e 2) via GPIO 20
  - Autenticação de Hardware Mileto via BLE (Código de Segurança)
  - Memória Permanente (Preferences) para estado e fonte

  PINAGEM ESP32-C3 SUPER MINI:
  - GPIO 6: Saída LED OWire
  - GPIO 8/9: SDA/SCL OLED
  - GPIO 20: DMX RX (RS485)
  - GPIO 2: Encoder CLK
  - GPIO 3: Encoder DT
  - GPIO 4: Encoder SW (Botão)
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_OWire_Arduino_Library.h>
#include <Preferences.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Configurações OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// OWire
OWIRE myLed;
#define PIN_OWIRE 6

// Encoder
#define ENC_CLK 2
#define ENC_DT 3
#define ENC_SW 4

// Segurança Mileto (Para o App Flutter)
#define MILETO_SECURITY_CODE "MILETO_AUTH_2024_PRO"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define AUTH_UUID           "d21b0f5b-0c9f-4d3b-9c5c-7d9d9f5c4d2d"

// Estados
enum ControlSource { SOURCE_MANUAL, SOURCE_DMX };
ControlSource current_source = SOURCE_DMX;
uint8_t current_color = OW_WHITE;
uint8_t current_mode = OW_SOLID;
uint8_t dmx_ch1 = 0, dmx_ch2 = 0;

Preferences preferences;
bool is_menu_mode = false;
int menu_index = 0; // 0: Fonte, 1: Cor, 2: Modo

const char* color_names[] = {"OFF", "RED", "GREEN", "YELLOW", "BLUE", "VIOLET", "CYAN", "WHITE", "RANDOM", "RGW", "RBW", "6COLOR"};
const char* mode_names[] = {"SOLID", "FADE 8S", "SPARKLE W-S", "WAVE FADE", "SPARKLE C-S", "SPARKLE W-F", "COLOR BLINK", "SPARKLE C-F"};

// Lógica do Encoder
int lastClk = HIGH;
unsigned long lastButtonPress = 0;

void saveSettings() {
  preferences.begin("mileto-settings", false);
  preferences.putUChar("color", current_color);
  preferences.putUChar("mode", current_mode);
  preferences.putInt("source", (int)current_source);
  preferences.end();
}

void updateLEDs() {
  myLed.setModeAndColor(current_mode, current_color);
}

void showSplash() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(25, 10);
  display.println("MILETO");
  display.setTextSize(1);
  display.setCursor(35, 35);
  display.println("INDUSTRIA");
  display.drawLine(0, 50, 128, 50, WHITE);
  display.setCursor(15, 55);
  display.println("mileto.ind.br");
  display.display();
  delay(2000);
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("MILETO OWIRE PRO");
  display.drawLine(0, 10, 128, 10, WHITE);

  if (!is_menu_mode) {
    display.setCursor(0, 20);
    display.print("MODO: "); display.println(current_source == SOURCE_DMX ? "DMX Activo" : "Manual");
    display.print("COR : "); display.println(color_names[current_color % 12]);
    display.print("EFET: "); display.println(mode_names[(current_mode >> 4) % 8]);
    if(current_source == SOURCE_DMX) {
      display.setCursor(0, 50);
      display.print("CH1:"); display.print(dmx_ch1);
      display.print(" CH2:"); display.println(dmx_ch2);
    }
  } else {
    display.setCursor(0, 15);
    display.println("> CONFIGURACAO");
    display.print(menu_index == 0 ? "[X] FONTE: " : "    FONTE: ");
    display.println(current_source == SOURCE_DMX ? "DMX" : "Manual");
    display.print(menu_index == 1 ? "[X] COR:   " : "    COR:   ");
    display.println(color_names[current_color]);
    display.print(menu_index == 2 ? "[X] MODO:  " : "    MODO:  ");
    display.println(mode_names[current_mode >> 4]);
  }
  display.display();
}

void handleEncoder() {
  int newClk = digitalRead(ENC_CLK);
  if (newClk != lastClk && newClk == LOW) {
    if (digitalRead(ENC_DT) != newClk) {
      // Girou Horário
      if(is_menu_mode) {
        if(menu_index == 0) current_source = SOURCE_DMX;
        else if(menu_index == 1) current_color = (current_color + 1) % 12;
        else if(menu_index == 2) { uint8_t m = (current_mode >> 4); m = (m + 1) % 8; current_mode = m << 4; }
      } else {
        menu_index = (menu_index + 1) % 3;
      }
    } else {
      // Girou Anti-horário
      if(is_menu_mode) {
        if(menu_index == 0) current_source = SOURCE_MANUAL;
        else if(menu_index == 1) current_color = (current_color == 0) ? 11 : current_color - 1;
        else if(menu_index == 2) { uint8_t m = (current_mode >> 4); m = (m == 0) ? 7 : m - 1; current_mode = m << 4; }
      } else {
        menu_index = (menu_index == 0) ? 2 : menu_index - 1;
      }
    }
    if(is_menu_mode || current_source == SOURCE_MANUAL) updateLEDs();
    updateDisplay();
  }
  lastClk = newClk;

  // Botão do Encoder
  if (digitalRead(ENC_SW) == LOW) {
    if (millis() - lastButtonPress > 300) {
      is_menu_mode = !is_menu_mode;
      if(!is_menu_mode) saveSettings();
      updateDisplay();
      lastButtonPress = millis();
    }
  }
}

unsigned long lastDmx = 0;
void handleDMX() {
  if (current_source != SOURCE_DMX) return;
  if (Serial1.available()) {
    if (millis() - lastDmx > 10) {
      while(Serial1.available() > 0) Serial1.read();
      delay(20);
      int count = 0;
      bool changed = false;
      while (Serial1.available() && count < 5) {
        uint8_t val = Serial1.read();
        if (count == 1) {
          if(val != dmx_ch1) { dmx_ch1 = val; current_color = map(val, 0, 255, 0, 11); changed = true; }
        }
        else if (count == 2) {
          if(val != dmx_ch2) { dmx_ch2 = val; current_mode = map(val, 0, 255, 0, 7) << 4; changed = true; }
        }
        count++;
      }
      if(changed) { updateLEDs(); updateDisplay(); }
    }
    lastDmx = millis();
  }
}

void setup() {
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  Wire.begin(8, 9);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showSplash();

  myLed.begin(PIN_OWIRE, false);

  preferences.begin("mileto-settings", true);
  current_color = preferences.getUChar("color", OW_WHITE);
  current_mode = preferences.getUChar("mode", OW_SOLID);
  current_source = (ControlSource)preferences.getInt("source", (int)SOURCE_DMX);
  preferences.end();

  // BLE com Identificação Mileto
  BLEDevice::init("Mileto-Pro-LED");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Característica de Autenticação (Apenas Leitura)
  BLECharacteristic *pAuth = pService->createCharacteristic(AUTH_UUID, BLECharacteristic::PROPERTY_READ);
  pAuth->setValue(MILETO_SECURITY_CODE);

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->start();

  Serial1.begin(250000, SERIAL_8N2, 20, -1);
  updateLEDs();
  updateDisplay();
}

void loop() {
  handleEncoder();
  handleDMX();
  delay(1);
}
