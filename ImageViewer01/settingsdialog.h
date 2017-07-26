#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    char getPaddingSet();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingsDialog *ui;
    char c;
};

#endif // SETTINGSDIALOG_H
