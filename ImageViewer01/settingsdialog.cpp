#include "settingsdialog.h"
#include "ui_settingsdialog.h"

/**
 * 选择填充方式
 */

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    c = 'z';
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    //点击对话框的确认按钮后，判断选择了哪一种填充方式，相应修改变量c
    if(ui->paddedZeroRBtn->isChecked())
        c = 'z';//补零填充
    else if(ui->repeatRBtn->isChecked())
        c = 'r';//重复填充
    else if(ui->mirrorRBtn->isChecked())
        c = 'm';//镜像填充
}

char SettingsDialog::getPaddingSet()
{
    return c;//返回这个变量值
}
