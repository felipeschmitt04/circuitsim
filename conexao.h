#ifndef CONEXAO_H
#define CONEXAO_H

#include <QPoint>
#include <vector>

using namespace std;

class Componente;

class Conexao
{
private:
    Componente* m_origem;
    Componente* m_destino;

    int m_pino_destino;
    vector<QPoint> m_caminho;
public:
    Conexao(Componente* origem, Componente* destino, int pino_destino);

    Componente* getOrigem() const;
    Componente* getDestino() const;
    int getPinoDestino() const;

    void setCaminho(const vector<QPoint>& caminho);
    vector<QPoint>& getCaminho();
};

#endif // CONEXAO_H
