#include "painelcontrole.h"


// ─────────────────────────────────────────────
//  BLOCO: Construtor — monta o painel lateral
// ─────────────────────────────────────────────
PainelControle::PainelControle(QWidget *parent)
    : QWidget{parent}
{
    // --- Sub-bloco: Objeto de leitura serial ---
    mLeitorSerial = new LeitorSerial(this);

    connect(mLeitorSerial, &LeitorSerial::novaTensaoRecebida,
            this,          &PainelControle::novaTensao);       // Repassa ao gráfico

    connect(mLeitorSerial, &LeitorSerial::erroConexao,
            this,          &PainelControle::onErroConexao);    // Exibe erros

    // ── Seção: Tensão máxima ──────────────────
    lblTensaoMax = new QLabel("Tensão máxima da placa (V):");

    spinTensaoMax = new QDoubleSpinBox();
    spinTensaoMax->setRange(0.0, 100.0);    // Faixa permitida
    spinTensaoMax->setDecimals(2);          // Duas casas decimais
    spinTensaoMax->setValue(20.64);         // Valor padrão da placa solar
    spinTensaoMax->setSuffix(" V");         // Unidade exibida ao lado

    connect(spinTensaoMax, &QDoubleSpinBox::valueChanged,
            this,          &PainelControle::onTensaoMaximaAlterada);

    // ── Seção: Porta serial ───────────────────
    lblPorta = new QLabel("Porta serial:");

    comboPortas = new QComboBox();          // Lista de portas do sistema

    btnAtualizar = new QPushButton("Atualizar portas");
    connect(btnAtualizar, &QPushButton::clicked,
            this,         &PainelControle::onBtnAtualizarPortas);

    // ── Seção: Baud rate ──────────────────────
    lblBaudRate = new QLabel("Baud rate:");

    comboBaudRate = new QComboBox();
    // Baud rates mais comuns usados com Arduino
    comboBaudRate->addItems({"9600", "19200", "38400", "57600", "115200"});
    comboBaudRate->setCurrentText("9600"); // Padrão do Arduino

    // ── Seção: Botão conectar ─────────────────
    btnConectar = new QPushButton("Conectar");
    connect(btnConectar, &QPushButton::clicked,
            this,        &PainelControle::onBtnConectar);

    // ── Seção: Status ─────────────────────────
    lblStatus = new QLabel("Desconectado");
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setStyleSheet("color: red; font-weight: bold;"); // Vermelho = desconectado

    // ---Seção: Botão exportar-----------------

    btnExportar = new QPushButton("Exportar");
    connect(btnExportar, &QPushButton::clicked, this, &PainelControle::onBtnExportar);

    // ── Monta o layout vertical ───────────────
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);     // Alinha os widgets ao topo

    layout->addWidget(lblTensaoMax);
    layout->addWidget(spinTensaoMax);
    layout->addSpacing(16);                 // Espaço entre seções

    layout->addWidget(lblPorta);
    layout->addWidget(comboPortas);
    layout->addWidget(btnAtualizar);
    layout->addSpacing(5);

    layout->addWidget(lblBaudRate);
    layout->addWidget(comboBaudRate);
    layout->addSpacing(3);

    layout->addWidget(btnConectar);
    layout->addSpacing(8);
    layout->addWidget(lblStatus);

    layout->addSpacing(35);
    layout->addWidget(btnExportar);


    setLayout(layout);

    onBtnAtualizarPortas(); // Já carrega as portas ao iniciar
}

// ─────────────────────────────────────────────
//  BLOCO: Retorna o valor de tensão máxima
// ─────────────────────────────────────────────
double PainelControle::tensaoMaxima() const
{
    return spinTensaoMax->value(); // Lê o valor atual do spinbox
}

// ─────────────────────────────────────────────
//  BLOCO: Atualiza a lista de portas seriais
// ─────────────────────────────────────────────
void PainelControle::onBtnAtualizarPortas()
{
    comboPortas->clear();                               // Limpa a lista atual
    comboPortas->addItems(LeitorSerial::portasDisponiveis()); // Adiciona as disponíveis

    if (comboPortas->count() == 0)
        comboPortas->addItem("Nenhuma porta encontrada"); // Aviso se não houver portas
}

// ─────────────────────────────────────────────
//  BLOCO: Conecta ou desconecta o Arduino
// ─────────────────────────────────────────────
void PainelControle::onBtnConectar()
{
    // --- Sub-bloco: Se já conectado, desconecta ---
    if (mLeitorSerial->estaConectado()) {
        mLeitorSerial->desconectar();
        btnConectar->setText("Conectar");
        lblStatus->setText("Desconectado");
        lblStatus->setStyleSheet("color: red; font-weight: bold;");
        return;
    }

    // --- Sub-bloco: Tenta conectar ---
    QString porta    = comboPortas->currentText();       // Porta selecionada
    int     baudRate = comboBaudRate->currentText().toInt(); // Baud rate selecionado

    if (mLeitorSerial->conectar(porta, baudRate)) {
        btnConectar->setText("Desconectar");
        lblStatus->setText("Conectado: " + porta);
        lblStatus->setStyleSheet("color: green; font-weight: bold;"); // Verde = conectado
    }
}

// ─────────────────────────────────────────────
//  BLOCO: Exibe erro de conexão
// ─────────────────────────────────────────────
void PainelControle::onErroConexao(const QString &erro)
{
    lblStatus->setText("Erro: " + erro);
    lblStatus->setStyleSheet("color: red; font-weight: bold;");
    QMessageBox::critical(this, "Erro de conexão", erro); // Popup de erro
}

// ─────────────────────────────────────────────
//  BLOCO: Emite o novo valor de tensão máxima
// ─────────────────────────────────────────────
void PainelControle::onTensaoMaximaAlterada()
{
    emit tensaoMaximaAlterada(spinTensaoMax->value()); // Notifica o gráfico
}
// ─────────────────────────────────────────────
//  BLOCO:Exporta imagem do grafico e cvs
// ─────────────────────────────────────────────
void PainelControle::onBtnExportar()
{
emit solicitaExportacao();

        }
