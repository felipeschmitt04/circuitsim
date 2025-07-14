#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString qss = R"(
    QMainWindow, QDialog {
        background-color: #282c34;
    }

    QToolBar {
        background-color: #2c313a;
        border: none;
        padding: 2px;
    }

    QToolButton {
        border: none;
        background-color: transparent;
        padding: 5px;
        margin: 2px;
    }
    QToolButton:hover {
        background-color: #3a3f4b;
        border-radius: 4px;
    }
    QToolButton:checked {
        background-color: #528bff;
        border-radius: 4px;
    }

    QTabWidget::pane {
        border: 1px solid #333740;
    }
    QTabBar::tab {
        background: #282c34;
        color: #ecf0f1;
        padding: 8px;
        border: 1px solid #333740;
        border-bottom: none;
        border-top-left-radius: 4px;
        border-top-right-radius: 4px;
    }
    QGroupBox#groupBox {
        background-color: #2c313a;
        border: 1px solid #333740;
        border-radius: 4px;
        margin-top: 15px;
    }
    QGroupBox::title {
        color: #ecf0f1;
        background-color: #528bff;
        border-radius: 4px;
        subcontrol-origin: margin;
        subcontrol-position: top center;
        padding: 2px 10px;
    }
    QTabBar::tab:selected {
        background: #2c313a;
        margin-bottom: -1px;
    }
    QTabBar::tab:!selected:hover {
        background: #3a3f4b;
    }

    QMenuBar, QMenu {
        background-color: #2c313a;
        color: #ecf0f1;
    }
    QMenuBar::item:selected, QMenu::item:selected {
        background-color: #528bff;
    }
    QToolTip {
        background-color: #212121;
        color: #ecf0f1;
        border: 1px solid #333740;
    }

    QLabel, QCheckBox {
        color: #ecf0f1;
    }
    QToolBox::tab {
      background-color: #2c313a;
      color: #ecf0f1;
       border: 1px solid #333740;
       border-top-left-radius: 4px;
       border-top-right-radius: 4px;
       padding: 5px;
    }

    QToolBox::tab:hover {
      background-color: #3a3f4b;
    }

    QToolBox::tab:selected {
       background-color: #528bff;
       font-weight: bold;
       color: white;
    }
    QHeaderView::section {
       background-color: #2c313a;
       color: #ecf0f1;
       padding: 4px;
       border: 1px solid #4a5b6a;
       font-weight: bold;
    }
)";
    a.setStyleSheet(qss);

    MainWindow w;
    w.show();
    return a.exec();
}
