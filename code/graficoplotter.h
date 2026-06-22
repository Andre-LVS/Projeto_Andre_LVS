#ifndef GRAFICOPLOTTER_H
#define GRAFICOPLOTTER_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QtCharts/QtCharts>
#include "gerenciadordados.h"

class graficoPlotter : public QWidget
{
    Q_OBJECT
public:
    explicit graficoPlotter(QWidget *parent = nullptr);
    //arduino
    void receberTensao(double tensao);      // Recebe dado real do Arduino
    void setTensaoMaxima(double maxima);    // Ajusta o eixo Y

private slots:
    //void atualizarGrafico();


private:
    QTimer      *timer; // Timer que dispara a atualização do gráfico
    QLineSeries *serie; //linha do gráfico
    QChart      *grafico; // Objeto do gráfico
    QChartView  *viewGrafico; // Widget que exibe o gráfico na janela
    QDateTimeAxis  *eixoX;
    QValueAxis  *eixoY;

    QDateTime horarioInicio;     // Horário real do sistema ao iniciar
    int tempoDecorrido;
    static const int JANELA_MAX = 86400; // 24 horas em segundos

    GerenciadorDados *mGerenciador; // Salva e carrega os dados
    void carregarDadosAnteriores(); // Restaura pontos salvos ao iniciar

signals:
};

#endif // GRAFICOPLOTTER_H
