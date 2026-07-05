# Sistema Mileto Indústria - ESP32-C3 OWire Pro

Este código foi otimizado para o **ESP32-C3 Super Mini (Blue)**, corrigindo a diferença de timing entre o processador original (ATmega328) e o novo chip.

## ⚠️ Por que os LEDs ficam todos acesos?
Se os LEDs ficarem todos acesos sem mudar de cor, existem dois motivos comuns:

1. **Diferença de Tensão (3.3V vs 5V):** O ESP32-C3 envia sinais em 3.3V, mas muitos LEDs OWire exigem pulsos de 5V para entender os comandos.
   - **Solução:** Use um **MOSFET P-Channel** (conforme explicado nos guias anteriores) para chavear uma fonte de 5V externa. Isso garante que os pulsos cheguem com 5V reais.
2. **Timing do Processador:** O ESP32 é muito mais rápido que o Arduino Uno. Atualizamos o código com uma função `sendCommand` customizada que usa tempos de espera maiores para garantir que o sinal OWire seja reconhecido.

## 1. Pinagem MILETO Final
| Componente | Pino ESP32-C3 |
| :--- | :--- |
| **LED OWire** | **GPIO 2** |
| **OLED SDA** | **GPIO 8** |
| **OLED SCL** | **GPIO 9** |
| **Encoder CLK**| **GPIO 3** |
| **Encoder DT** | **GPIO 4** |
| **Encoder SW** | **GPIO 5** |
| **DMX RX** | **GPIO 20** |

## 2. Operação
- **Giro do Encoder:** Altera a cor manualmente e muda para modo **MANUAL**.
- **Clique do Encoder:** Alterna entre modo **DMX** e modo **MANUAL**.
- **Splash Screen:** Exibe a marca Mileto ao iniciar.
- **DMX:** Canal 1 (Cor) e Canal 2 (Efeito).

## 3. Segurança App Mileto
O App Flutter deve ler a característica BLE para validar o código `MILETO_AUTH_2024_PRO`. Se não encontrar, deve redirecionar para: https://mileto.ind.br/
