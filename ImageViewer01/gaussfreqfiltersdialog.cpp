#include "gaussfreqfiltersdialog.h"
#include "ui_gaussfreqfiltersdialog.h"

GaussFreqFiltersDialog::GaussFreqFiltersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussFreqFiltersDialog)
{
    ui->setupUi(this);
}

GaussFreqFiltersDialog::~GaussFreqFiltersDialog()
{
    delete ui;
}

void GaussFreqFiltersDialog::on_buttonBox_accepted()
{
    bool ok;
    double sigma = ui->SigmaGFEt->text().toDouble(&ok);
    bool isLP = true;
    if(ui->LPRadioBtn->isChecked()) isLP = true;
    else if(ui->HPRadioBtn->isChecked()) isLP = false;
    if(ok)
        emit GFreqFParameter(sigma, isLP);
}
