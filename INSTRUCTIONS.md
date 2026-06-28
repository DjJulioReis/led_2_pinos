# Sistema de Controle OWire LED - DMX Final

Ajustamos o código para garantir que o **Canal 1** da sua mesa DMX corresponda ao **Canal 1** no código.

## 1. Mapeamento DMX (Corrigido)
- **Canal 1 (Mesa):** Controla a COR do LED.
- **Canal 2 (Mesa):** Controla o EFEITO do LED.

O display OLED agora mostra "CH1" e "CH2" para facilitar a conferência com a sua mesa.

## 2. Nomes das Variáveis
A biblioteca está em inglês, então usamos o padrão `myLed` no código para evitar confusão de "tradução".

## 3. Pinagem (ESP32-C3 Super Mini)
| Componente | Pino |
| :--- | :--- |
| **LED OWire** | GPIO 6 |
| **OLED SDA** | GPIO 0 |
| **OLED SCL** | GPIO 1 |
| **DMX RX** | GPIO 20 |

## 4. Estrutura de Pastas
Para compilar: Pasta `OWirePro` contendo o arquivo `OWirePro.ino`.

## 5. Dica Técnica
Se o Canal 1 da mesa continuar mudando o Canal 2 no display, verifique se a sua mesa DMX não está com um "offset" de endereçamento ou se o dispositivo não está configurado para iniciar em outro canal. O código atual assume que o dispositivo é o **Endereço 1**.
