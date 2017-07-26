#include "getbilateraldialog.h"
#include "ui_getbilateraldialog.h"

/**
 * 双边滤波
 */

GetBilateralDialog::GetBilateralDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetBilateralDialog)
{
    ui->setupUi(this);
}

GetBilateralDialog::~GetBilateralDialog()
{
    delete ui;
}

void GetBilateralDialog::on_buttonBox_accepted()
{
    bool ok;
    int size = ui->bilaSizeLineEdit->text().toInt(&ok);
    double sigma = ui->bilaSigmaLineEdit->text().toDouble(&ok);
    double colorSigma = ui->bilaColorSigmaLineEdit->text().toDouble(&ok);
    if(ok)
        emit getBilaParameter(size, sigma, colorSigma);
}
