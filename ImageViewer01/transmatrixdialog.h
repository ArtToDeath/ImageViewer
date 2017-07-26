#ifndef TRANSMATRIXDIALOG_H
#define TRANSMATRIXDIALOG_H

#include <QDialog>
#include "fft.h"

namespace Ui {
class TransMatrixDialog;
}

class TransMatrixDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransMatrixDialog(QWidget *parent = 0);
    ~TransMatrixDialog();

private:
    Ui::TransMatrixDialog *ui;

signals:
    void matrixConfirmed(Matrix<double>);
private slots:
    void on_buttonBox_accepted();
};

#endif // TRANSMATRIXDIALOG_H
