# Sistema de Controle OWire LED - DMX (CH 1, 2)

Este sistema foca no controle via DMX, usando os dois primeiros canais.

## 1. Mapeamento DMX
O dispositivo escuta nos canais 1 e 2 por padrão:

- **Canal 1 (COR):**
  - 0-255: Mapeia para as 12 cores (Vermelho, Verde, Azul, etc).
- **Canal 2 (EFEITO):**
  - 0-255: Mapeia para os 8 modos (Sólido, Fade, Sparkle, etc).

## 2. Operação Local
- **Botão SELECT:** Alterna entre modo **DMX** e modo **MANUAL**.
- **Modo MANUAL:**
  - Use o botão **UP** para mudar a cor.
  - Use o botão **DOWN** para mudar o efeito.

## 3. Pinagem (ESP32-C3 Super Mini)
| Componente | Pino |
| :--- | :--- |
| **LED OWire** | GPIO 10 |
| **OLED I2C** | GPIO 8 (SDA) / 9 (SCL) |
| **Botões** | GPIO 3, 4, 5, 6 |
| **DMX RX** | GPIO 20 |

## 4. Estrutura de Pastas
Para compilar sem erros no Arduino IDE, mantenha o arquivo `OWirePro.ino` dentro da pasta chamada `OWirePro`.
