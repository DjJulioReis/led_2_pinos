/*
  SISTEMA PROFISSIONAL DE CONTROLE OWIRE LED - ESP32-C3 SUPER MINI

  Recursos:
  - Controle de LEDs RGB 2 pinos (OWire)
  - Display OLED (I2C) para interface visual
  - 4 Botões para navegação de menu (UP, DOWN, SELECT, BACK)
  - Controle via Bluetooth (BLE) para App Flutter
  - Controle via DMX512 (Requer módulo RS485)
  - Memória Permanente (Preferences) para salvar último estado

  PINOUT ESP32-C3 SUPER MINI (Otimizado):
  - GPIO 10: Saída LED OWire (Evitando strapping pins)
  - GPIO 8: SDA (OLED)
  - GPIO 9: SCL (OLED)
  - GPIO 3: Botão UP (Pull-up)
  - GPIO 4: Botão DOWN (Pull-up)
  - GPIO 5: Botão SELECT (Pull-up)
  - GPIO 6: Botão BACK (Pull-up)
  - GPIO 20: DMX RX (Conectar ao RO do módulo RS485)
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

// Configurações OWire
OWIRE meusLeds;
#define PIN_OWIRE 10

// Botões
#define BTN_UP 3
#define BTN_DOWN 4
#define BTN_SELECT 5
#define BTN_BACK 6

// Bluetooth (BLE)
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Estados e Memória
Preferences preferences;
uint8_t current_color = OW_WHITE;
uint8_t current_mode = OW_SOLID;
int dmx_address = 1;
bool is_menu_mode = false;
int menu_index = 0;

const char* color_names[] = {"OFF", "RED", "GREEN", "YELLOW", "BLUE", "VIOLET", "CYAN", "WHITE", "RANDOM", "RGW", "RBW", "6COLOR"};
const char* mode_names[] = {"SOLID", "FADE 8S", "SPARKLE W-S", "WAVE FADE", "SPARKLE C-S", "SPARKLE W-F", "COLOR BLINK", "SPARKLE C-F"};

void saveState() {
  preferences.begin("led-settings", false);
  preferences.putUChar("color", current_color);
  preferences.putUChar("mode", current_mode);
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
    display.print("COR: ");
    display.println(color_names[current_color > 11 ? 0 : current_color]);

    display.print("MODO: ");
    display.println(mode_names[(current_mode >> 4) > 7 ? 0 : (current_mode >> 4)]);

    display.setCursor(0, 50);
    display.println("PRESS SELECT P/ MENU");
  } else {
    display.setCursor(0, 20);
    display.println("> MENU CONFIG");
    display.print(menu_index == 0 ? "[X] COR: " : "    COR: ");
    display.println(color_names[current_color]);
    display.print(menu_index == 1 ? "[X] MODO: " : "    MODO: ");
    display.println(mode_names[current_mode >> 4]);
  }

  display.display();
}

// Callback BLE
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      if (value.length() >= 2) {
        current_mode = value[0];
        current_color = value[1];
        updateLEDs();
        saveState();
        updateDisplay();
      }
    }
};

// DMX Logic
unsigned long last_dmx_packet = 0;
void handleDMX() {
  if (Serial1.available()) {
    // Implementação simplificada: se recebermos dados em massa após um tempo sem nada,
    // assumimos que é o início do frame (pobre detecção de BREAK).
    if (millis() - last_dmx_packet > 50) {
       // Possível início de frame
       int count = 0;
       while(Serial1.available() && count < 512) {
         uint8_t val = Serial1.read();
         if (count == dmx_address) {
            // Canal de Cor
            current_color = map(val, 0, 255, 0, 11);
         }
         if (count == dmx_address + 1) {
            // Canal de Modo
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
  // Botões
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  // OLED
  Wire.begin(8, 9);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // Erro crítico
  }

  // OWire
  meusLeds.begin(PIN_OWIRE, false);

  // Recuperar Memória
  preferences.begin("led-settings", true);
  current_color = preferences.getUChar("color", OW_WHITE);
  current_mode = preferences.getUChar("mode", OW_SOLID);
  preferences.end();

  // BLE
  BLEDevice::init("ESP32-OWire-Pro");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEDevice::getAdvertising()->start();

  // DMX (Serial1)
  Serial1.begin(250000, SERIAL_8N2, 20, -1);

  updateLEDs();
  updateDisplay();
}

void loop() {
  handleDMX();

  // Navegação de Menu
  if(digitalRead(BTN_SELECT) == LOW) {
    is_menu_mode = !is_menu_mode;
    updateDisplay();
    delay(250);
  }

  if(is_menu_mode) {
    if(digitalRead(BTN_UP) == LOW) {
      menu_index = (menu_index + 1) % 2;
      updateDisplay();
      delay(200);
    }
    if(digitalRead(BTN_DOWN) == LOW) {
      if(menu_index == 0) {
        current_color = (current_color + 1) % 12;
      } else {
        uint8_t m = current_mode >> 4;
        m = (m + 1) % 8;
        current_mode = m << 4;
      }
      updateLEDs();
      saveState();
      updateDisplay();
      delay(200);
    }
  }

  if(digitalRead(BTN_BACK) == LOW) {
    is_menu_mode = false;
    updateDisplay();
    delay(250);
  }

  delay(10);
}
