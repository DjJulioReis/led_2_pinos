# Guia de Controle de LEDs RGB de 2 Pinos com ESP32-C3 Super Mini

Este projeto permite controlar LEDs RGB de 2 pinos (OWire) de forma independente por cor e efeito.

## ⚠️ Solução de Problemas: O "Toque" de Start
Se o seu LED só começa a funcionar quando você toca no fio/circuito, isso indica um **problema de integridade de sinal ou falta de corrente**.
- O ESP32-C3 fornece apenas 3.3V e baixa corrente nos pinos.
- O sinal OWire precisa de transições limpas de tensão. Seu corpo está agindo como uma capacitância/aterramento que "limpa" o ruído ou ajuda no pulso inicial.
- **Solução:** Você **precisa** usar um MOSFET (P-Channel) ou ao menos um transistor para chavear a alimentação dos LEDs de forma robusta. Veja a seção de hardware.

## 1. Controle Independente
Os LEDs OWire em uma mesma fita recebem todos o mesmo comando (não são endereçáveis individualmente como os WS2812B).
Para ter controle independente de diferentes conjuntos de LEDs:
1. Conecte cada fita a um pino diferente (ex: GPIO 2 e GPIO 3).
2. Crie múltiplas instâncias no código: `OWIRE fita1; OWIRE fita2;`.

## 2. Configuração do Hardware (Recomendada para Estabilidade)

### Uso de MOSFET P-Channel (Essencial para evitar o erro do "toque")
1. **Fonte 5V (+) -> MOSFET Source**.
2. **MOSFET Drain -> Anodo dos LEDs (+)**.
3. **ESP32 GPIO -> Resistor 1k -> MOSFET Gate**.
4. **Resistor de Pull-up (10k)** entre o **Gate** e o **Source** (ajuda a manter o sinal limpo).
5. **GND comum** para tudo.

No código, se usar MOSFET P-Channel, use: `fita.begin(PINO, true);`.

## 3. Comandos via Serial
O código atual permite enviar letras pelo Monitor Serial (115200 baud) para trocar cores e efeitos de forma independente:

| Comando | Ação (Fita 1) | Comando | Ação (Fita 2) |
| :--- | :--- | :--- | :--- |
| **r** | Vermelho | **R** | Vermelho |
| **g** | Verde | **G** | Verde |
| **b** | Azul | **B** | Azul |
| **1** | Modo Sólido | **!** | Modo Sólido |
| **2** | Modo Fade | **@** | Modo Fade |

## 4. Instalação da Biblioteca
1. Baixe/Copie a pasta `SparkFun_OWire_Arduino_Library-main` para sua pasta `libraries` do Arduino.
2. Reinicie o Arduino IDE.
