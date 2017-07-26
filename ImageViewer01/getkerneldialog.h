#ifndef GETKERNELDIALOG_H
#define GETKERNELDIALOG_H

#include <QDialog>

namespace Ui {
class GetKernelDialog;
}

class GetKernelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetKernelDialog(QWidget *parent = 0);
    ~GetKernelDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GetKernelDialog *ui;

signals:
    void getKernelConfirmed(QString, bool);
};

#endif // GETKERNELDIALOG_H
