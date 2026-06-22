# Implementação

>[!NOTE] 
 Relatar o processo de implementação do problemas, incluindo as
 ferramentas e bibliotecas utilizadas
>
# Log 01:
Implementou-se um protótipo da janela com alguns widgets para uma primeira visualização.
À esquerda localiza-se o painel de controle. Acima, uma barra permite que o operador defina a tensão máxima da placa solar. O valor definido deve alterar a ordenada do gráfico em tempo real. Ainda à esquerda, foram inseridos os botões e comandos/dados necessários para a conexão com o Arduino (ainda não testada).

À direita, há um gráfico que se atualiza dinamicamente. No eixo vertical está a tensão, e no horizontal, o tempo. Esse gráfico emprega o horário do sistema para iniciar a medição. Atualmente, o valor de tensão é gerado aleatoriamente para fins de teste, sem qualquer vínculo com a leitura do Arduino. O gráfico realiza a coleta ao longo de 24 horas, sendo assim necessária a funcionalidade de aplicar zoom in/out para que o usuário faça a observação com a precisão desejada. Acrescentou-se um botão para restaurar a visualização padrão.

Na janela principal, na parte inferior, adicionaram-se dois botões. Um para encerrar o programa (Sair) e outro que descreve o propósito do programa (Sobre). 

## bibliotecas utilizadas:

| Módulo | Para que serve no projeto |
|---|---|
| `Qt::Core` | Base do Qt — `QTimer`, `QDateTime`, `QFile`, `QDir`, `QString`, `QList` |
| `Qt::Widgets` | Interface gráfica — `QWidget`, `QPushButton`, `QLabel`, `QComboBox`, `QDoubleSpinBox`, `QMessageBox`, layouts |
| `Qt::Charts` | Gráfico — `QChart`, `QChartView`, `QLineSeries`, `QDateTimeAxis`, `QValueAxis` |
| `Qt::SerialPort` | Comunicação serial com o Arduino — `QSerialPort`, `QSerialPortInfo` | 


![Protótipo 01](img/dev_1_prototipo1.png)

# Log 02:
Implementou-se a função que faz a leitura serial do arduino.  
Com o código abaixo, o arduino envia a leitura da tensão analógica para a saída serial que será lido pelo software.
>[!NOTE]   
void setup() {
    Serial.begin(9600);
}  
void loop() {  
    int leitura = analogRead(A0);               // Lê o pino analógico (0-1023)  
    float tensao = leitura * (12.5 / 1023.0);  // Converte para tensão   
    Serial.println(tensao);                     
    delay(1000);                                
}  
>
Além disto, efetuou-se os teste com os botões para a conexão e comunicação com o arduino, que agora está funcionando. O texto fica verde para indicar que a conexão foi efetuada com sucesso.  
Como "setup" de teste, utilizou-se uma fonte de 12V, um conversor buck, protoboard e 3 resistores para se fazer o divisor resistivo para não queimar aa porta analógica do arduino.
![Setup_arduino](img/Buck_arduino_protoboard.jpg)

Para definir os valores dos resistores deve-se ter em mente que o valor máximo da leitura analógica do arduino é 5V. Então basta fazer um ralação de modo que 5V represente o valor máximo de tensão que será fornecida pela placa solar.
![Setup_arduino](img/divisor_resistivo_R1.png)
Foi realizado um teste para ver se o gráfico plotava o valor entregado pelo conversor buck.
![Teste_arduino](img/Teste_arduino.png)
<div align="center">



[Retroceder](projeto.md) | [Avançar](testes.md)

</div>