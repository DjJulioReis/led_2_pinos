# Guia de Controle de LEDs RGB de 2 Pinos com ESP32-C3 Super Mini

Este projeto permite controlar LEDs RGB de 2 pinos (conhecidos como OWire) usando um microcontrolador ESP32-C3 Super Mini (frequentemente confundido com STM32 devido ao formato "mini").

## 1. Instalação da Biblioteca

Para usar este código, você precisa da biblioteca **SparkFun OWire Arduino Library**.

1. Abra o Arduino IDE.
2. Vá em **Sketch** -> **Include Library** -> **Add .ZIP Library...**.
3. Selecione a pasta ou o arquivo ZIP da biblioteca contida neste repositório: `SparkFun_OWire_Arduino_Library-main`.

## 2. Configuração do Hardware

Os LEDs OWire funcionam variando a tensão no pino de alimentação para enviar comandos.

### Conexão Simples (Apenas 1 ou 2 LEDs)
Se você for ligar apenas um LED para teste, pode conectar diretamente ao pino do ESP32:

- **Anodo do LED (Perna Longa/+)**: Conecte ao pino **GPIO 2** do ESP32-C3.
- **Catodo do LED (Perna Curta/-)**: Conecte ao **GND**.

*Nota: O ESP32-C3 fornece 3.3V. Alguns LEDs RGB funcionam melhor com 5V. Se o brilho estiver muito baixo ou as cores não mudarem corretamente, use o método com MOSFET abaixo.*

### Conexão de Potência (Fitas ou Muitos LEDs)
Para controlar muitos LEDs ou usar uma fonte de 5V externa, você deve usar um **MOSFET de Canal P** (ex: IRF9540) para chavear o positivo da fonte.

1. **Fonte 5V (+) -> MOSFET Source**.
2. **MOSFET Drain -> Anodo dos LEDs (+)**.
3. **ESP32 GPIO 2 -> Resistor 1k -> MOSFET Gate**.
4. **Catodo dos LEDs (-) -> GND comum**.
5. **GND do ESP32 -> GND comum**.

*Importante: No código, ao usar MOSFET, você deve iniciar a biblioteca com `meusLeds.begin(PIN_LED, true);` para inverter a lógica.*

## 3. Configuração no Arduino IDE

1. Vá em **Tools** -> **Board** -> **ESP32 Arduino**.
2. Selecione **ESP32C3 Dev Module** (ou similar para o Super Mini).
3. Selecione a **Porta COM** correta.
4. Clique em **Upload**.

## 4. Comandos Principais

No seu código, você pode usar:

- `meusLeds.setColor(OW_RED)`: Muda a cor (RED, GREEN, BLUE, YELLOW, VIOLET, CYAN, WHITE).
- `meusLeds.setMode(OW_SOLID)`: Muda o efeito (SOLID, 8SECONDFADE, WHITESPARKLES_FAST, etc).
- `meusLeds.setModeAndColor(modo, cor)`: Define ambos simultaneamente (recomendado para a primeira chamada).
