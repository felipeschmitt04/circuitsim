 #include "astar.h"
#include <queue>
#include <map>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

int distanciaManhattan(QPoint a, QPoint b) {
    return abs(a.x() - b.x()) + abs(a.y() - b.y());
}
//essa função acima calcular a distancia de um ponto a outro, mas não pela diagonal
//e sim pelos catetos, o algoritmo vai usar essa distancia para achar o caminho com menor custo

struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->get_f_cost() > b->get_f_cost();
    }
};
//essa struct vai ser usada como uma função para ordenar a openList
//o operator() faz com que o objeto da struct se comporte como uma função quando chamado

vector<QPoint> findPath(QPoint saida, QPoint chegada, const vector<vector<int>>& grid) {
    Node* startNode = new Node(saida);
    startNode->h_cost = distanciaManhattan(saida, chegada);

    priority_queue<Node*, vector<Node*>, CompareNode> openList;
//a função priority_queue é do próprio c++, mas ela ordena os valores do maior para o menor
//nesse caso, eu preciso do contrário, deixar o menor com mais prioridade,
//para assim o algoritmo saber o nó que tem menor custo.
//essa lista espera 3 parâmetros, o tipo de dados, onde vão ser guardados e um objeto de função
//para fazer a comparação entre eles.

    openList.push(startNode);
//adiciona ao inicio da fila

    map<QPoint, Node*> allNodes;
//map é como um vetor de vários pair, só que mais dinâmico, pois ele define uma "chave" de busca,
//então ao inves de percorrer todo o vetor com um for, ele apenas busca pela palavra chave.

    allNodes[saida] = startNode;

    while (!openList.empty()) {
        Node* currentNode = openList.top();
//pega o item no topo da openList e atribui ao node atual.
//esse item no topo é o mais promissor a fazer parte do melhor caminho que tem menor f_cost

        openList.pop();
//tira esse node da lista pois ele vai ser processado nesse laço, então no próximo laço o algoritmo
//vai processar o próximo node.

        if (currentNode->pos == chegada) {
//se a posição do node atual é a chegada
            vector<QPoint> path;
//cria o vetor do caminho
            Node* temp = currentNode;
//atribui o ponteiro do node atual a um ponteiro temporario

            while (temp != nullptr) {
                path.push_back(temp->pos);
                temp = temp->parent;
//percorre todo o caminho de tras para frente, até chegar ao node com parent == nullptr, que é o node inicial
            }
            reverse(path.begin(), path.end());
//inverte o caminho, para ficar na direção correta

            for (auto & [key, val] : allNodes) {
                delete val;
            }
//esse for utiliza uma construção moderna, funciona semelhante a um forEach do javascript,
//ele percorre todo o map criado antes e libera a memória.
//o auto deixa pro compilador decidir qual o tipo do dado, o & significa que cada item vai ser acessado por referencia e o allNodes
//é o map que vai ser percorrido.
//[key, val] é a mesma coisa que acessar por .first e .second, mas ele faz isso automaticamente.
//só deletamos val pois ele é o ponteiro que vai ser limpo.

            return path;
//retorna o caminho se deu tudo certo
        }

        QPoint neighbors[] = { QPoint(0, -1), QPoint(0, 1), QPoint(-1, 0), QPoint(1, 0) };
//é um array com as possiveis direções que o algoritmo pode seguir

        for (int i = 0; i < 4; i++) {
//percorre o array dos vizinhos
            QPoint dir = neighbors[i];
            QPoint neighborPos = currentNode->pos + dir;
//define a posição do vizinho como a posição do node atual + a direção

            if (neighborPos.y() < 0 || static_cast<size_t>(neighborPos.y()) >= grid.size() ||
                neighborPos.x() < 0 || static_cast<size_t>(neighborPos.x()) >= grid[0].size()) {
                continue;
//se a posição do vizinho está fora dos limites da grid recomeçar o laço
            }
            if (grid[neighborPos.y()][neighborPos.x()] == 1) {
                continue;
//se a posição do vizinho for ==1, significa que tem um componente no caminho
            }

            int custo_movimento = 1;
            if (currentNode->parent != nullptr) {
                int dir_anterior_x = currentNode->pos.x() - currentNode->parent->pos.x();
                int dir_anterior_y = currentNode->pos.y() - currentNode->parent->pos.y();
                if (dir.x() != dir_anterior_x || dir.y() != dir_anterior_y) {
                    custo_movimento += 10;
                }
//lógica para fazer com que o algoritmo tente seguir o maximo de linhas retas possiveis,
//ele calcula se mudou o valor de x ou y e soma 10 no g_cost atual
            }

            int tentative_g_cost = currentNode->g_cost + custo_movimento;

            if (allNodes.find(neighborPos) == allNodes.end() || tentative_g_cost < allNodes[neighborPos]->g_cost) {
//se esse esse vizinho está no final da lista, significa que é a primeira vez que ele é visitado
// ou se o custo para passar por esse node está menor do que estava antes, então a condição é verdadeira

                Node* neighborNode;
                if (allNodes.find(neighborPos) != allNodes.end()) {
                    neighborNode = allNodes[neighborPos];
//se ele ja visitou esse node antes, ele pega o ponteiro que ja existia

                } else {
                    neighborNode = new Node(neighborPos);
                    allNodes[neighborPos] = neighborNode;
//se ele nunca visitou, cria um novo Node e guarda no map

                }
                neighborNode->parent = currentNode;
                neighborNode->g_cost = tentative_g_cost;
                neighborNode->h_cost = distanciaManhattan(neighborPos, chegada);
//aqui é definido o node pai do node vizinho, o g_cost e o h_cost

                openList.push(neighborNode);
            }
        }
    }
    for (auto & [key, val] : allNodes) {
        delete val;
    }
//mesmo esquema explicado antes

    return {};
//retorna um vetor vazio se não achou um caminho
}

bool operator<(const QPoint& a, const QPoint& b) {
    if (a.y() == b.y()) {
        return a.x() < b.x();
    }
    return a.y() < b.y();
}
//esse operator é necessário para o map, pois o compilador não sabe lidar nativamente com
//valores QPoint
