#ifndef JANELAPRINCIPAL_H
#define JANELAPRINCIPAL_H

#include <QWidget>
#include <QtWidgets>

class JanelaPrincipal : public QWidget
{
    Q_OBJECT

public:
    explicit JanelaPrincipal(QWidget *parent = nullptr);
    ~JanelaPrincipal() override;

private:
    QLabel          *tituloProjeto;
    QPushButton     *sairBotao;
    QPushButton     *sobreBotao;
    QVBoxLayout     *mainLayout;
    QHBoxLayout     *botoesLayout;

private slots:
    void showAbout();   // Declara o slot personalizado
};
#endif // JANELAPRINCIPAL_H
