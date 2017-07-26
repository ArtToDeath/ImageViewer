#ifndef GETGAUSSPARADIALOG_H
#define GETGAUSSPARADIALOG_H

#include <QDialog>

namespace Ui {
class GetGaussParaDialog;
}

class GetGaussParaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetGaussParaDialog(QWidget *parent = 0);
    ~GetGaussParaDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GetGaussParaDialog *ui;

signals:
    void getGaussParameter(int size, double sigma, bool isGray);
};

#endif // GETGAUSSPARADIALOG_H
