#ifndef CIRCUITO_H
#define CIRCUITO_H

#include <vector>
#include <QString>
#include <QPoint>

using namespace std;

class Componente;
class Conexao;

class Circuito
{
private:
    vector<Componente*> m_componentes;
    vector<Conexao*> m_conexoes;
    int m_proximo_id;

public:
    Circuito();
    ~Circuito();

    void criarComponente(const QString& tipo, QPoint pos, int id = -1);
    void removerComponente(Componente* componente);
    Componente* getComponenteEm(const QPoint& pos_mouse);

    Conexao* criarConexao(int id_origem, int id_destino, int pino_destino);
    void removerConexao(Conexao* conexao);
    Conexao* getConexaoEm(const QPoint& pos_mouse);
    bool verificarClique(const QPoint& pos_mouse, const QPoint& p1, const QPoint& p2);

    vector<Componente*>& getComponentes();
    vector<Conexao*>& getConexoes();
    Componente* getComponentePorId(int id);

    void simular();
    vector<vector<int>> gerarGridLogico(int largura, int altura);
    void atualizarCaminhoDe(Conexao* conn, int canvas_largura, int canvas_altura);
    void recalcularCaminhos(Componente* comp_movido, int canvas_largura, int canvas_altura);
    void recalcularTodosOsCaminhos(int canvas_largura, int canvas_altura);

    void salvarParaArquivo(const QString& caminho);
    void carregarDeArquivo(const QString& caminho, int canvas_largura, int canvas_altura);
    void limpar();
};

#endif // CIRCUITO_H
