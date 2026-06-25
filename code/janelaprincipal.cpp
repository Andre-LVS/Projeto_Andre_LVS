#include "janelaprincipal.h"

JanelaPrincipal::JanelaPrincipal(QWidget *parent)
    : QWidget(parent)
{
    //titulo============================================================
    tituloProjeto = new QLabel("<h1> Monitor de Painel Solar </h1>");
    tituloProjeto->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    //Gráfico===================================================================
    grafico = new graficoPlotter(this);
    grafico->setMinimumHeight(400);

    //Painel Lateral + Arduino======================================================
    painel  = new PainelControle(this);

    // Conecta tensão recebida do Arduino ao gráfico
    connect(painel,  &PainelControle::novaTensao,
            grafico, &graficoPlotter::receberTensao);

    // Conecta mudança de tensão máxima ao gráfico
    connect(painel,  &PainelControle::tensaoMaximaAlterada,
            grafico, &graficoPlotter::setTensaoMaxima);


    //Botões inferiores===============================================================

    sairBotao = new QPushButton("Sair");
    QObject::connect(sairBotao, SIGNAL(clicked(bool)), this, SLOT(close()));
    sobreBotao = new QPushButton("Sobre");
    QObject::connect(sobreBotao, SIGNAL(clicked(bool)), this, SLOT(showAbout()));

    botoesLayout= new QHBoxLayout;
    botoesLayout->addWidget(sairBotao);//colocar o botão sair dentro do layout de botões
    botoesLayout->addWidget(sobreBotao);

    //=================================================================================

    //-----------------exportação grafico--------------------


    connect(painel, &PainelControle::solicitaExportacao, this, [this]() {

        // Abre a janela de salvamento usando a JanelaPrincipal (this) como pai
        QString filePath = QFileDialog::getSaveFileName(this, "Save Graph", "grafico.png", "PNG Images (*.png);;JPEG Images (*.jpg)");

        if (!filePath.isEmpty() && grafico) {
            // Chama o método público que criamos na classe do gráfico
            QPixmap pixmap = grafico->exportarComoImagem();

            if (pixmap.save(filePath)) {
                QMessageBox::information(this, "Sucesso", "Gráfico exportado com sucesso!");
            } else {
                QMessageBox::warning(this, "Erro", "Falha ao exportar o gráfico.");
            }
        }
    });



    //-------------------------------------

    // BLOCO: Layout horizontal — painel ocupa 1/3, gráfico 2/3
    conteudoLayout = new QHBoxLayout();
    conteudoLayout->addWidget(painel,  0); // stretch 1 = 1/3
    conteudoLayout->addWidget(grafico, 1); // stretch 2 = 2/3


    mainLayout = new QVBoxLayout;

    mainLayout->addWidget(tituloProjeto);
    //mainLayout->addWidget(grafico);
    mainLayout->addLayout(conteudoLayout);
    mainLayout->addLayout(botoesLayout);


    setMinimumSize(900, 600);
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
