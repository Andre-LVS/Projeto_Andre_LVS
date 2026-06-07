#include "leitorserial.h"

// ─────────────────────────────────────────────
//  BLOCO: Construtor e destrutor
// ─────────────────────────────────────────────
LeitorSerial::LeitorSerial(QObject *parent)
    : QObject{parent}
{
    mPorta = new QSerialPort(this); // Cria o objeto da porta serial

    // Quando chegarem dados, chama onDadosDisponiveis()
    connect(mPorta, &QSerialPort::readyRead,
            this,   &LeitorSerial::onDadosDisponiveis);
}

LeitorSerial::~LeitorSerial()
{
    desconectar(); // Garante que a porta é fechada ao destruir
}

// ─────────────────────────────────────────────
//  BLOCO: Conecta à porta serial do Arduino
// ─────────────────────────────────────────────
bool LeitorSerial::conectar(const QString &porta, int baudRate)
{
    if (mPorta->isOpen())
        mPorta->close(); // Fecha conexão anterior se houver

    mPorta->setPortName(porta);                          // Define a porta (ex: /dev/ttyUSB0)
    mPorta->setBaudRate(baudRate);                       // Define o baud rate (ex: 9600)
    mPorta->setDataBits(QSerialPort::Data8);             // 8 bits de dados (padrão Arduino)
    mPorta->setParity(QSerialPort::NoParity);            // Sem paridade
    mPorta->setStopBits(QSerialPort::OneStop);           // 1 stop bit
    mPorta->setFlowControl(QSerialPort::NoFlowControl);  // Sem controle de fluxo

    if (!mPorta->open(QIODevice::ReadOnly)) {            // Tenta abrir em modo leitura
        emit erroConexao(mPorta->errorString());         // Emite erro se falhar
        return false;
    }

    return true; // Conexão bem sucedida
}

// ─────────────────────────────────────────────
//  BLOCO: Desconecta da porta serial
// ─────────────────────────────────────────────
void LeitorSerial::desconectar()
{
    if (mPorta->isOpen())
        mPorta->close(); // Fecha a porta se estiver aberta
    mBuffer.clear();     // Limpa o buffer de dados
}

// ─────────────────────────────────────────────
//  BLOCO: Retorna se está conectado
// ─────────────────────────────────────────────
bool LeitorSerial::estaConectado() const
{
    return mPorta->isOpen(); // Verdadeiro se a porta estiver aberta
}

// ─────────────────────────────────────────────
//  BLOCO: Lista todas as portas disponíveis
// ─────────────────────────────────────────────
QStringList LeitorSerial::portasDisponiveis()
{
    QStringList lista;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
        lista << info.portName(); // Adiciona o nome de cada porta à lista
    return lista;
}

// ─────────────────────────────────────────────
//  BLOCO: Lê e interpreta dados recebidos
// ─────────────────────────────────────────────
void LeitorSerial::onDadosDisponiveis()
{
    // --- Sub-bloco: Acumula os dados no buffer ---
    mBuffer += QString::fromUtf8(mPorta->readAll()); // Lê tudo disponível e acumula

    // --- Sub-bloco: Processa linhas completas ---
    // O Arduino envia uma linha por vez terminada em '\n'
    while (mBuffer.contains('\n')) {
        int idx = mBuffer.indexOf('\n');            // Acha o fim da linha
        QString linha = mBuffer.left(idx).trimmed(); // Extrai a linha sem espaços
        mBuffer = mBuffer.mid(idx + 1);             // Remove a linha processada do buffer

        // --- Sub-bloco: Converte o texto para double ---
        bool ok = false;
        double tensao = linha.toDouble(&ok); // Tenta converter para número

        if (ok)
            emit novaTensaoRecebida(tensao); // Emite o valor se a conversão funcionou
    }
}
