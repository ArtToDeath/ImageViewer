#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QDir>
#include <QFileInfoList>

#include "mymouseevent.h"
#include "imgprocessor.h"

#include "getcdialog.h"
#include "getkerneldialog.h"
#include "getgaussparadialog.h"
#include "getbilateraldialog.h"
#include "medianfilterdialog.h"
#include "expansiondialog.h"
#include "corrosiondialog.h"
#include "settingsdialog.h"
#include "transmatrixdialog.h"

#include "gaussfreqfiltersdialog.h"

//#include "fft.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionReset_triggered();

    void on_actionLast_triggered();

    void on_actionNext_triggered();

    void on_actionRgb2Gray_triggered();

    void on_actionReversal_triggered();

    void on_actionLogTrans_triggered();
    void getCFromDlg(double c);

    void on_actionGray_HistEq_triggered();

    void on_actionColor_HistEq01_triggered();

    void on_actionColor_HistEq02_triggered();

    void on_actionLinear_triggered();
    void getKernelFromDlg(QString, bool);

    void on_actionGauss_Blur_triggered();
    void getParaFromGauss(int, double, bool);

    void on_actionBilateral_Filter_triggered();
    void getParaFromBila(int, double, double);

    void on_actionMedian_Filter_triggered();
    void getMedianSize(int, bool);

    void on_actionExpansion_triggered();
    void getExpansionSize(int, bool);

    void on_actionCorrosion_triggered();
    void getCorrosionSize(int, bool);

    void on_actionSettings_triggered();

    void on_actionExit_triggered();

    void on_actionSpectrum_Analysis_triggered();

    void on_actionBasic_Transformation_triggered();
    void getQMatrixFromDlg(Matrix<double>);

    void on_actionSave_triggered();

    void GaussFreqFilter(double, bool);

    void on_actionGaussFrequency_filtering_triggered();

private:
    Ui::MainWindow *ui;

    QFileDialog *FileDlg;
    MyMouseEvent *gpi;
    QGraphicsScene *gs;

    QFileInfoList images;
    int currentIndex;
    char paddingParameter;

    GetCDialog* getCDlg;
    GetKernelDialog* getKernelDlg;
    GetGaussParaDialog* getGaussParaDlg;
    GetBilateralDialog* getBilaParaDlg;
    MedianFilterDialog* medianSizeDlg;
    ExpansionDialog* expansionSizeDlg;
    CorrosionDialog* corrosionSizeDlg;
    SettingsDialog* paddingDlg;
    TransMatrixDialog* transMatrixDlg;

    GaussFreqFiltersDialog* gaussFreqFilterDlg;

    void showImg(QImage);
    void reset();

    bool getDisplayImage(QImage& img) const;

    vector<double> parseMatrix2(QString txt, int& nCol);
};

#endif // MAINWINDOW_H
