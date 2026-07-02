# Sistema Profissional Mileto Indústria - OWire LED

Este firmware é exclusivo para produtos da **Mileto Indústria**. Ele utiliza um encoder rotativo para navegação e possui um sistema de segurança via Bluetooth para validar a autenticidade do hardware no App Flutter.

## 1. Pinagem (Hardware)

| Componente | Pino ESP32-C3 | Notas |
| :--- | :--- | :--- |
| **LED OWire** | GPIO 6 | Saída de dados para os LEDs. |
| **OLED SDA** | GPIO 8 | Linha de dados I2C. |
| **OLED SCL** | GPIO 9 | Linha de clock I2C. |
| **Encoder CLK** | GPIO 2 | Conectar ao pino CLK do encoder. |
| **Encoder DT** | GPIO 3 | Conectar ao pino DT do encoder. |
| **Encoder SW** | GPIO 4 | Botão do encoder (Clique para OK). |
| **DMX RX** | GPIO 20 | Entrada DMX (RS485). |

## 2. Interface do Usuário (Menu Mileto)
- **Giro do Encoder:** Navega entre as opções (Cor, Modo, Fonte).
- **Clique no Encoder:** Entra ou sai do modo de configuração.
- **Splash Screen:** Exibe a marca "Mileto" ao ligar.

## 3. Segurança e App Flutter
O App Flutter deve validar o dispositivo antes de permitir o controle.
- **UUID de Autenticação:** `d21b0f5b-0c9f-4d3b-9c5c-7d9d9f5c4d2d`
- **Código de Segurança esperado:** `MILETO_AUTH_2024_PRO`

Se o App não encontrar este UUID ou o código estiver incorreto, ele deve exibir a mensagem:
> "Nenhum produto Mileto foi encontrado. Deseja acessar nosso site? https://mileto.ind.br/"

## 4. Controle DMX
O sistema escuta nos canais 1 e 2. O display OLED atua como um monitor em tempo real, exibindo os valores recebidos da mesa DMX.

## 5. Bibliotecas Necessárias
- `Adafruit SSD1306` & `Adafruit GFX`
- `Preferences` (Nativa)
- `BLEDevice` (Nativa)
- `SparkFun OWire Arduino Library` (Incluída no repo)
