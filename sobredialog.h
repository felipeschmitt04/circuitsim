#ifndef SOBREDIALOG_H
#define SOBREDIALOG_H

#include <QDialog>

namespace Ui {
class SobreDialog;
}

class SobreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SobreDialog(QWidget *parent = nullptr);
    ~SobreDialog();

private:
    Ui::SobreDialog *ui;
};

#endif // SOBREDIALOG_H
