# Monitor de Controle OWire LED - DMX

Este sistema funciona exclusivamente via DMX, usando o display OLED como monitor.

## 1. Mapeamento DMX
- **Canal 1 (COR):** 0-255 (Mapeia para as 12 cores disponíveis).
- **Canal 2 (EFEITO):** 0-255 (Mapeia para os 8 modos disponíveis).

O OLED exibirá o valor bruto recebido em cada canal e o nome da cor/efeito selecionado.

## 2. Pinagem (ESP32-C3 Super Mini)
| Componente | Pino |
| :--- | :--- |
| **LED OWire** | GPIO 6 |
| **OLED SDA** | GPIO 8 |
| **OLED SCL** | GPIO 9 |
| **DMX RX** | GPIO 20 |

## 3. Estrutura de Pastas
Para compilar no Arduino IDE:
1. Crie uma pasta chamada `OWirePro`.
2. Coloque o arquivo `OWirePro.ino` dentro dela.

## 4. Recomendações Técnicas
Como este sistema é alimentado por DMX, garanta que:
- O **GND** do controlador DMX esteja unido ao **GND** do ESP32.
- Você esteja usando um transceptor RS485 (ex: MAX485) para converter o sinal DMX para o GPIO 20.
