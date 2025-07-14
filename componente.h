#ifndef COMPONENTE_H
#define COMPONENTE_H

#include <QPoint>
#include <QString>
#include <vector>
#include <QPixmap>

using namespace std;

class QPainter;

class Componente{
protected:
    int m_id;
    QPoint m_posicao;
    QString m_nome;
    bool m_estado_saida;
    QPixmap m_imagem;
public:
    Componente(const QString& nome, QPoint pos, int id);
    virtual ~Componente() = default;

    virtual void desenhar(QPainter* painter) = 0;
    virtual void CalcularSaida() = 0;

    int getId() const;
    QString& getNome();
    QPoint getPosicao();
    void setPosicao(const QPoint& pos);
    virtual bool getEstadoSaida();

    virtual QPoint getPosicaoPinoEntrada(int pino =0) const;
    virtual QPoint getPosicaoPinoSaida();

    virtual QString getTipoString() const = 0;
};

class PortaLogica : public Componente{
protected:
    int m_num_entradas;
    vector<Componente*> m_entradas;
public:
    PortaLogica(const QString& nome, QPoint pos, int id, int num_entradas);

    void adicionarEntrada(Componente *componente, int pino);
    int getNumeroEntradas() const;
    void liberarPinoDeEntrada(int pino);
    int getPinoEntradaMaisProximo(const QPoint& pos_mouse) const;
    bool pinoEstaOcupado(int pino) const;

    QPoint getPosicaoPinoEntrada(int pino) const;
};

class PortaAND : public PortaLogica{
public:
    PortaAND(QPoint pos, int id);

    void desenhar(QPainter* painter);
    void CalcularSaida();
    QString getTipoString() const;
};

class PortaOR : public PortaLogica{
public:
    PortaOR(QPoint pos, int id);

    void desenhar(QPainter* painter);
    void CalcularSaida();
    QString getTipoString() const;
};

class PortaNOT : public PortaLogica{
public:
    PortaNOT(QPoint pos, int id);

    void desenhar(QPainter* painter);
    void CalcularSaida();
    QString getTipoString() const;
};

class Input : public Componente{
public:
    Input(QPoint pos, int id);

    void desenhar(QPainter* painter);
    void CalcularSaida();
    void alternarEstado();
    QString getTipoString() const;
};

class Output : public Componente{
private:
    Componente* m_entrada;
public:
    Output(QPoint pos, int id);

    void desenhar(QPainter* painter);
    void CalcularSaida();
    void setEntrada(Componente* componente);
    QString getTipoString() const;
    bool isOcupado() const;
    Componente* getEntrada() const;
};

#endif // COMPONENTE_H
