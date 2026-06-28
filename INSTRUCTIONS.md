# Sistema Profissional OWire LED - ESP32-C3 Super Mini

Este sistema permite alternar entre controle manual e via DMX.

## 1. Como Selecionar a Fonte de Controle
O sistema possui dois modos de operação:

1. **MANUAL:** O controle é feito exclusivamente pelos botões e OLED do painel. Comandos DMX são ignorados.
2. **DMX:** O sistema obedece aos comandos vindos da porta DMX (Canais 1 e 2).

### Para trocar a fonte:
1. Pressione o botão **SELECT** para entrar no menu.
2. O cursor estará em **FONTE**.
3. Pressione o botão **DOWN** para alternar entre MANUAL e DMX.
4. Pressione **BACK** para sair e salvar.

## 2. Pinagem (Hardware)
| Componente | Pino |
| :--- | :--- |
| **LED OWire** | GPIO 10 |
| **Display I2C** | GPIO 8 (SDA) / 9 (SCL) |
| **UP / DOWN** | GPIO 3 / 4 |
| **SELECT / BACK** | GPIO 5 / 6 |
| **DMX RX** | GPIO 20 |

## 3. Comportamento da Memória
Tanto a **Fonte Selecionada** quanto a **Última Cor/Modo** são gravados na memória Flash automaticamente ao serem alterados. Ao reiniciar o dispositivo, ele voltará no mesmo modo de operação anterior.

## 4. Dicas de Uso
- Se estiver no modo **DMX**, os botões do painel ainda permitem navegar no menu, mas o LED será atualizado assim que um novo pacote DMX chegar.
- Use o modo **MANUAL** para travar uma configuração específica e evitar interferências externas.

## 5. Bibliotecas Necessárias
- `Adafruit SSD1306` & `Adafruit GFX`
- `Preferences` (Nativa)
- `SparkFun OWire Arduino Library` (Incluída no repo)
