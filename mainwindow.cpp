#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvaswidget.h"
#include "circuito.h"
#include <QSize>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include "sobredialog.h"
#include "ajudadialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/images/logo.png"));

    const QSize TAMANHO_ICONE(42, 42);

    m_grupoFerramentas = new QButtonGroup(this);
    m_grupoFerramentas->setExclusive(true);

    ui->btnSelecao->setIcon(QIcon(":/images/images/selecao.png"));
    ui->btnSelecao->setIconSize(TAMANHO_ICONE);
    ui->btnSelecao->setToolTip("Seleção (S)");
    ui->btnSelecao->setShortcut(QKeySequence(Qt::Key_S));
    ui->btnSelecao->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnSelecao, static_cast<int>(FerramentaAtiva::SELECAO));

    ui->btnLixeira->setIcon(QIcon(":/images/images/remover.png"));
    ui->btnLixeira->setIconSize(TAMANHO_ICONE);
    ui->btnLixeira->setToolTip("Remover (R)");
    ui->btnLixeira->setShortcut(QKeySequence(Qt::Key_R));
    ui->btnLixeira->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnLixeira, static_cast<int>(FerramentaAtiva::LIXEIRA));

    ui->btnInteracao->setIcon(QIcon(":/images/images/interacao.png"));
    ui->btnInteracao->setIconSize(TAMANHO_ICONE);
    ui->btnInteracao->setToolTip("Interação (I)");
    ui->btnInteracao->setShortcut(QKeySequence(Qt::Key_I));
    ui->btnInteracao->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnInteracao, static_cast<int>(FerramentaAtiva::INTERACAO));

    ui->btnConexao->setIcon(QIcon(":/images/images/conexao.png"));
    ui->btnConexao->setIconSize(TAMANHO_ICONE);
    ui->btnConexao->setToolTip("Conexão (C)");
    ui->btnConexao->setShortcut(QKeySequence(Qt::Key_C));
    ui->btnConexao->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnConexao, static_cast<int>(FerramentaAtiva::CONEXAO));

    ui->btnAdicionarAND->setIcon(QIcon(":/images/images/and_icon.png"));
    ui->btnAdicionarAND->setIconSize(TAMANHO_ICONE);
    ui->btnAdicionarAND->setToolTip("Porta AND (A)");
    ui->btnAdicionarAND->setShortcut(QKeySequence(Qt::Key_A));
    ui->btnAdicionarAND->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnAdicionarAND, static_cast<int>(FerramentaAtiva::ADICIONAR_AND));

    ui->btnAdicionarOR->setIcon(QIcon(":/images/images/or_icon.png"));
    ui->btnAdicionarOR->setIconSize(TAMANHO_ICONE);
    ui->btnAdicionarOR->setToolTip("Porta OR (O)");
    ui->btnAdicionarOR->setShortcut(QKeySequence(Qt::Key_O));
    ui->btnAdicionarOR->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnAdicionarOR, static_cast<int>(FerramentaAtiva::ADICIONAR_OR));

    ui->btnAdicionarNOT->setIcon(QIcon(":/images/images/not_icon.png"));
    ui->btnAdicionarNOT->setIconSize(TAMANHO_ICONE);
    ui->btnAdicionarNOT->setToolTip("Porta NOT (N)");
    ui->btnAdicionarNOT->setShortcut(QKeySequence(Qt::Key_N));
    ui->btnAdicionarNOT->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnAdicionarNOT, static_cast<int>(FerramentaAtiva::ADICIONAR_NOT));

    ui->btnAdicionarInput->setIcon(QIcon(":/images/images/input.png"));
    ui->btnAdicionarInput->setIconSize(TAMANHO_ICONE);
    ui->btnAdicionarInput->setToolTip("Input (E)");
    ui->btnAdicionarInput->setShortcut(QKeySequence(Qt::Key_E));
    ui->btnAdicionarInput->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnAdicionarInput, static_cast<int>(FerramentaAtiva::ADICIONAR_INPUT));

    ui->btnAdicionarOutput->setIcon(QIcon(":/images/images/output.png"));
    ui->btnAdicionarOutput->setIconSize(TAMANHO_ICONE);
    ui->btnAdicionarOutput->setToolTip("Output (L)");
    ui->btnAdicionarOutput->setShortcut(QKeySequence(Qt::Key_L));
    ui->btnAdicionarOutput->setCheckable(true);
    m_grupoFerramentas->addButton(ui->btnAdicionarOutput, static_cast<int>(FerramentaAtiva::ADICIONAR_OUTPUT));

    ui->btnSelecao->setChecked(true);

    on_actionNew_triggered();

    connect(m_grupoFerramentas, SIGNAL(idClicked(int)), this, SLOT(onToolButtonClicked(int)));
//essa função do próprio Qt liga os botões de ferramenta do front-end com o conjunto definido no enum
//e chama a função abaixo
}

void MainWindow::onToolButtonClicked(int id) {
    CanvasWidget* canvasAtivo = qobject_cast<CanvasWidget*>(ui->tabWidget->currentWidget());
    if (canvasAtivo) {
        FerramentaAtiva ferramenta = static_cast<FerramentaAtiva>(id);
        canvasAtivo->setFerramentaAtual(ferramenta);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    Circuito* novoCircuito = new Circuito();
    CanvasWidget* novoCanvas = new CanvasWidget(this);
    novoCanvas->setCircuito(novoCircuito);

    int index = ui->tabWidget->addTab(novoCanvas, "Circuito" + QString::number(ui->tabWidget->count()+1));
    ui->tabWidget->setCurrentIndex(index);
}

Circuito* MainWindow::getCircuitoAtivo() const{
    if(ui->tabWidget->count() == 0) return nullptr;
    CanvasWidget* canvasAtivo = qobject_cast<CanvasWidget*>(ui->tabWidget->currentWidget());
    return canvasAtivo ? canvasAtivo->getCircuito() : nullptr;
}

void MainWindow::on_actionClose_tab_triggered()
{
    if(ui->tabWidget->count()==0) return;
    int index = ui->tabWidget->currentIndex();
    CanvasWidget* canvas = qobject_cast<CanvasWidget*>(ui->tabWidget->widget(index));
    ui->tabWidget->removeTab(index);
    delete canvas;
}

void MainWindow::on_actionSave_triggered()
{
    CanvasWidget* canvasAtivo = qobject_cast<CanvasWidget*>(ui->tabWidget->currentWidget());
    if (!canvasAtivo) return;

    if (canvasAtivo->getCaminhoArquivo().isEmpty()) {
        on_actionSalvar_como_triggered();
    } else {
        canvasAtivo->getCircuito()->salvarParaArquivo(canvasAtivo->getCaminhoArquivo());
    }
}


void MainWindow::on_actionSalvar_como_triggered()
{
    CanvasWidget* canvasAtivo = qobject_cast<CanvasWidget*>(ui->tabWidget->currentWidget());
    if (!canvasAtivo) return;

    QString caminho_arquivo = QFileDialog::getSaveFileName(this, "Salvar Circuito Como...", QDir::homePath(), "Arquivo CircuitSim (*.csim)");
    if (caminho_arquivo.isEmpty()) return;

    canvasAtivo->getCircuito()->salvarParaArquivo(caminho_arquivo);

    canvasAtivo->setCaminhoArquivo(caminho_arquivo);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(caminho_arquivo).fileName());
}


void MainWindow::on_actionOpen_triggered()
{
    QString caminho_arquivo = QFileDialog::getOpenFileName(this, "Abrir Circuito", QDir::homePath(), "Arquivos CircuitSim (*.csim)");
    if (caminho_arquivo.isEmpty()) return;

    on_actionNew_triggered();
    CanvasWidget* canvasAtivo = qobject_cast<CanvasWidget*>(ui->tabWidget->currentWidget());
    if (!canvasAtivo) return;

    Circuito* circuitoAtivo = canvasAtivo->getCircuito();
    circuitoAtivo->carregarDeArquivo(caminho_arquivo, canvasAtivo->width(), canvasAtivo->height());

    canvasAtivo->setCaminhoArquivo(caminho_arquivo);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(caminho_arquivo).fileName());

    canvasAtivo->update();
}

void MainWindow::on_actionHelp_triggered()
{
    AjudaDialog dialog(this);
    dialog.exec();
}


void MainWindow::on_actionAbout_triggered()
{
    SobreDialog dialog(this);
    dialog.exec();
}

