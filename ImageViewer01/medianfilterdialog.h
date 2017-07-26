#ifndef MEDIANFILTERDIALOG_H
#define MEDIANFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class MedianFilterDialog;
}

class MedianFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedianFilterDialog(QWidget *parent = 0);
    ~MedianFilterDialog();

private:
    Ui::MedianFilterDialog *ui;
signals:
    void medianParaConfirmed(int, bool);
private slots:
    void on_buttonBox_accepted();
};

#endif // MEDIANFILTERDIALOG_H
