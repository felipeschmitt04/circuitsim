#include "ajudadialog.h"
#include "ui_ajudadialog.h"

AjudaDialog::AjudaDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AjudaDialog)
{
    ui->setupUi(this);
    ui->toolBox->setCurrentIndex(0);
}

AjudaDialog::~AjudaDialog()
{
    delete ui;
}
