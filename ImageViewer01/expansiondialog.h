#ifndef EXPANSIONDIALOG_H
#define EXPANSIONDIALOG_H

#include <QDialog>

namespace Ui {
class ExpansionDialog;
}

class ExpansionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExpansionDialog(QWidget *parent = 0);
    ~ExpansionDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ExpansionDialog *ui;
signals:
    void expansionSizeConfirmed(int, bool);
};

#endif // EXPANSIONDIALOG_H
