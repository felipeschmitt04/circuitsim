#ifndef AJUDADIALOG_H
#define AJUDADIALOG_H

#include <QDialog>

namespace Ui {
class AjudaDialog;
}

class AjudaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AjudaDialog(QWidget *parent = nullptr);
    ~AjudaDialog();

private:
    Ui::AjudaDialog *ui;
};

#endif // AJUDADIALOG_H
