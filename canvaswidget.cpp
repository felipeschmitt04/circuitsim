#include "canvaswidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include "conexao.h"
#include "componente.h"

const int TAMANHO_GRADE = 10;
const int ALTURA_COMP = 60;
const int LARGURA_COMP = 80;

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget{parent}, m_circuito(nullptr), m_ferramenta_atual(FerramentaAtiva::SELECAO), m_arrastando_componente(false), m_componente_selecionado(nullptr),
    m_desenhando_conexao(false), m_origem_conexao(nullptr)
{
    setMouseTracking(true);
}

CanvasWidget::~CanvasWidget(){
    delete m_circuito;
}

void CanvasWidget::setCircuito(Circuito* circuito) { m_circuito = circuito; }

Circuito* CanvasWidget::getCircuito(){ return m_circuito; }

void CanvasWidget::setFerramentaAtual(FerramentaAtiva nova_ferramenta){
    m_ferramenta_atual = nova_ferramenta;
//recebe a ferramenta ativa
    m_cursor_ferramenta = QPixmap();
//reseta o cursor da ferramenta

    const QSize TAMANHO_CURSOR(100, 100);
//define o tamanho do cursor

    switch (m_ferramenta_atual) {
    case FerramentaAtiva::ADICIONAR_AND:{
        QPixmap original(":/images/images/and_icon.png");
        m_cursor_ferramenta = original.scaled(TAMANHO_CURSOR,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation);
        setCursor(Qt::BlankCursor);
        break;
    }
    case FerramentaAtiva::ADICIONAR_OR:{
        QPixmap original(":/images/images/or_icon.png");
        m_cursor_ferramenta = original.scaled(TAMANHO_CURSOR,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation);
        setCursor(Qt::BlankCursor);
        break;
    }
    case FerramentaAtiva::ADICIONAR_NOT:{
        QPixmap original(":/images/images/not_icon.png");
        m_cursor_ferramenta = original.scaled(TAMANHO_CURSOR,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation);
        setCursor(Qt::BlankCursor);
        break;
    }
    default:
        setCursor(Qt::ArrowCursor);
        if (m_ferramenta_atual == FerramentaAtiva::LIXEIRA) setCursor(Qt::CrossCursor);
        if (m_ferramenta_atual == FerramentaAtiva::INTERACAO) setCursor(Qt::PointingHandCursor);
        if (m_ferramenta_atual == FerramentaAtiva::ADICIONAR_INPUT) setCursor(Qt::CrossCursor);
        if (m_ferramenta_atual == FerramentaAtiva::ADICIONAR_OUTPUT) setCursor(Qt::CrossCursor);
        break;
    }
//esse switch case serve para definir o cursor, se for uma das ferramentas de adicionar porta lógica,
//ficará a imagem delas
//se não, ficará um cursor comum, mas se uma das ferramentas que não coloquei imagem tiver ativa,
//elas terão um cursor diferente.
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
//essa função é do próprio QT para definir que um parâmetro não foi usado,
//nesse caso, a função paintEvent é do próprio QT e ela obriga ter um parametro desse tipo
// mesmo que não usado.

    QPainter painter(this);
//cria o "pincel" para esse canvas


    painter.fillRect(rect(), QColor("#21252b"));
//pinta todo o fundo do canvas

    QPen pen_grade(QColor("#2c313a"), 1, Qt::DotLine);
    painter.setPen(pen_grade);
    for (int x = 0; x < this->width(); x += TAMANHO_GRADE) {
        painter.drawLine(x, 0, x, this->height());
    }
    for (int y = 0; y < this->height(); y += TAMANHO_GRADE) {
        painter.drawLine(0, y, this->width(), y);
    }
//desenha as linhas

    for (Conexao* conn : m_circuito->getConexoes()) {
//percorre todas as conexões do circuito
        vector<QPoint>& caminho = conn->getCaminho();
        if (caminho.size() < 2) continue;
//esse if evita que o programa feche quando não for possível desenhar uma conexão

        bool estado = conn->getOrigem()->getEstadoSaida();
        QColor cor = estado ? QColor(0, 255, 0) : QColor(0, 100, 0);
//define a cor da conexão com base no sinal que está passando por ela

        painter.setPen(QPen(cor, 2));
//define a caneta que vai ser usada, com base na QColor cor e de espessura = 2

        for (size_t i = 0; i < caminho.size() - 1; i++) {
            painter.drawLine(caminho[i], caminho[i+1]);
//vai desenhar a conexão a cada par de segmentos
        }
    }

    for (Componente* comp : m_circuito->getComponentes()) {
        comp->desenhar(&painter);
//chama a função desenhar referente a cada componente, viva o polimorfismo
    }

    if (!m_cursor_ferramenta.isNull()) {
//responsável pelo cursor com imagem as ferramentas de adicionar portas lógicas

        painter.save();
//salva o estado padrão

        painter.setOpacity(0.5);

        QPoint pos_cursor_img = m_posicao_mouse - QPoint(m_cursor_ferramenta.width() / 2, m_cursor_ferramenta.height() / 2);
//serve para que a imagem fique centralizada no ponteiro do mouse,
//pois a posição padrão é no canto superior esquerdo (0, 0)

        painter.drawPixmap(pos_cursor_img, m_cursor_ferramenta);

        painter.restore();
//volta para o estado salvo
    }

    if (m_desenhando_conexao && m_origem_conexao) {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        QPoint pino_saida = m_origem_conexao->getPosicaoPinoSaida();
        painter.drawLine(pino_saida, m_preview_ponto_final);
//serve para dar o preview da linha que vai ser desenhada
// enquanto o usuário ainda está arrastando o mouse,
//utilizei valores do tipo bool (flags) como condição
    }
}

void CanvasWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() != Qt::LeftButton || !m_circuito){
        return;
//se o clique não foi com o botão esquerdo, nada acontece
    }
    switch(m_ferramenta_atual){
    case FerramentaAtiva::SELECAO:{
        m_componente_selecionado = m_circuito->getComponenteEm(event->pos());
//componente selecionado vai ser o que foi clicado na posição que é passada como parametro,
//event->pos() é uma função do qt exatamente para isso.
        if(m_componente_selecionado){
            m_arrastando_componente = true;
            m_offset_arraste = event->pos() - m_componente_selecionado->getPosicao();
//o offset arraste é para quando o objeto for clicado ele nao pular para o centro do cursor
        }
        break;
    }
    case FerramentaAtiva::LIXEIRA:{
        Conexao* conn = m_circuito->getConexaoEm(event->pos());
        if(conn){
            m_circuito->removerConexao(conn);
//maior prioridade para apagar uma conexao
        } else {
            Componente* comp = m_circuito->getComponenteEm(event->pos());
            if(comp){
                m_circuito->removerComponente(comp);
            }
        }
        break;
    }
    case FerramentaAtiva::INTERACAO:{
        Componente* comp = m_circuito->getComponenteEm(event->pos());
//essa funçao retorna um ponteiro genérico da classe componente
        Input* fonte = dynamic_cast<Input*>(comp);
//o dynamic_cast testa se esse componente é um Input e aí sim atribui ao ponteiro fonte
//vai evitar que fecha o programa se o usuário tentar alterar o estado de uma porta lógica por exemplo
        if(fonte){
            fonte->alternarEstado();
            m_circuito->simular();
        }
        break;
    }
    case FerramentaAtiva::CONEXAO:{
        Componente* comp = m_circuito->getComponenteEm(event->pos());
        if(comp && dynamic_cast<Output*>(comp)==nullptr){
            m_desenhando_conexao = true;
            m_origem_conexao = comp;
            m_preview_ponto_final = event->pos();
//verifica se clicou em um componente e se ele não clicou em um output, pois o output não tem saída
//se sim, ativa a flag desenhando_conexao e define a origem e o preview;
        }
        break;
    }
    case FerramentaAtiva::ADICIONAR_AND: {
        QPoint pos_mira = event->pos();
        QPoint pos_componente = pos_mira - QPoint(LARGURA_COMP / 2, ALTURA_COMP / 2);
        QPoint pos_final_alinhada = alinhar(pos_componente);
        m_circuito->criarComponente("AND", pos_final_alinhada);
//essas operações são para centralizar a posição de onde as portas lógicas vão ser colocadas
        break;
    }
    case FerramentaAtiva::ADICIONAR_OR: {
        QPoint pos_mira = event->pos();
        QPoint pos_componente = pos_mira - QPoint(LARGURA_COMP / 2, ALTURA_COMP / 2);
        QPoint pos_final_alinhada = alinhar(pos_componente);
        m_circuito->criarComponente("OR", pos_final_alinhada);
        break;
    }
    case FerramentaAtiva::ADICIONAR_NOT: {
        QPoint pos_mira = event->pos();
        QPoint pos_componente = pos_mira - QPoint(LARGURA_COMP / 2, ALTURA_COMP / 2);
        QPoint pos_final_alinhada = alinhar(pos_componente);
        m_circuito->criarComponente("NOT", pos_final_alinhada);
        break;
    }
    case FerramentaAtiva::ADICIONAR_INPUT: {
        QPoint pos_mira = event->pos();
        QPoint pos_componente = pos_mira - QPoint(LARGURA_COMP / 2, ALTURA_COMP / 2);
        QPoint pos_final_alinhada = alinhar(pos_componente);
        m_circuito->criarComponente("INPUT", pos_final_alinhada);
        break;
    }
    case FerramentaAtiva::ADICIONAR_OUTPUT: {
        QPoint pos_mira = event->pos();
        QPoint pos_componente = pos_mira - QPoint(LARGURA_COMP / 2, ALTURA_COMP / 2);
        QPoint pos_final_alinhada = alinhar(pos_componente);
        m_circuito->criarComponente("OUTPUT", pos_final_alinhada);
        break;
    }
    }
    m_circuito->recalcularTodosOsCaminhos(this->width(), this->height());
    update();
//atualiza a tela
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event){
    m_posicao_mouse = event->pos();

    if(!m_cursor_ferramenta.isNull()){
        update();
//se tiver usando um cursor com imagem, a tela será atualizada constantemente
    }

    if (m_arrastando_componente && m_componente_selecionado) {
        QPoint nova_pos_bruta = event->pos() - m_offset_arraste;
        QPoint nova_pos_alinhada = alinhar(nova_pos_bruta);

        if (m_componente_selecionado->getPosicao() != nova_pos_alinhada) {
            m_componente_selecionado->setPosicao(nova_pos_alinhada);
            m_circuito->recalcularCaminhos(m_componente_selecionado, this->width(), this->height());
            update();
        }
//função de arrastar componente, chama a função alinhar para encaixar os componentes na alinhados com a grade
    }
    if(m_desenhando_conexao){
        m_preview_ponto_final = event->pos();
        update();
        return;
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event){
        if (event->button() == Qt::LeftButton) {
            if (m_desenhando_conexao) {
                Componente* comp_destino = m_circuito->getComponenteEm(event->pos());
//recebe o componente que o usuário soltou o mouse em cima

                if (comp_destino && comp_destino != m_origem_conexao && dynamic_cast<Input*>(comp_destino) == nullptr) {
//se tem um componente de destino e se esse componente for diferente do de origem e
//se esse componente nao for um input (pois não tem entrada no input)

                    int pino_alvo = -1;
//defini pino_alvo como -1 porque é um valor inválido para as entradas

                    PortaLogica* porta = dynamic_cast<PortaLogica*>(comp_destino);
                    Output* p_teste = dynamic_cast<Output*>(comp_destino);
//usei dynamic_cast para verificar se o componente é uma porta_logica ou output

                    if (porta) {
                        int pino_candidato = porta->getPinoEntradaMaisProximo(event->pos());
//o pino candidato é o da função, ela basicamente define qual pino vai ser usado com base
//de onde o usuário soltou o mouse, se foi da metade pra cima ou da metade pra baixo
//já que as portas só tem duas entradas

                        if (!porta->pinoEstaOcupado(pino_candidato)) {
                            pino_alvo = pino_candidato;
                        }
                    } else if (p_teste) {
                        if (!p_teste->isOcupado()) {
                            pino_alvo = 0;
                        }
                    }

                    if (pino_alvo != -1) {
//vai criar a conexao
                        Conexao* nova_conn = m_circuito->criarConexao(m_origem_conexao->getId(), comp_destino->getId(), pino_alvo);
                        if (nova_conn) {
                            m_circuito->atualizarCaminhoDe(nova_conn, this->width(), this->height());
                            m_circuito->simular();
                        }
                    }
                }
                m_desenhando_conexao = false;
                m_origem_conexao = nullptr;
//reseta as flags e atualiza a tela
                update();
            }
            else if (m_arrastando_componente) {
                if (m_circuito) {
                    m_circuito->recalcularTodosOsCaminhos(this->width(), this->height());
                }
                m_arrastando_componente = false;
                m_componente_selecionado = nullptr;
                update();
            }
        }
}

QPoint CanvasWidget::alinhar(QPoint& pos){
    int alinharX = qRound(static_cast<double>(pos.x()) / TAMANHO_GRADE) * TAMANHO_GRADE;

    int alinharY = qRound(static_cast<double>(pos.y()) / TAMANHO_GRADE) * TAMANHO_GRADE;

    return QPoint(alinharX, alinharY);

//função que alinha os componentes à grade
}

void CanvasWidget::setCaminhoArquivo(QString& caminho)
{
    m_caminhoArquivo = caminho;
}

QString CanvasWidget::getCaminhoArquivo()
{
    return m_caminhoArquivo;
}
