# Sistema Mileto Indústria - OWire LED (Correção)

Atualizamos os nomes das funções e instâncias para garantir compatibilidade total com a biblioteca SparkFun OWire.

## ⚠️ Nome da Instância: myLED
A biblioteca espera o uso do nome `myLED` conforme os exemplos oficiais. Todos os arquivos foram atualizados para este padrão.

## 1. Pinagem Final (Dev Kit V1)
| Componente | Pino |
| :--- | :--- |
| **LED OWire** | **GPIO 32** |
| **DMX RX** | GPIO 16 |
| **Encoder CLK**| GPIO 13 |
| **Encoder DT** | GPIO 14 |
| **Encoder SW** | GPIO 27 |
| **OLED SDA** | GPIO 21 |
| **OLED SCL** | GPIO 22 |

## 2. Estrutura de Pastas
Para evitar erros de compilação:
- Sistema Completo: `OWirePro/OWirePro.ino`
- Teste Auto-Ciclo: `exemplo_simples/exemplo_simples.ino`

## 3. Dica de Funcionamento
Se os LEDs continuarem sem mudar de cor:
1. Verifique se o **GND** está bem conectado.
2. Tente alimentar o LED com **5V** e o sinal via **MOSFET**, pois o pino do ESP32 (3.3V) pode não ter força suficiente para o protocolo OWire.
