#ifndef JANELAPRINCIPAL_H
#define JANELAPRINCIPAL_H

#include <QWidget>
#include <QtWidgets>
#include "graficoplotter.h"
#include "painelcontrole.h"

class JanelaPrincipal : public QWidget
{
    Q_OBJECT

public:
    explicit JanelaPrincipal(QWidget *parent = nullptr);
    ~JanelaPrincipal() override;

private:
    QLabel          *tituloProjeto;
    graficoPlotter  *grafico;
    PainelControle  *painel;        // Widget lateral
    QPushButton     *sairBotao;
    QPushButton     *sobreBotao;
    QHBoxLayout     *conteudoLayout; // Layout horizontal (painel + gráfico)
    QVBoxLayout     *mainLayout;
    QHBoxLayout     *botoesLayout;

private slots:
    void showAbout();   // Declara o slot personalizado
};
#endif // JANELAPRINCIPAL_H
