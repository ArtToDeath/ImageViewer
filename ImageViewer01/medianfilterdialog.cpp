#include "medianfilterdialog.h"
#include "ui_medianfilterdialog.h"

/**
 * 中值滤波
 */

MedianFilterDialog::MedianFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MedianFilterDialog)
{
    ui->setupUi(this);
}

MedianFilterDialog::~MedianFilterDialog()
{
    delete ui;
}

void MedianFilterDialog::on_buttonBox_accepted()
{
    bool ok;
    bool isGray;
    int size = ui->MedianFilterLEd->text().toInt(&ok);
    if(ui->medianGrayRBtn->isChecked()) isGray = true;
    else if(ui->medianRgbRBtn->isChecked()) isGray = false;
    if(ok)
        emit medianParaConfirmed(size, isGray);
}
