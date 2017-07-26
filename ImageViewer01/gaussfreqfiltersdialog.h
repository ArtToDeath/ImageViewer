#ifndef GAUSSFREQFILTERSDIALOG_H
#define GAUSSFREQFILTERSDIALOG_H

#include <QDialog>

namespace Ui {
class GaussFreqFiltersDialog;
}

class GaussFreqFiltersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GaussFreqFiltersDialog(QWidget *parent = 0);
    ~GaussFreqFiltersDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GaussFreqFiltersDialog *ui;

signals:
    void GFreqFParameter(double, bool);
};

#endif // GAUSSFREQFILTERSDIALOG_H
