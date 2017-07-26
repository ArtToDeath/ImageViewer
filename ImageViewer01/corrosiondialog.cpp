#include "corrosiondialog.h"
#include "ui_corrosiondialog.h"

/**
 * 腐蚀
 */

CorrosionDialog::CorrosionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CorrosionDialog)
{
    ui->setupUi(this);
}

CorrosionDialog::~CorrosionDialog()
{
    delete ui;
}

void CorrosionDialog::on_buttonBox_accepted()
{
    bool ok;
    bool isGray = true;
    int size = ui->corrosionLEd->text().toInt(&ok);
    if(ui->corrosionGrayRBtn->isChecked()) isGray = true;
    else if(ui->corrosionRgbRBtn->isChecked()) isGray = false;
    if(ok)
        emit corrosionSizeConfirmed(size, isGray);
}
