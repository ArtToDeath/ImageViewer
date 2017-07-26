#include "getcdialog.h"
#include "ui_getcdialog.h"

/**
 * 对数变换
 */

GetCDialog::GetCDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetCDialog)
{
    ui->setupUi(this);
}

GetCDialog::~GetCDialog()
{
    delete ui;
}

void GetCDialog::on_buttonBox_accepted()
{
    bool ok;

    //获得输入的参数c
    double c = ui->lineEdit_c->text().toDouble(&ok);
    if(ok)
        emit confirmed(c);//传递参数c
}
