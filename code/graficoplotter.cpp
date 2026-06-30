#include "graficoplotter.h"
#include <QRandomGenerator> //Só para gerar numeros aleatórios para testar o gráfico
#include <QVBoxLayout>
#include <QToolTip>


// ─────────────────────────────────────────────
//  BLOCO: Subclasse do QChartView para capturar
//         zoom com scroll e arraste com mouse
// ─────────────────────────────────────────────
class ChartViewInterativo : public QChartView
{
public:
    ChartViewInterativo(QChart *chart, QWidget *parent = nullptr)
        : QChartView(chart, parent)
    {
        setRubberBand(QChartView::HorizontalRubberBand);
        setDragMode(QGraphicsView::NoDrag);
    }

    // BLOCO: Corrige os dois eixos para não exibir valores inválidos
    void corrigirLimites()
    {
        // --- Eixo X: impede tempo anterior ao início ---
        QDateTimeAxis *eixoX = qobject_cast<QDateTimeAxis*>(
            chart()->axes(Qt::Horizontal).first());
        if (eixoX && eixoX->min() < mHorarioInicio)
            eixoX->setMin(mHorarioInicio); // Não deixa passar do horário de início

        // --- Eixo Y: impede tensão negativa ---
        QValueAxis *eixoY = qobject_cast<QValueAxis*>(
            chart()->axes(Qt::Vertical).first());
        if (eixoY && eixoY->min() < 0.0)
            eixoY->setMin(0.0); // Tensão não pode ser menor que zero
    }

protected:
    void wheelEvent(QWheelEvent *event) override
    {
        double fator = event->angleDelta().y() > 0 ? 1.15 : 0.85; // cima = zoom in

        // --- Sub-bloco: Pega a área visível atual do gráfico em coordenadas de cena ---
        QRectF areaAtual = chart()->plotArea(); // Retângulo da área do gráfico

        // --- Sub-bloco: Posição do mouse dentro da área do gráfico ---
        QPointF posicaoMouse = event->position(); // Posição do mouse na tela

        // --- Sub-bloco: Calcula a nova área com zoom centrado no mouse ---
        double novaLargura  = areaAtual.width()  / fator; // Nova largura após zoom
        double novaAltura   = areaAtual.height() / fator; // Nova altura após zoom

        // Proporção do mouse dentro da área (0.0 = esquerda/topo, 1.0 = direita/baixo)
        double propX = (posicaoMouse.x() - areaAtual.left()) / areaAtual.width();
        double propY = (posicaoMouse.y() - areaAtual.top())  / areaAtual.height();

        // Novo canto superior esquerdo, mantendo o mouse no mesmo ponto relativo
        double novoX = posicaoMouse.x() - propX * novaLargura;
        double novoY = posicaoMouse.y() - propY * novaAltura;

        QRectF novaArea(novoX, novoY, novaLargura, novaAltura); // Nova área com zoom

        // --- Sub-bloco: Aplica o zoom na nova área e corrige limites ---
        chart()->zoomIn(novaArea);  // Zoom na área calculada
        corrigirLimites();          // Impede tempo negativo e tensão negativa
        event->accept();
    }

    // BLOCO: Início do arraste com botão direito
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::RightButton)
            mUltimoPonto = event->pos();
        QChartView::mousePressEvent(event);
    }

    // BLOCO: Arraste — corrige limites a cada movimento
    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (event->buttons() & Qt::RightButton) {
            QPoint delta = event->pos() - mUltimoPonto;
            chart()->scroll(-delta.x(), delta.y());
            corrigirLimites(); // Verifica os dois eixos após cada passo do arraste
            mUltimoPonto = event->pos();
        }

        //----------
        // --- Sub-bloco: Tooltip com horário e tensão ---
        // Converte posição do mouse para coordenadas do gráfico
        QPointF valorGrafico = chart()->mapToValue(event->pos()); // Coordenada em unidades do gráfico

        // Converte o valor X (ms desde epoch) para QDateTime legível
        QDateTime horario = QDateTime::fromMSecsSinceEpoch(
            static_cast<qint64>(valorGrafico.x()));

        // Monta o texto do tooltip
        QString texto = QString("Horário: %1\nTensão: %2 V")
                            .arg(horario.toString("hh:mm:ss"))      // Formata o horário
                            .arg(valorGrafico.y(), 0, 'f', 2);      // Formata tensão com 2 casas decimais

        // Exibe o tooltip próximo ao cursor
        QToolTip::showText(event->globalPosition().toPoint(), texto, this);
        //----------

        QChartView::mouseMoveEvent(event);
    }

    // BLOCO:zoom por seleção — corrige limites
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QChartView::mouseReleaseEvent(event);
        corrigirLimites();
    }

public:
    QDateTime mHorarioInicio; // Limite mínimo do eixo X

private:
    QPoint mUltimoPonto;
};

graficoPlotter::graficoPlotter(QWidget *parent)
    : QWidget{parent}
{

    setWindowTitle("Monito de tensão"); //define titulo da janela

    //tempoDecorrido = 0;
    //horarioInicio  = QDateTime::currentDateTime(); // Captura horário atual do sistema
    horarioInicio = QDateTime(QDate::currentDate(),QTime(6,0,0));
    tempoDecorrido = horarioInicio.secsTo(QDateTime::currentDateTime());

    serie = new QLineSeries(); //Cria a linha do gráfico
    serie->setName("Tensão(V)"); //nome exibido na legenda

    //---------------------
    grafico = new QChart(); //cria o gráfico
    grafico->addSeries(serie);
    grafico->setTitle("Tensão x Tempo");
    grafico->setAnimationOptions(QChart::NoAnimation);

    //----------------------
    eixoX = new QDateTimeAxis();
    eixoX->setTitleText("Horário)");
    eixoX->setFormat("hh:mm");
    eixoX->setMin(horarioInicio);                       // Começa no horário atual
    eixoX->setMax(horarioInicio.addSecs(JANELA_MAX));   // Termina 24h depois
   // eixoX->setRange(0,JANELA_MAX);//exibe as 24 horas;
    //eixoX->setLabelFormat("%d");// Formato inteiro
    eixoX->setTickCount(13);            // Uma marcação a cada 2h aprox.
    grafico->addAxis(eixoX, Qt::AlignBottom);//Anexa o eixo X no gráfico
    serie->attachAxis(eixoX);

    //---------------------
    eixoY = new QValueAxis();
    eixoY->setTitleText("Tensão(V)");
    eixoY->setRange(0,25); //0 a 25 Volts
    eixoY->setLabelFormat("%.1f");// Formato com 1 casa decimal
    grafico->addAxis(eixoY, Qt::AlignLeft);// Anexa o eixo à lateral do gráfico
    serie->attachAxis(eixoY);//liga a linha ao eixo

    // --- Sub-bloco: View interativo ---
    ChartViewInterativo *viewInterativo = new ChartViewInterativo(grafico, this);
    viewInterativo->setRenderHint(QPainter::Antialiasing);
    viewInterativo->mHorarioInicio = horarioInicio; // Passa o horário para o view
    viewGrafico = viewInterativo;                   // Salva na variável da classe

    // --- Sub-bloco: Botão para resetar o zoom ---
    QPushButton *btnReset = new QPushButton("Resetar zoom");
    connect(btnReset, &QPushButton::clicked,   // Quando clicado...
            grafico,  &QChart::zoomReset);      // ...restaura a visão completa

    //---------------
    // BLOCO: Gerenciador de dados — salva e restaura medições
    mGerenciador = new GerenciadorDados(this);
    carregarDadosAnteriores(); // Carrega os dados salvos anteriormente (se houver)


    //----------------------
    timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &graficoPlotter::atualizarGrafico); //Quando o timer disparar chama a função
    timer->start(1000);//Dispara a cada 1000ms->1s

    //-------------------
    QVBoxLayout *layout = new QVBoxLayout(this); // Cria layout interno do widget
    layout->addWidget(viewGrafico);              // Adiciona o view ao layout
     layout->addWidget(btnReset);
    layout->setContentsMargins(0, 0, 0, 0);     // Remove margens internas
    setLayout(layout);                           // Aplica o layout ao widget
}

/*
//Esta função gera um valor aleatório para tensar o programa.
//Deletar esta função quando implementar o arduino
void graficoPlotter::atualizarGrafico(){

    if (tempoDecorrido > JANELA_MAX) return; // Para de coletar após 24h

    // --- Sub-bloco: Calcula o horário real deste ponto ---
    QDateTime agora = horarioInicio.addSecs(tempoDecorrido); // Horário = início + segundos

    double tensao = 0.0 + QRandomGenerator::global()->generateDouble() * 24.0; //Gera numeros aleatorios para testar o gŕafico. Talvez eu coloque o arduino aqui?
    serie->append(agora.toMSecsSinceEpoch(), tensao); // QDateTimeAxis usa ms desde epoch //insere tempo e tensão na linha do gráfico;
    mGerenciador->salvarPonto({agora, tensao}); // Salva imediatamente no arquivo

   // if(tempoDecorrido>60){
    //    eixoX->setRange(tempoDecorrido-60, tempoDecorrido);
   // }

    tempoDecorrido++;

}*/


// Receber valor da leitura analógica do arduino
// BLOCO: Recebe tensão real do Arduino (substitui o valor randômico)
void graficoPlotter::receberTensao(double tensao)
{
    if (tempoDecorrido > JANELA_MAX) return;

    QDateTime agora = horarioInicio.addSecs(tempoDecorrido);
    serie->append(agora.toMSecsSinceEpoch(), tensao); // Plota o valor real
    mGerenciador->salvarPonto({agora, tensao}); // Salva imediatamente no arquivo
    tempoDecorrido++;
}


// BLOCO: Ajusta o limite máximo do eixo Y conforme tensão máxima da placa
void graficoPlotter::setTensaoMaxima(double maxima)
{
    eixoY->setMax(maxima); // Atualiza o eixo Y com o novo valor máximo

}


void graficoPlotter::carregarDadosAnteriores()
{
    if (!mGerenciador->temDadosDeHoje()) return; // Nada a carregar

    QList<PontoMedicao> pontos = mGerenciador->carregarDados(
        mGerenciador->caminhoArquivo());

    if (pontos.isEmpty()) return;

    // --- Sub-bloco: Insere os pontos antigos  ---
    for (const PontoMedicao &p : pontos) {
        serie->append(p.horario.toMSecsSinceEpoch(), p.tensao); // Plota no gráfico
    }

    // --- Sub-bloco: Ajusta o horário para continuar de onde parou ---

    eixoX->setMin(horarioInicio);                     // Sempre 06:00 do dia atual
    eixoX->setMax(horarioInicio.addSecs(JANELA_MAX)); // Sempre 06:00 do dia seguinte

    // --- Sub-bloco: Atualiza o limite do zoom no view interativo ---
    ChartViewInterativo *view = static_cast<ChartViewInterativo*>(viewGrafico);
    view->mHorarioInicio = horarioInicio;
}


