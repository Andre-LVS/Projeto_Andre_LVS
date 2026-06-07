#ifndef LEITORSERIAL_H
#define LEITORSERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

// ─────────────────────────────────────────────
//  BLOCO: Classe responsável pela comunicação
//         serial com o Arduino
// ─────────────────────────────────────────────
class LeitorSerial : public QObject
{
    Q_OBJECT
public:
    explicit LeitorSerial(QObject *parent = nullptr);
    ~LeitorSerial();

    // BLOCO: Métodos públicos de controle
    bool conectar(const QString &porta, int baudRate); // Abre a porta serial
    void desconectar();                                // Fecha a porta serial
    bool estaConectado() const;                        // Retorna estado da conexão

    // BLOCO: Utilitário estático
    static QStringList portasDisponiveis(); // Lista as portas detectadas no sistema

signals:
    // BLOCO: Sinais emitidos para outros widgets
    void novaTensaoRecebida(double tensao); // Emitido quando chega um valor válido
    void erroConexao(const QString &erro);  // Emitido quando ocorre erro

private slots:
    void onDadosDisponiveis(); // Chamado quando chegam dados na porta serial

private:
    QSerialPort *mPorta;       // Objeto da porta serial
    QString      mBuffer;      // Buffer para acumular dados incompletos
};

#endif // LEITORSERIAL_H
