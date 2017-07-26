#ifndef CORROSIONDIALOG_H
#define CORROSIONDIALOG_H

#include <QDialog>

namespace Ui {
class CorrosionDialog;
}

class CorrosionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CorrosionDialog(QWidget *parent = 0);
    ~CorrosionDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CorrosionDialog *ui;
signals:
    void corrosionSizeConfirmed(int, bool);
};

#endif // CORROSIONDIALOG_H
