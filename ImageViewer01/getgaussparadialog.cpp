#include "getgaussparadialog.h"
#include "ui_getgaussparadialog.h"

/**
 * 高斯滤波
 */

GetGaussParaDialog::GetGaussParaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetGaussParaDialog)
{
    ui->setupUi(this);
}

GetGaussParaDialog::~GetGaussParaDialog()
{
    delete ui;
}

void GetGaussParaDialog::on_buttonBox_accepted()
{
    bool ok;
    int size = ui->sizeLineEdit->text().toInt(&ok);
    double sigma = ui->sigmaLineEdit->text().toDouble(&ok);
    bool isGray = true;
    if(ui->gaussGrayRBtn->isChecked()) isGray = true;
    else if(ui->gaussRgbRBtn->isChecked()) isGray = false;
    if(ok)
        emit getGaussParameter(size, sigma, isGray);
}
