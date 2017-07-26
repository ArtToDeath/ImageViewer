#include "getkerneldialog.h"
#include "ui_getkerneldialog.h"

/**
 * 双边滤波
 */

GetKernelDialog::GetKernelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetKernelDialog)
{
    ui->setupUi(this);
}

GetKernelDialog::~GetKernelDialog()
{
    delete ui;
}

void GetKernelDialog::on_buttonBox_accepted()
{
    bool isGray = true;
    if(ui->linearGrayRBtn->isChecked()) isGray = true;
    else if(ui->linearRgbRBtn->isChecked()) isGray = false;
    emit getKernelConfirmed(ui->kernelTextEdit->toPlainText(), isGray);
}
