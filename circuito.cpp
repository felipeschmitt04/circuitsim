#include "circuito.h"
#include "componente.h"
#include "conexao.h"
#include "astar.h"
#include <QFile>
#include <QSize>
#include <QRect>
#include <QHash>
#include <QDateTime>
#include <QTextStream>

Circuito::Circuito(): m_proximo_id(1){}

Circuito::~Circuito(){
    limpar();
}

void Circuito::criarComponente(const QString& tipo, QPoint pos, int id){
//função de criar componente
    Componente* novo_comp = nullptr;
    int id_final = (id == -1) ? m_proximo_id++ : id;
    if(tipo == "AND"){
        novo_comp = new PortaAND(pos, id_final);
    } else if (tipo == "OR") {
        novo_comp = new PortaOR(pos, id_final);
    } else if (tipo == "NOT") {
        novo_comp = new PortaNOT(pos, id_final);
    } else if (tipo == "INPUT") {
        novo_comp = new Input(pos, id_final);
    } else if (tipo == "OUTPUT") {
        novo_comp = new Output(pos, id_final);
    }
    if(novo_comp){
        m_componentes.push_back(novo_comp);
        if (id_final >= m_proximo_id) {
            m_proximo_id = id_final + 1;
        }
    }
}

void Circuito::removerComponente(Componente* componente_para_remover){
    if(!componente_para_remover) return;

    vector<Conexao*> conexoes_para_remover;
    for(Conexao* conn:m_conexoes){
        if(conn->getOrigem() == componente_para_remover || conn->getDestino() == componente_para_remover){
            conexoes_para_remover.push_back(conn);
        }
    }
    for(Conexao* conn: conexoes_para_remover){
        removerConexao(conn);
    }
    m_componentes.erase(remove(m_componentes.begin(), m_componentes.end(), componente_para_remover),m_componentes.end());
    delete componente_para_remover;
//função para remover componente, chama também a função de removerConexão para não ter vazamento de memória e remover as conexões
//ligadas ao componente
}

Componente* Circuito::getComponentePorId(int id) {
    for(Componente* comp : m_componentes){
        if(comp->getId()==id){
            return comp;
        }
    }
    return nullptr;
}

Componente* Circuito::getComponenteEm(const QPoint &pos_mouse){
    for (auto it = m_componentes.rbegin(); it != m_componentes.rend(); it++){
//esse for percorre a lista de componentes, o 'r' na frente das funções
//begin() e end() é para percorrer de trás para frente,
//a ideia é que o último componente adicionado vai estar sobre o componente anterior
//tanto na lista quanto na tela, então o componente na camada mais acima tem prioridade ao ser clicado

        Componente* comp = *it;

        const int LARGURA_COMP = 80;
        const int ALTURA_COMP = 60;

        QRect hit_box(comp->getPosicao(), QSize(LARGURA_COMP, ALTURA_COMP));
//cria uma área de contato

        if (hit_box.contains(pos_mouse)) {
            return comp;
        }
    }
    return nullptr;
}

Conexao* Circuito::criarConexao(int id_origem, int id_destino, int pino_destino){
    Componente* origem = getComponentePorId(id_origem);
    Componente* destino = getComponentePorId(id_destino);
//recebes os ids que foram passados como parametro quando a função for chamada
//no evento de soltar o mouse

    if(origem && destino){
        PortaLogica* porta_destino = dynamic_cast<PortaLogica*>(destino);
        Output* p_teste_destino = dynamic_cast<Output*>(destino);
        if(porta_destino){
            porta_destino->adicionarEntrada(origem, pino_destino);
        } else if(p_teste_destino){
            p_teste_destino->setEntrada(origem);
        }
        else {
            return nullptr;
        }
//faz o dynamic_cast pra ver se o componente recebido é uma porta logica ou um output
//e define uma entrada

        Conexao* nova_conexao = new Conexao(origem, destino, pino_destino);
        m_conexoes.push_back(nova_conexao);
        return nova_conexao;
    }
    return nullptr;
}

void Circuito::removerConexao(Conexao* conexao_para_remover){
//função para remover uma conexão, é chamada com a ferramenta lixeira
    if (!conexao_para_remover) return;

    Componente* destino = conexao_para_remover->getDestino();
    int pino = conexao_para_remover->getPinoDestino();
    Componente* origem = conexao_para_remover->getOrigem();

    PortaLogica* porta_destino = dynamic_cast<PortaLogica*>(destino);
    Output* p_teste_destino = dynamic_cast<Output*>(destino);

    if (porta_destino) {
        porta_destino->liberarPinoDeEntrada(pino);
    } else if (p_teste_destino) {
        if (p_teste_destino->getEntrada() == origem) {
            p_teste_destino->setEntrada(nullptr);
        }
    }

    m_conexoes.erase(remove(m_conexoes.begin(), m_conexoes.end(), conexao_para_remover), m_conexoes.end());
    delete conexao_para_remover;
    simular();
}

Conexao* Circuito::getConexaoEm(const QPoint& pos_mouse) {
    for (auto it = m_conexoes.rbegin(); it != m_conexoes.rend(); ++it) {
//acha a conexão que está mais acima naquela posição
        Conexao* conn = *it;
        if (conn->getCaminho().empty()) continue;

        const vector<QPoint>& caminho = conn->getCaminho();
        if (caminho.size() < 2) continue;

        for (size_t i = 0; i < caminho.size() - 1; ++i) {
            if (verificarClique(pos_mouse, caminho[i], caminho[i+1])) {
                return conn;
            }
        }
    }
    return nullptr;
}

bool Circuito::verificarClique(const QPoint& pos_mouse, const QPoint& p1, const QPoint& p2) {
    const int TOLERANCIA = 5;

    if (p1.y() == p2.y()) {
        return (abs(pos_mouse.y() - p1.y()) <= TOLERANCIA &&
                pos_mouse.x() >= min(p1.x(), p2.x()) - TOLERANCIA &&
                pos_mouse.x() <= max(p1.x(), p2.x()) + TOLERANCIA);
    }
    else if (p1.x() == p2.x()) {
        return (abs(pos_mouse.x() - p1.x()) <= TOLERANCIA &&
                pos_mouse.y() >= min(p1.y(), p2.y()) - TOLERANCIA &&
                pos_mouse.y() <= max(p1.y(), p2.y()) + TOLERANCIA);
    }
    return false;
//cria uma hitbox ao redor do fio
}

vector<Componente*>& Circuito::getComponentes(){
    return m_componentes;
}

vector<Conexao*>& Circuito::getConexoes(){
    return m_conexoes;
}

void Circuito::simular(){
    for(size_t i=0; i<m_componentes.size();i++){
        for(Componente* comp : m_componentes){
            comp->CalcularSaida();
        }
    }
//calcula a saída de todos os componentes para fazer o circuito funcionar
}

vector<vector<int>> Circuito::gerarGridLogico(int largura, int altura){
    const int TAMANHO_GRADE = 10;
    const int LARGURA_COMP_GRID = 80 / TAMANHO_GRADE;
    const int ALTURA_COMP_GRID = 60 / TAMANHO_GRADE;

    int grid_largura = largura / TAMANHO_GRADE;
    int grid_altura = altura / TAMANHO_GRADE;

    vector<vector<int>> grid(grid_altura, vector<int>(grid_largura, 0));
//cria uma matriz com todos os seus valores = 0

    for (Componente* comp : m_componentes) {
        int x_start = comp->getPosicao().x() / TAMANHO_GRADE;
        int y_start = comp->getPosicao().y() / TAMANHO_GRADE;

        for (int y = y_start; y < y_start + ALTURA_COMP_GRID; ++y) {
            for (int x = x_start; x < x_start + LARGURA_COMP_GRID; ++x) {
                if (y >= 0 && y < grid_altura && x >= 0 && x < grid_largura) {
                    grid[y][x] = 1;
                }
            }
        }
//para cada componente, pega sua posição e divide pelo tamanho da grade
//para otimizar a execução, pois o tamanho de todos os componentes e o caminho que os fios
//podem passar são divisíveis pelo tamanho da grade, resultando em posições na matriz lógica,
//os for percorrem as posições que o componente ocupa e define = 1
//isso vai servir para o algoritmo A* saber onde ele não pode passar.
//resumindo, literalmente ta transformando a grade visual em uma grade lógica para
//o back-end funcionar de forma sincronizada com o que é exibido

    }
    return grid;
}

void Circuito::salvarParaArquivo(const QString& caminho)
{
    QFile arquivo(caminho);
    if (!arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&arquivo);

    for (size_t i = 0; i < m_componentes.size(); ++i) {
        Componente* comp = m_componentes[i];

        QString linha = "COMP;";
        linha += "TIPO=" + comp->getTipoString() + ";";
        linha += "ID=" + QString::number(comp->getId()) + ";";
        linha += "POS=" + QString::number(comp->getPosicao().x()) + "," + QString::number(comp->getPosicao().y()) + ";";
        linha += "NOME=" + comp->getNome();

        out << linha << "\n";
    }

    for (size_t i = 0; i < m_conexoes.size(); ++i) {
        Conexao* conn = m_conexoes[i];

        QString linha = "CONN;";
        linha += "ORIGEM_ID=" + QString::number(conn->getOrigem()->getId()) + ";";
        linha += "DESTINO_ID=" + QString::number(conn->getDestino()->getId()) + ";";
        linha += "PINO_DESTINO=" + QString::number(conn->getPinoDestino());

        out << linha << "\n";
    }
}

void Circuito::carregarDeArquivo(const QString& caminho, int canvas_largura, int canvas_altura)
{
    QFile arquivo(caminho);
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    limpar();

    QString conteudo_arquivo = arquivo.readAll();
    const QStringList linhas = conteudo_arquivo.split('\n');
    arquivo.close();

    for (int i = 0; i < linhas.size(); ++i) {
        QString linha = linhas[i];
        if (linha.startsWith("#") || linha.isEmpty()) continue;

        QStringList partes = linha.split(';');
        if (partes.first() == "COMP") {
            partes.removeFirst();

            QString tipo;
            int id = -1;
            QPoint pos;

            for (int j = 0; j < partes.size(); ++j) {
                QString parte = partes[j];
                QStringList par = parte.split('=');
                if (par.size() == 2) {
                    QString chave = par[0];
                    QString valor = par[1];

                    if (chave == "TIPO") tipo = valor;
                    else if (chave == "ID") id = valor.toInt();
                    else if (chave == "POS") {
                        QStringList pos_str = valor.split(',');
                        if (pos_str.size() == 2) {
                            pos.setX(pos_str[0].toInt());
                            pos.setY(pos_str[1].toInt());
                        }
                    }
                }
            }
            if (id != -1) {
                criarComponente(tipo, pos, id);
            }
        }
    }

    for (int i = 0; i < linhas.size(); ++i) {
        QString linha = linhas[i];
        if (linha.startsWith("#") || linha.isEmpty()) continue;

        QStringList partes = linha.split(';');
        if (partes.first() == "CONN") {
            partes.removeFirst();

            int id_origem = -1, id_destino = -1, pino_destino = -1;

            for (int j = 0; j < partes.size(); ++j) {
                QString parte = partes[j];
                QStringList par = parte.split('=');
                if (par.size() == 2) {
                    QString chave = par[0];
                    QString valor = par[1];

                    if (chave == "ORIGEM_ID") id_origem = valor.toInt();
                    else if (chave == "DESTINO_ID") id_destino = valor.toInt();
                    else if (chave == "PINO_DESTINO") pino_destino = valor.toInt();
                }
            }
            if (id_origem != -1 && id_destino != -1 && pino_destino != -1) {
                criarConexao(id_origem, id_destino, pino_destino);
            }
        }
    }

    recalcularTodosOsCaminhos(canvas_largura, canvas_altura);
}

void Circuito::limpar() {
    for (Conexao* conn : m_conexoes) {
        delete conn;
    }
    m_conexoes.clear();

    for (Componente* comp : m_componentes) {
        delete comp;
    }
    m_componentes.clear();

    m_proximo_id = 1;
//apaga tudo
}

void Circuito::atualizarCaminhoDe(Conexao* conn, int canvas_largura, int canvas_altura){
    if (!conn || !conn->getOrigem() || !conn->getDestino()) return;
//verifica se os componentes sao validos

    const int TAMANHO_GRADE = 10;

    vector<vector<int>> grid_logico = gerarGridLogico(canvas_largura, canvas_altura);
//gera o grid pro back-end

    Componente* origem = conn->getOrigem();
    Componente* destino = conn->getDestino();

    QPoint pino_origem_grid(origem->getPosicaoPinoSaida().x() / TAMANHO_GRADE,
                            origem->getPosicaoPinoSaida().y() / TAMANHO_GRADE);
    QPoint pino_destino_grid(destino->getPosicaoPinoEntrada(conn->getPinoDestino()).x() / TAMANHO_GRADE,
                             destino->getPosicaoPinoEntrada(conn->getPinoDestino()).y() / TAMANHO_GRADE);
//pega a posição dos pinos e divide pelo tamanho por 10, explicado o porque na função de gerar grid

    QPoint startPoint(pino_origem_grid.x() + 2, pino_origem_grid.y());
    QPoint goalPoint(pino_destino_grid.x() - 2, pino_destino_grid.y());
//soma 2 a cada ponto, isso faz com que pelo menos 2 segmentos saiam na horizontal,
//apenas por detalhes estéticos

    for(int i=0; i <= 2; ++i) {
        if(pino_origem_grid.y() >= 0 && static_cast<size_t>(pino_origem_grid.y()) < grid_logico.size() &&
            pino_origem_grid.x() + i >= 0 && static_cast<size_t>(pino_origem_grid.x() + i) < grid_logico[0].size()) {
            grid_logico[pino_origem_grid.y()][pino_origem_grid.x() + i] = 0;
        }
        if(pino_destino_grid.y() >= 0 && static_cast<size_t>(pino_destino_grid.y()) < grid_logico.size() &&
            pino_destino_grid.x() - i >= 0 && static_cast<size_t>(pino_destino_grid.x() - i) < grid_logico[0].size()) {
            grid_logico[pino_destino_grid.y()][pino_destino_grid.x() - i] = 0;
        }
//faz com que os pontos de entrada e saída sejam definidos como 0 no grid lógico, para caso um
//componente for colocado muito próximo do outro
    }

    vector<QPoint> caminho_grid_astar = findPath(startPoint, goalPoint, grid_logico);
//bota no waze

    vector<QPoint> caminho_final_grid;
    if (!caminho_grid_astar.empty()) {
        caminho_final_grid.push_back(pino_origem_grid);
        caminho_final_grid.push_back(startPoint);
        caminho_final_grid.insert(caminho_final_grid.end(), caminho_grid_astar.begin(), caminho_grid_astar.end());
//pega todo o vetor que vem do astar e engata no final do caminho_final_grid
        caminho_final_grid.push_back(goalPoint);
        caminho_final_grid.push_back(pino_destino_grid);
    }
//se encontrou o caminho, ele aloca em ordem o caminho, porque ta tudo separado?
//porque os dois primeiros e dois últimos segmentos já foram pré definidos.

    vector<QPoint> caminho_visual;
    for (const QPoint& p_grid : caminho_final_grid) {
        caminho_visual.push_back(QPoint(p_grid.x() * TAMANHO_GRADE, p_grid.y() * TAMANHO_GRADE));
    }
//multiplica todo mundo por 10 pra ficar relativo à grade visual
    conn->setCaminho(caminho_visual);
}

void Circuito::recalcularCaminhos(Componente* comp_movido, int canvas_largura, int canvas_altura) {
    if (!comp_movido) return;

    for (Conexao* conn : m_conexoes) {
        if (conn->getOrigem() == comp_movido || conn->getDestino() == comp_movido) {
            atualizarCaminhoDe(conn, canvas_largura, canvas_altura);
        }
    }
//essa função atualiza em tempo real a conexão de um componente que ta sendo arrastado
}

void Circuito::recalcularTodosOsCaminhos(int canvas_largura, int canvas_altura) {
    for (Conexao* conn : m_conexoes) {
        atualizarCaminhoDe(conn, canvas_largura, canvas_altura);
    }
//essa função recalcula todos os caminhos de todas as conexões
//por que eu fiz uma que recalcula apenas um e outra que recalcula todos?
//porque uma é chamada quando os componentes são movidos e outra
//quando o botão do mouse é solto e quando carrego arquivos
//por que somente quando é solto?
//porque eu posso soltar um componente em cima de uma conexão
//aí ela vai precisar "fugir" dele
//e atualizar todos os caminhos à cada milésimo que o mouse é movido não é legal pra pc fraco
//se tiver muitas conexões
}

