#include "expansiondialog.h"
#include "ui_expansiondialog.h"

/**
 * 膨胀
 */

ExpansionDialog::ExpansionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExpansionDialog)
{
    ui->setupUi(this);
}

ExpansionDialog::~ExpansionDialog()
{
    delete ui;
}

void ExpansionDialog::on_buttonBox_accepted()
{
    bool ok;
    bool isGray = true;
    int size = ui->expansionLEd->text().toInt(&ok);
    if(ui->expansionGrayRBtn->isChecked()) isGray = true;
    else if(ui->expansionRgbRbtn->isChecked()) isGray = false;
    if(ok)
        emit expansionSizeConfirmed(size, isGray);
}
