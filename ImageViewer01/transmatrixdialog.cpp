#include "transmatrixdialog.h"
#include "ui_transmatrixdialog.h"

TransMatrixDialog::TransMatrixDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransMatrixDialog)
{
    ui->setupUi(this);
}

TransMatrixDialog::~TransMatrixDialog()
{
    delete ui;
}

void TransMatrixDialog::on_buttonBox_accepted()
{
    Matrix<double> mat(3, 3, 0.0);
    bool ok;
    double m11 = ui->lineEdit_1->text().toDouble(&ok);
    double m12 = ui->lineEdit_2->text().toDouble(&ok);
    double m21 = ui->lineEdit_4->text().toDouble(&ok);
    double m22 = ui->lineEdit_5->text().toDouble(&ok);
    double dx = ui->lineEdit_7->text().toDouble(&ok);
    double dy = ui->lineEdit_8->text().toDouble(&ok);
    if(ok)
    {
        mat(0, 0) = m11;
        mat(0, 1) = m12;
        mat(1, 0) = m21;
        mat(1, 1) = m22;
        mat(2, 0) = dx;
        mat(2, 1) = dy;
        mat(2, 2) = 1;
        emit matrixConfirmed(mat);
    }
}
