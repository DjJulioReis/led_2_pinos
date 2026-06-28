# Sistema Profissional OWire LED - ESP32-C3 Super Mini

Este sistema permite alternar entre controle manual e via DMX.

## 🚀 Como resolver o erro de Compilação
O erro `undefined reference to setup()` e `loop()` acontece quando o Arduino IDE tenta compilar arquivos `.ino` fora de uma pasta com o mesmo nome ou quando há vários arquivos na raiz.

**Como usar os arquivos deste repositório:**
1. Para o sistema completo, abra a pasta `OWirePro` e clique em `OWirePro.ino`.
2. Para um teste simples, abra a pasta `exemplo_basico` e clique em `exemplo_basico.ino`.

## 1. Como Selecionar a Fonte de Controle (No OWirePro)
O sistema possui dois modos de operação:

1. **MANUAL:** O controle é feito pelos botões e OLED.
2. **DMX:** O sistema obedece aos comandos vindos da porta DMX (Canais 1 e 2).

### Para trocar a fonte:
1. Pressione **SELECT** para entrar no menu.
2. Com o cursor em **FONTE**, pressione **DOWN** para alternar entre MANUAL e DMX.
3. Pressione **BACK** para sair e salvar.

## 2. Pinagem (Hardware)
| Componente | Pino |
| :--- | :--- |
| **LED OWire** | GPIO 10 |
| **Display I2C** | GPIO 8 (SDA) / 9 (SCL) |
| **UP / DOWN** | GPIO 3 / 4 |
| **SELECT / BACK** | GPIO 5 / 6 |
| **DMX RX** | GPIO 20 |

## 3. Bibliotecas Necessárias
- `Adafruit SSD1306` & `Adafruit GFX`
- `Preferences` (Nativa)
- `SparkFun OWire Arduino Library` (Incluída no repo)
