#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "circuito.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Circuito* getCircuitoAtivo() const;

private slots:
    void on_actionNew_triggered();

    void on_actionClose_tab_triggered();

    void on_actionSave_triggered();

    void on_actionSalvar_como_triggered();

    void on_actionOpen_triggered();

    void onToolButtonClicked(int id);

    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QButtonGroup* m_grupoFerramentas;
};
#endif // MAINWINDOW_H
