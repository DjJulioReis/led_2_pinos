# Sistema Mileto Indústria - OWire LED

Este repositório contém diferentes versões de controle para LEDs OWire de 2 pinos.

## 1. Versão Simples (Auto-Ciclo)
Ideal para testar se os LEDs estão funcionando e se a fiação está correta.
- **Pasta:** `exemplo_simples`
- **Funcionamento:** O LED troca de cor automaticamente a cada 3 segundos. Não requer botões ou sensores.
- **Pino de Saída:** GPIO 32.

## 2. Versão Profissional (DMX + Encoder + OLED)
Sistema completo da Mileto Indústria com interface visual e controle via mesa de luz.
- **Pasta:** `OWirePro`
- **Recursos:** Encoder rotativo, DMX Canais 1 e 2, Splash Screen Mileto e Segurança BLE.

## 3. Pinagem Recomendada (Dev Kit V1)
| Componente | Pino |
| :--- | :--- |
| **LED OWire** | GPIO 32 |
| **DMX RX** | GPIO 16 |
| **Encoder** | GPIO 13, 14, 27 |
| **OLED** | GPIO 21, 22 |

## 4. Estrutura de Pastas
Para evitar erros de compilação no Arduino IDE:
1. O arquivo `.ino` deve estar dentro de uma pasta com o **mesmo nome** (Ex: `exemplo_simples/exemplo_simples.ino`).
