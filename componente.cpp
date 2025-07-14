#include "componente.h"
#include <QPainter>
#include <QFont>
#include <QRect>
#include <QDebug>

const int LARGURA_COMP = 80;
const int ALTURA_COMP = 60;

Componente::Componente(const QString& nome, QPoint pos, int id)
    : m_id(id), m_posicao(pos), m_nome(nome), m_estado_saida(false) {}

int Componente::getId() const {return m_id;}

QString& Componente::getNome() {return m_nome;}

QPoint Componente::getPosicao() {return m_posicao;}

void Componente::setPosicao(const QPoint& pos){m_posicao = pos;}

bool Componente::getEstadoSaida() {return m_estado_saida;}

QPoint Componente::getPosicaoPinoEntrada(int pino) const{
    Q_UNUSED(pino);
    return QPoint(m_posicao.x(), m_posicao.y() + ALTURA_COMP / 2);
}

QPoint Componente::getPosicaoPinoSaida(){
    return QPoint(m_posicao.x() + LARGURA_COMP, m_posicao.y() + ALTURA_COMP / 2);
}

PortaLogica::PortaLogica(const QString& nome, QPoint pos, int id, int num_entradas)
    : Componente(nome, pos, id), m_num_entradas(num_entradas) {
    m_entradas.resize(num_entradas, nullptr);
}

void PortaLogica::adicionarEntrada(Componente* componente, int pino){
    if(pino>=0 && pino<m_num_entradas){
        m_entradas[pino] = componente;
    }
}

void PortaLogica::liberarPinoDeEntrada(int pino) {
    if (pino >= 0 && static_cast<size_t>(pino) < m_entradas.size()) {
        m_entradas[pino] = nullptr;
    }
}

int PortaLogica::getNumeroEntradas()const{return m_num_entradas;}

QPoint PortaLogica::getPosicaoPinoEntrada(int pino) const{
    if(pino<0 || pino>= m_num_entradas){
        return m_posicao;
    }
    int secao_altura = ALTURA_COMP / (m_num_entradas+1);
    int y_offset = secao_altura * (pino+1);
    return QPoint(m_posicao.x(), m_posicao.y() + y_offset);
}

bool PortaLogica::pinoEstaOcupado(int pino) const {
    if (pino < 0 || static_cast<size_t>(pino) >= m_entradas.size()) {
        return true;
    }
    return m_entradas[pino] != nullptr;
}

int PortaLogica::getPinoEntradaMaisProximo(const QPoint& pos_mouse) const {
    if (m_num_entradas == 1) {
        return 0;
    }

    int centro_y = m_posicao.y() + (ALTURA_COMP / 2);

    if (pos_mouse.y() < centro_y) {
        return 0;
    } else {
        return 1;
    }
}

PortaAND::PortaAND(QPoint pos, int id) : PortaLogica("AND_"+QString::number(id), pos, id, 2){
    m_imagem.load(":/images/images/and.png");
};

void PortaAND::desenhar(QPainter* painter){
    if(!m_imagem.isNull()){
        QRectF target_rect(m_posicao, QSize(LARGURA_COMP, ALTURA_COMP));
        QRectF source_rect = m_imagem.rect();
        painter->drawPixmap(target_rect, m_imagem, source_rect);
    }
}

void PortaAND::CalcularSaida(){
    if(!m_entradas[0]|| !m_entradas[1]){
        m_estado_saida = false;
        return;
    }
    m_estado_saida = m_entradas[0]->getEstadoSaida() && m_entradas[1]->getEstadoSaida();
}

QString PortaAND::getTipoString() const {
    return "AND";
}

PortaOR::PortaOR(QPoint pos, int id) : PortaLogica("OR_"+QString::number(id), pos, id, 2){
    m_imagem.load(":/images/images/or.png");
}

void PortaOR::desenhar(QPainter* painter){
    if(!m_imagem.isNull()){
        QRectF target_rect(m_posicao, QSize(LARGURA_COMP, ALTURA_COMP));
        QRectF source_rect = m_imagem.rect();
        painter->drawPixmap(target_rect, m_imagem, source_rect);
    }
}

void PortaOR::CalcularSaida(){
    if(!m_entradas[0] || !m_entradas[1]){
        m_estado_saida = false;
        return;
    }
    m_estado_saida = m_entradas[0]->getEstadoSaida() || m_entradas[1]->getEstadoSaida();
}

QString PortaOR::getTipoString() const {
    return "OR";
}

PortaNOT::PortaNOT(QPoint pos, int id) : PortaLogica("NOT_"+QString::number(id), pos, id, 1){
    m_imagem.load(":/images/images/not.png");
}

void PortaNOT::desenhar(QPainter* painter){
    if(!m_imagem.isNull()){
        QRectF target_rect(m_posicao, QSize(LARGURA_COMP, ALTURA_COMP));
        QRectF source_rect = m_imagem.rect();
        painter->drawPixmap(target_rect, m_imagem, source_rect);
    }
}

void PortaNOT::CalcularSaida(){
    if(!m_entradas[0]){
        m_estado_saida = false;
        return;
    }
    m_estado_saida = !m_entradas[0]->getEstadoSaida();
}

QString PortaNOT::getTipoString() const {
    return "NOT";
}

Input::Input(QPoint pos, int id) : Componente("INPUT_"+QString::number(id), pos, id){}

void Input::desenhar(QPainter* painter){
    QRect corpo(m_posicao, QSize(LARGURA_COMP, ALTURA_COMP));
    painter->setBrush(m_estado_saida ? Qt::green : Qt::red);
    painter->drawRect(corpo);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 12, QFont::Bold));
    painter->drawText(corpo, Qt::AlignCenter, m_estado_saida ? "1" : "0");
    painter->setPen(Qt::black);
}

void Input::CalcularSaida(){};

void Input::alternarEstado(){
    m_estado_saida = !m_estado_saida;
}

QString Input::getTipoString() const {
    return "INPUT";
}

Output::Output(QPoint pos, int id) : Componente("OUTPUT_"+QString::number(id), pos, id),m_entrada(nullptr){}

void Output::desenhar(QPainter* painter){
    bool estado_entrada = m_entrada ? m_entrada->getEstadoSaida() : false;
    QRectF led(m_posicao.x(), m_posicao.y(), ALTURA_COMP, ALTURA_COMP);
    painter->setBrush(estado_entrada ? QColor(0, 150, 255) : Qt::darkGray);
    painter->drawEllipse(led);
}

void Output::CalcularSaida(){}

void Output::setEntrada(Componente* componente){m_entrada = componente;}

QString Output::getTipoString() const {
    return "OUTPUT";
}

bool Output::isOcupado() const {
    return m_entrada != nullptr;
}

Componente* Output::getEntrada() const {
    return m_entrada;
}
