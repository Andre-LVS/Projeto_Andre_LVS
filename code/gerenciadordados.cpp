#include "gerenciadordados.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>

// ─────────────────────────────────────────────
//  BLOCO: Construtor — define o arquivo padrão
// ─────────────────────────────────────────────
GerenciadorDados::GerenciadorDados(QObject *parent)
    : QObject{parent}
{
    // --- Sub-bloco: Pega a pasta onde o executável está rodando ---
    QString pasta = QCoreApplication::applicationDirPath(); // Caminho do executável

    // --- Sub-bloco: Cria subpasta "dados" dentro da pasta do programa ---
    pasta += "/dados";
    QDir().mkpath(pasta); // Cria a pasta se não existir

    // --- Sub-bloco: Nome do arquivo inclui a data ---
    QString dataHoje = QDate::currentDate().toString("yyyy-MM-dd");
    mCaminhoArquivo  = pasta + "/medicoes_" + dataHoje + ".csv";
}

// ─────────────────────────────────────────────
//  BLOCO: Salva um único ponto no CSV
// ─────────────────────────────────────────────
bool GerenciadorDados::salvarPonto(const PontoMedicao &ponto)
{
    QFile arquivo(mCaminhoArquivo);

    // Abre em modo append (acrescenta ao final sem apagar o que já existe)
    if (!arquivo.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream stream(&arquivo);

    // --- Sub-bloco: Escreve uma linha no formato: horario,tensao ---
    stream << ponto.horario.toString("yyyy-MM-dd hh:mm:ss") // Horário completo
           << ","
           << QString::number(ponto.tensao, 'f', 4) // Tensão com 4 casas decimais
           << "\n";

    arquivo.close();
    return true;
}

// ─────────────────────────────────────────────
//  BLOCO: Carrega todos os pontos do CSV
// ─────────────────────────────────────────────
QList<PontoMedicao> GerenciadorDados::carregarDados(const QString &caminhoArquivo)
{
    QList<PontoMedicao> lista; // Lista de pontos que será retornada

    QFile arquivo(caminhoArquivo);
    if (!arquivo.exists() || !arquivo.open(QIODevice::ReadOnly | QIODevice::Text))
        return lista; // Retorna lista vazia se arquivo não existir

    QTextStream stream(&arquivo);

    while (!stream.atEnd()) {
        QString linha = stream.readLine().trimmed(); // Lê uma linha por vez
        if (linha.isEmpty()) continue;               // Ignora linhas vazias

        QStringList partes = linha.split(',');        // Divide pelo separador
        if (partes.size() != 2) continue;            // Ignora linhas malformadas

        // --- Sub-bloco: Converte os campos ---
        QDateTime horario = QDateTime::fromString(
            partes[0].trimmed(),
            "yyyy-MM-dd hh:mm:ss"); // Lê o horário

        bool ok = false;
        double tensao = partes[1].trimmed().toDouble(&ok); // Lê a tensão

        if (!horario.isValid() || !ok) continue; // Ignora linhas com dados inválidos

        lista.append({horario, tensao}); // Adiciona o ponto à lista
    }

    arquivo.close();
    return lista;
}

// ─────────────────────────────────────────────
//  BLOCO: Getters e setters
// ─────────────────────────────────────────────
void GerenciadorDados::setCaminhoArquivo(const QString &caminho)
{
    mCaminhoArquivo = caminho; // Permite sobrescrever o caminho padrão
}

QString GerenciadorDados::caminhoArquivo() const
{
    return mCaminhoArquivo; // Retorna o caminho atual
}

// ─────────────────────────────────────────────
//  BLOCO: Verifica se o arquivo de hoje existe
// ─────────────────────────────────────────────
bool GerenciadorDados::temDadosDeHoje() const
{
    return QFile::exists(mCaminhoArquivo); // Verdadeiro se o CSV de hoje já existe
}
