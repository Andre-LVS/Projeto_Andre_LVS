#include "janelaprincipal.h"

JanelaPrincipal::JanelaPrincipal(QWidget *parent)
    : QWidget(parent)
{
    //titulo============================================================
    tituloProjeto = new QLabel("<h1> Título do projeto</h1>");
    tituloProjeto->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    //===================================================================

    //Botões inferiores===============================================================

    sairBotao = new QPushButton("Sair");
    QObject::connect(sairBotao, SIGNAL(clicked(bool)), this, SLOT(close()));
    sobreBotao = new QPushButton("Sobre");
    QObject::connect(sobreBotao, SIGNAL(clicked(bool)), this, SLOT(showAbout()));

    botoesLayout= new QHBoxLayout;
    botoesLayout->addWidget(sairBotao);//colocar o botão sair dentro do layout de botões
    botoesLayout->addWidget(sobreBotao);

    //=================================================================================


    mainLayout = new QVBoxLayout;

    mainLayout->addWidget(tituloProjeto);
    mainLayout->addLayout(botoesLayout);

    setLayout(mainLayout);
}

void JanelaPrincipal::showAbout() {
    QMessageBox::about(this,
                       "Sobre o Projeto",
                       "<h2>Monitor de Painel Solar</h2>"
                       "<p>O projeto consiste no desenvolvimento de uma aplicação em C++ capaz de monitorar, em tempo real, "
                       "a tensão gerada por uma placa solar por meio de comunicação serial com um microcontrolador "
                       "(como um Arduino) e gerar um gráfico.</p>"
                       "<p>Lembre-se de reduzir a tensão da placa solar para níveis seguros de leitura pelo microcontrolador, "
                       "como por exmplo um divisor de tensão. O Arduino será responsável por realizar a leitura analógica "
                       "dessa tensão e enviá-la via comunicação serial para o programa em C++.</p>"
                       "<p>A aplicação terá como principal objetivo coletar, processar e visualizar esses dados ao longo do tempo, "
                       "permitindo ao usuário acompanhar o comportamento da geração de energia durante o dia. "
                       "Os dados serão apresentados na forma de um gráfico, onde:</p>"

                           "<p>O eixo X representará o tempo (horário das medições)</p>"
                           "<p>O eixo Y representará a tensão medida</p>"

                                   "<p>O usuário poderá exportar os dados ou o gráfico gerado para análise posterior.</p>"
                       "<p></p>"
                       );}

JanelaPrincipal::~JanelaPrincipal() = default;
