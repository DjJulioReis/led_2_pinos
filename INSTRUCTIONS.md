# Sistema Mileto Indústria - ESP32 Dev Kit V1

Este firmware foi adaptado para o **ESP32 Dev Kit V1 (30 pinos)**.

## 1. Pinagem (Hardware)

| Componente | Pino ESP32 DevKit | Notas |
| :--- | :--- | :--- |
| **LED OWire** | **GPIO 4** | Saída para os LEDs. |
| **OLED SDA** | **GPIO 21** | Padrão I2C. |
| **OLED SCL** | **GPIO 22** | Padrão I2C. |
| **Encoder CLK**| **GPIO 13** | |
| **Encoder DT** | **GPIO 14** | |
| **Encoder SW** | **GPIO 27** | Clique p/ Alternar Modo. |
| **DMX RX** | **GPIO 16** | Conecte ao pino RO do MAX485 (RX2). |

## 2. Operação
- **Modo DMX:** O sistema obedece aos canais 1 (Cor) e 2 (Efeito) da mesa DMX.
- **Modo MANUAL:** Use o encoder para mudar a cor. O sistema entra em modo manual automaticamente ao girar o encoder.
- **Alternar Modo:** Pressione o botão do encoder para alternar manualmente entre DMX e MANUAL.

## 3. Segurança e Branding
O App Flutter Mileto irá procurar o dispositivo "Mileto-DevKit-LED" e validar o código de segurança. Certifique-se de que o dispositivo está alimentado corretamente (5V no pino VIN ou via USB).

## 4. Bibliotecas Necessárias
- `Adafruit SSD1306` & `Adafruit GFX`
- `Preferences` (Nativa)
- `BLEDevice` (Nativa)
- `SparkFun OWire Arduino Library`
