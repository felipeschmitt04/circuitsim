#ifndef ASTAR_H
#define ASTAR_H

#include <QPoint>

using namespace std;

struct Node{
    int g_cost = 0;
    int h_cost = 0;
    QPoint pos;
    Node* parent = nullptr;

    Node(QPoint p, Node* parent_node = nullptr) : pos(p), parent(parent_node) {}

    int get_f_cost() const{
        return g_cost + h_cost;
    }
};

vector<QPoint> findPath(QPoint saida, QPoint chegada, const vector<vector<int>>& grid);
//apesar de nao ser uma classe, declarei essa função no header para ela ser chamada posteriormente
//em um arquivo cpp
//link de referencia para construcao do algoritmo A*: https://www.youtube.com/watch?v=ySN5Wnu88nE&list=PLBWLDH_qy9ng0QASdrtvgnbAR7fXdw_lW
//tive que fazer umas adaptacoes para o Qt, como na parte de posicao
//tambem adicionei uma "penalidade" para toda vez que o fio mudar de direcao, dessa forma o algoritmo tenta
//manter linhas retas, deixando visualmente mais bonito e organizado

#endif // ASTAR_H
