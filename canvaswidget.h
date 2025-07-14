#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include "circuito.h"

enum FerramentaAtiva { SELECAO, LIXEIRA, INTERACAO, CONEXAO, ADICIONAR_AND, ADICIONAR_OR, ADICIONAR_NOT, ADICIONAR_INPUT, ADICIONAR_OUTPUT };
//referencia: https://www.w3schools.com/cpp/cpp_enum.asp
//usei por questao de boas práticas, até poderia definir cada um como um valor inteiro ou colocar em um vetor (codigo ficaria menos legivel
//no switch case), mas dessa forma é mais prático e mais seguro

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent = nullptr);

    ~CanvasWidget();

    void setCircuito(Circuito* circuito);
    Circuito* getCircuito();
    void setFerramentaAtual(FerramentaAtiva nova_ferramenta);
    void setCaminhoArquivo(QString& caminho);
    QString getCaminhoArquivo();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Circuito* m_circuito;
    FerramentaAtiva m_ferramenta_atual;
    bool m_arrastando_componente;
    Componente* m_componente_selecionado;
    QPoint m_offset_arraste;
    QPoint alinhar(QPoint& pos);
    bool m_desenhando_conexao;
    Componente* m_origem_conexao;
    QPoint m_preview_ponto_final;
    QPixmap m_cursor_ferramenta;
    QPoint m_posicao_mouse;
    QString m_caminhoArquivo;
};

#endif // CANVASWIDGET_H
