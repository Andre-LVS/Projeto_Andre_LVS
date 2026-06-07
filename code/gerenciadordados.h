#ifndef GERENCIADORDADOS_H
#define GERENCIADORDADOS_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QPair>

// ─────────────────────────────────────────────
//  BLOCO: Estrutura de um ponto de medição
// ─────────────────────────────────────────────
struct PontoMedicao {
    QDateTime horario; // Horário real da medição
    double    tensao;  // Tensão medida naquele instante
};

// ─────────────────────────────────────────────
//  BLOCO: Classe responsável por salvar e
//         carregar os dados em arquivo CSV
// ─────────────────────────────────────────────
class GerenciadorDados : public QObject
{
    Q_OBJECT
public:
    explicit GerenciadorDados(QObject *parent = nullptr);

    // BLOCO: Operações de arquivo
    bool salvarPonto(const PontoMedicao &ponto);        // Acrescenta um ponto no CSV
    QList<PontoMedicao> carregarDados(const QString &caminhoArquivo); // Lê o CSV

    // BLOCO: Configuração do arquivo
    void setCaminhoArquivo(const QString &caminho); // Define onde salvar
    QString caminhoArquivo() const;                 // Retorna o caminho atual

    // BLOCO: Verifica se há dados do dia de hoje no arquivo
    bool temDadosDeHoje() const;

private:
    QString mCaminhoArquivo; // Caminho completo do arquivo CSV
};

#endif // GERENCIADORDADOS_H
