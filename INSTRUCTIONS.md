# Guia de Diagnóstico - OWire LED ESP32-C3

Se nada estiver funcionando, siga estes passos para identificar o problema.

## 1. Nova Pinagem (Segura)
Alteramos os pinos I2C para evitar conflitos com os pinos de boot do ESP32-C3.

| Componente | Pino ESP32-C3 | Notas |
| :--- | :--- | :--- |
| **OLED SDA** | **GPIO 0** | Conecte o SDA do display aqui. |
| **OLED SCL** | **GPIO 1** | Conecte o SCL do display aqui. |
| **LED OWire** | **GPIO 6** | Saída para os LEDs. |
| **DMX RX** | **GPIO 20** | Entrada do sinal RS485. |

## 2. Sequência de Inicialização (O que observar)
Ao ligar o dispositivo, ele executará um auto-teste:
1. **LED Azul Interno:** Irá piscar uma vez. Se não piscar, o ESP32 não está recebendo energia ou o código não foi carregado.
2. **Display OLED:** Deve mostrar "OWIRE PRO OK". Se ficar preto, verifique se os fios SDA/SCL estão nos pinos 0 e 1 e se o display tem alimentação (3.3V e GND).
3. **LEDs OWire:** Irão ciclar rapidamente entre Vermelho, Verde e Azul. Se não mudarem de cor, verifique a fiação no GPIO 6.

## 3. Monitoramento
Após o teste inicial, o display mostrará os valores DMX recebidos nos canais 1 e 2.
- O LED Azul interno continuará piscando a cada 1 segundo para indicar que o sistema está "vivo".

## 4. Estrutura de Pastas
Certifique-se de que o arquivo se chama `OWirePro.ino` e está dentro de uma pasta chamada `OWirePro`.
