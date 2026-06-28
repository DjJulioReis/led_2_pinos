# Sistema Profissional OWire LED - ESP32-C3 Super Mini

Este sistema avançado permite controle total de LEDs RGB de 2 pinos via App, DMX, e Painel Físico.

## 1. Esquema de Hardware (Pinagem Atualizada)
*Evite usar GPIO 0, 2 ou 8 para botões para não interferir no boot do ESP32.*

| Componente | Pino ESP32-C3 | Função |
| :--- | :--- | :--- |
| **LED OWire** | GPIO 10 | Saída de Dados (Requer MOSFET P-Channel). |
| **Display OLED SDA** | GPIO 8 | Dados I2C. |
| **Display OLED SCL** | GPIO 9 | Clock I2C. |
| **Botão UP** | GPIO 3 | Navegar menu / Trocar valor. |
| **Botão DOWN** | GPIO 4 | Navegar menu / Trocar valor. |
| **Botão SELECT** | GPIO 5 | Entrar no Menu / Confirmar. |
| **Botão BACK** | GPIO 6 | Sair do Menu / Voltar. |
| **DMX RX** | GPIO 20 | Entrada DMX (Conectar ao RO do MAX485). |

## 2. Operação do Sistema

### Menu Físico
- **Tela Inicial:** Exibe a cor e o modo atuais por nome.
- **Botão SELECT:** Entra no modo de edição.
- **Botões UP/DOWN:** Alternam entre Cor e Modo, ou mudam os valores quando selecionados.
- **Botão BACK:** Retorna à tela principal.

### Controle via App (Bluetooth)
- Conecte ao dispositivo `ESP32-OWire-Pro`.
- Envie um array de bytes `[Modo, Cor]` para atualização instantânea.
- O sistema salvará automaticamente na memória flash.

### DMX512
- O sistema escuta no endereço inicial (default: 1).
- **Canal 1:** Cor (mapeado de 0-255 para as 12 cores disponíveis).
- **Canal 2:** Modo (mapeado de 0-255 para os 8 modos disponíveis).

## 3. Integridade de Sinal e "Toque"
Se o sistema apresentar instabilidade ou exigir o toque do dedo para iniciar:
1. **Use um MOSFET P-Channel** conforme recomendado no primeiro guia.
2. Certifique-se de que o **GND** do ESP32 e o **GND** da fonte dos LEDs estejam unidos.
3. Adicione um capacitor de 100uF na entrada de alimentação dos LEDs.

## 4. Bibliotecas Necessárias
- `Adafruit SSD1306` & `Adafruit GFX`
- `Preferences` (Nativa)
- `SparkFun OWire Arduino Library` (Incluída no repo)
