#ifndef PAINELCONTROLE_H
#define PAINELCONTROLE_H

#include <QWidget>
#include <QtWidgets>
#include "leitorserial.h"
#include "graficoplotter.h"

//--Para exporta o gráfico e salvar em um local--
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>

// ─────────────────────────────────────────────
//  BLOCO: Widget lateral de controle
// ─────────────────────────────────────────────
class PainelControle : public QWidget
{
    Q_OBJECT
public:
    explicit PainelControle(QWidget *parent = nullptr);

    double tensaoMaxima() const; // Retorna o valor máximo configurado pelo usuário

signals:
    void novaTensao(double tensao);       // Repassado do LeitorSerial para o gráfico
    void tensaoMaximaAlterada(double v);  // Emitido quando o usuário muda o valor máximo
     void solicitaExportacao();

private slots:
    void onBtnAtualizarPortas();          // Atualiza a lista de portas disponíveis
    void onBtnConectar();                 // Conecta ou desconecta o Arduino
    void onErroConexao(const QString &e); // Exibe mensagem de erro
    void onTensaoMaximaAlterada();        // Lê e emite o novo valor máximo
    void onBtnExportar(); //exporta um print do gráfico e cvs com os dados

private:
    // BLOCO: Seção de tensão máxima
    QLabel        *lblTensaoMax;
    QDoubleSpinBox *spinTensaoMax;  // Campo numérico para tensão máxima

    // BLOCO: Seção de porta serial
    QLabel        *lblPorta;
    QComboBox     *comboPortas;     // Lista de portas disponíveis
    QPushButton   *btnAtualizar;    // Atualiza a lista de portas

    // BLOCO: Seção de configuração serial
    QLabel        *lblBaudRate;
    QComboBox     *comboBaudRate;   // Seleção do baud rate

    // BLOCO: Botão de conexão
    QPushButton   *btnConectar;

    // BLOCO: Status da conexão
    QLabel        *lblStatus;

    // BLOCO: Objeto de comunicação serial
    LeitorSerial  *mLeitorSerial;

    //Botão exportar
    QPushButton *btnExportar;
};

#endif // PAINELCONTROLE_H
