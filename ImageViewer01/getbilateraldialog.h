#ifndef GETBILATERALDIALOG_H
#define GETBILATERALDIALOG_H

#include <QDialog>

namespace Ui {
class GetBilateralDialog;
}

class GetBilateralDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetBilateralDialog(QWidget *parent = 0);
    ~GetBilateralDialog();

private:
    Ui::GetBilateralDialog *ui;

signals:
    void getBilaParameter(int, double, double);
private slots:
    void on_buttonBox_accepted();
};

#endif // GETBILATERALDIALOG_H
