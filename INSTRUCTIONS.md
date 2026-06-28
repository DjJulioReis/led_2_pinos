# Monitor de Controle OWire LED - DMX

Este sistema funciona exclusivamente via DMX, usando o display OLED como monitor.

## 1. Mapeamento DMX
- **Canal 1 (COR):** 0-255 (Mapeia para as 12 cores).
- **Canal 2 (EFEITO):** 0-255 (Mapeia para os 8 modos).

## 2. Pinagem (Padrão ESP32-C3 Super Mini)
| Componente | Pino |
| :--- | :--- |
| **LED OWire** | GPIO 6 |
| **OLED SDA** | **GPIO 8** |
| **OLED SCL** | **GPIO 9** |
| **DMX RX** | GPIO 20 |

## 3. Estrutura de Pastas
Para compilar no Arduino IDE:
1. Crie uma pasta chamada `OWirePro`.
2. Coloque o arquivo `OWirePro.ino` dentro dela.

## 4. Recomendações Técnicas
- O **GND** do controlador DMX deve estar unido ao **GND** do ESP32.
- Use um transceptor RS485 para converter o sinal DMX para o GPIO 20.
