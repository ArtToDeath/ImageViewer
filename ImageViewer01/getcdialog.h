#ifndef GETCDIALOG_H
#define GETCDIALOG_H

#include <QDialog>

namespace Ui {
class GetCDialog;
}

class GetCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetCDialog(QWidget *parent = 0);
    ~GetCDialog();

private:
    Ui::GetCDialog *ui;
signals:
    void confirmed(double c);
private slots:
    void on_buttonBox_accepted();
};

#endif // GETCDIALOG_H
