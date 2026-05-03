# Análise orientada a objeto
> [!NOTE]
> A **análise** orientada a objeto consiste na descrição do problema a ser tratado, duas primeiras etapas da tabela abaixo, a definição de casos de uso e a definição do domínio do problema.

## Descrição Geral do Domínio do Problema

O projeto consiste no desenvolvimento de uma aplicação em C++ capaz de monitorar, em tempo real, a tensão gerada por uma placa solar por meio de comunicação serial com um microcontrolador (como um Arduino).

A tensão original da placa solar (aproximadamente 20,64V) será reduzida para níveis seguros de leitura pelo microcontrolador utilizando um divisor de tensão. O Arduino será responsável por realizar a leitura analógica dessa tensão e enviá-la via comunicação serial para o programa em C++.

A aplicação terá como principal objetivo coletar, processar e visualizar esses dados ao longo do tempo, permitindo ao usuário acompanhar o comportamento da geração de energia durante o dia. Os dados serão apresentados na forma de um gráfico, onde:

- O eixo **X** representará o tempo (horário das medições)
- O eixo **Y** representará a tensão medida

Além da visualização gráfica em tempo real, o sistema também fornecerá informações relevantes como:
- Tensão máxima registrada
- Tensão mínima registrada

O usuário poderá ainda exportar os dados ou o gráfico gerado para análise posterior.

---

# Requisitos Funcionais

- Realizar comunicação serial com o microcontrolador
- Receber dados de tensão em tempo real
- Converter e interpretar os dados recebidos
- Plotar gráfico da tensão ao longo do tempo
- Atualizar o gráfico dinamicamente
- Registrar tensão máxima e mínima
- Exibir informações de horário associadas às medições
- Permitir exportação dos dados coletados (ex: CSV)
- Permitir exportação do gráfico (ex: imagem)

---

# Requisitos Não-Funcionais

- Interface simples e de fácil utilização
- Baixo consumo de recursos computacionais
- Atualização em tempo real com baixa latência
- Código modular e bem estruturado
- Compatibilidade com sistemas operacionais comuns (Linux/Windows)
- Confiabilidade na leitura e armazenamento dos dados
- Precisão na representação gráfica

---

# Possíveis Extensões Futuras

- Monitoramento de corrente e cálculo de potência
- Armazenamento em banco de dados
- Interface gráfica mais avançada (GUI)
- Acesso remoto aos dados (via rede)
- Integração com sistemas de IoT

## Diagrama de Casos de Uso

Apresentar o diagram de casos de uso, identificando as funcionalidades do sistema assim como os atores envolvidos
 
## Diagrama de Domínio do problema

Elaborar um diagrama conceitual do domínio do problema.


<div align="center">

[Retroceder](README.md) | [Avançar](projeto.md)

</div>
