#include "conexao.h"
#include "componente.h"

Conexao::Conexao(Componente* origem, Componente* destino, int pino_destino) : m_origem(origem), m_destino(destino), m_pino_destino(pino_destino){}

Componente* Conexao::getOrigem() const{
    return m_origem;
}

Componente* Conexao::getDestino() const{
    return m_destino;
}

int Conexao::getPinoDestino() const{
    return m_pino_destino;
}

void Conexao::setCaminho(const vector<QPoint>& caminho){
    m_caminho = caminho;
}

vector<QPoint>& Conexao::getCaminho(){
    return m_caminho;
}
