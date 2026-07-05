/*
  SISTEMA PROFISSIONAL OWIRE LED - MILETO INDÚSTRIA
  VERSÃO ADAPTADA PARA ESP32 DEV KIT V1

  PASTA DO PROJETO: OWirePro
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_OWire_Arduino_Library.h>
#include <Preferences.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// PINAGEM ESP32 DEV KIT V1
#define PIN_OWIRE 32      // OWire LED (GPIO 32 conforme solicitado)
#define SDA_PIN 21        // I2C SDA (Padrão DevKit)
#define SCL_PIN 22        // I2C SCL (Padrão DevKit)
#define ENC_CLK 13        // Encoder CLK
#define ENC_DT 14         // Encoder DT
#define ENC_SW 27         // Encoder Button
#define PIN_DMX_RX 16     // DMX RX (Serial 2 - RX2)

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OWIRE myLed;
Preferences preferences;

// Segurança Mileto
#define MILETO_SECURITY_CODE "MILETO_AUTH_2024_PRO"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define AUTH_UUID           "d21b0f5b-0c9f-4d3b-9c5c-7d9d9f5c4d2d"

// Estados
uint8_t current_color = OW_WHITE;
uint8_t current_mode = OW_SOLID;
enum Source { MANUAL, DMX };
Source current_source = DMX;
uint8_t dmx_ch1 = 0, dmx_ch2 = 0;

const char* color_names[] = {"OFF", "RED", "GREEN", "YELLOW", "BLUE", "VIOLET", "CYAN", "WHITE", "RANDOM", "RGW", "RBW", "6COLOR"};
const char* mode_names[] = {"SOLID", "FADE 8S", "SPARKLE W-S", "WAVE FADE", "SPARKLE C-S", "SPARKLE W-F", "COLOR BLINK", "SPARKLE C-F"};

void saveSettings() {
  preferences.begin("mileto-pref", false);
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
  display.println("MILETO DEV-KIT V1");
  display.drawLine(0, 10, 128, 10, WHITE);

  display.setCursor(0, 20);
  display.print("MODO: "); display.println(current_source == DMX ? "DMX Activo" : "Manual");
  display.print("COR : "); display.println(color_names[current_color % 12]);
  display.print("EFE : "); display.println(mode_names[(current_mode >> 4) % 8]);

  if(current_source == DMX) {
    display.setCursor(0, 52);
    display.print("DMX CH1:"); display.print(dmx_ch1);
    display.print(" CH2:"); display.println(dmx_ch2);
  }
  display.display();
}

int lastClk = HIGH;
unsigned long lastButtonPress = 0;

void handleEncoder() {
  int nClk = digitalRead(ENC_CLK);
  if (nClk != lastClk && nClk == LOW) {
    if (digitalRead(ENC_DT) != nClk) {
      current_color = (current_color + 1) % 12;
    } else {
      current_color = (current_color == 0) ? 11 : current_color - 1;
    }
    current_source = MANUAL;
    updateLEDs();
    updateDisplay();
    saveSettings();
  }
  lastClk = nClk;

  if (digitalRead(ENC_SW) == LOW) {
    if (millis() - lastButtonPress > 400) {
      current_source = (current_source == DMX) ? MANUAL : DMX;
      saveSettings();
      updateDisplay();
      lastButtonPress = millis();
    }
  }
}

unsigned long last_dmx_packet = 0;
void handleDMX() {
  if (current_source != DMX) return;
  if (Serial2.available()) { // RX2 no Dev Kit
    if (millis() - last_dmx_packet > 10) {
      while(Serial2.available() > 0) Serial2.read();
      delay(20);
      int count = 0;
      bool changed = false;
      while(Serial2.available() && count < 5) {
        uint8_t val = Serial2.read();
        if (count == 1) { dmx_ch1 = val; current_color = map(val, 0, 255, 0, 11); changed = true; }
        if (count == 2) { dmx_ch2 = val; current_mode = map(val, 0, 255, 0, 7) << 4; changed = true; }
        count++;
      }
      if(changed) { updateLEDs(); updateDisplay(); }
    }
    last_dmx_packet = millis();
  }
}

void setup() {
  Serial.begin(115200);

  // I2C Dev Kit (21, 22)
  Wire.begin(SDA_PIN, SCL_PIN);
  if(display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    showSplash();
  }

  // OWire
  myLed.begin(PIN_OWIRE, false);

  // Encoder
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  // Recuperar Memoria
  preferences.begin("mileto-pref", true);
  current_color = preferences.getUChar("color", OW_WHITE);
  current_mode = preferences.getUChar("mode", OW_SOLID);
  current_source = (Source)preferences.getInt("source", (int)DMX);
  preferences.end();

  // BLE Seguranca Mileto
  BLEDevice::init("Mileto-DevKit-LED");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pAuth = pService->createCharacteristic(AUTH_UUID, BLECharacteristic::PROPERTY_READ);
  pAuth->setValue(MILETO_SECURITY_CODE);
  pService->start();
  BLEDevice::getAdvertising()->start();

  // DMX RX no RX2 (GPIO 16)
  Serial2.begin(250000, SERIAL_8N2, PIN_DMX_RX, -1);

  updateLEDs();
  updateDisplay();
}

void loop() {
  handleEncoder();
  handleDMX();
  delay(1);
}
