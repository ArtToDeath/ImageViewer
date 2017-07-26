#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    FileDlg(new QFileDialog()),
    gpi(NULL),
    gs(NULL),
    getCDlg(new GetCDialog),
    getKernelDlg(new GetKernelDialog),
    getGaussParaDlg(new GetGaussParaDialog),
    getBilaParaDlg(new GetBilateralDialog),
    medianSizeDlg(new MedianFilterDialog),
    expansionSizeDlg(new ExpansionDialog),
    corrosionSizeDlg(new CorrosionDialog),
    paddingDlg(new SettingsDialog),
    transMatrixDlg(new TransMatrixDialog),
    gaussFreqFilterDlg(new GaussFreqFiltersDialog)
{
    ui->setupUi(this);

    connect(getCDlg,SIGNAL(confirmed(double)),
            this,SLOT(getCFromDlg(double)));

    connect(getKernelDlg,SIGNAL(getKernelConfirmed(QString, bool)),
            this,SLOT(getKernelFromDlg(QString, bool)));

    connect(getGaussParaDlg,SIGNAL(getGaussParameter(int, double, bool)),
            this,SLOT(getParaFromGauss(int, double, bool)));

    connect(getBilaParaDlg,SIGNAL(getBilaParameter(int, double, double)),
            this,SLOT(getParaFromBila(int, double, double)));

    connect(medianSizeDlg,SIGNAL(medianParaConfirmed(int, bool)),
            this,SLOT(getMedianSize(int, bool)));

    connect(expansionSizeDlg,SIGNAL(expansionSizeConfirmed(int, bool)),
            this,SLOT(getExpansionSize(int, bool)));

    connect(corrosionSizeDlg,SIGNAL(corrosionSizeConfirmed(int, bool)),
            this,SLOT(getCorrosionSize(int, bool)));

    connect(transMatrixDlg, SIGNAL(matrixConfirmed(Matrix<double>)),
            this, SLOT(getQMatrixFromDlg(Matrix<double>)));

    connect(gaussFreqFilterDlg, SIGNAL(GFreqFParameter(double, bool)), this, SLOT(GaussFreqFilter(double, bool)));
}

MainWindow::~MainWindow()
{
    delete gpi;
    delete gs;
    delete FileDlg;
    delete getCDlg;
    delete getKernelDlg;
    delete getGaussParaDlg;
    delete getBilaParaDlg;
    delete medianSizeDlg;
    delete expansionSizeDlg;
    delete corrosionSizeDlg;
    delete paddingDlg;
    delete transMatrixDlg;
    delete ui;
}


/**
 * 打开图片
 */
void MainWindow::on_actionOpen_triggered()
{
    if(FileDlg->exec())
    {
        QStringList files = FileDlg->selectedFiles();
        QImage img = QImage(files[0]);
        QDir dir = FileDlg->directory();//获取当前目录

        //Returns a list of all the strings containing the substring str
        QStringList filters;
        filters<<"*.jpg"<<"*.png"<<"*.bmp"<<"*.tif"<<"*.gif";

        images = dir.entryInfoList(filters);//符合条件的文件加入images列表
        for(int i = 0; i < images.length(); i ++)
        {
            if(images[i].absoluteFilePath() == files[0])
            {
                currentIndex = i;//获得当前图片位置
                break;
            }
        }
        showImg(img);//显示当前选中图像
    }
}


/**
 * 显示图片
 */
void MainWindow::showImg(QImage img)
{
    if(NULL != gpi)
    {
        delete gpi;
        gpi = NULL;
    }
    if(NULL != gs)
    {
        delete gs;
        gs = NULL;
    }
    gpi = new MyMouseEvent();
    gpi->setPixmap(QPixmap::fromImage(img));

    gs = new QGraphicsScene(this);
    gs->addItem(gpi);//添加到画布上
    ui->graphicsView->setScene(gs);//显示到控件上

    reset();
}


/**
 * 图像复位
 */
void MainWindow::reset()
{
    if(NULL == gpi)
        return;
    gpi->reset();
    gs->setSceneRect(gpi->boundingRect());
    ui->graphicsView->fitInView(gpi->boundingRect(),Qt::KeepAspectRatio);
}
void MainWindow::on_actionReset_triggered()
{
    reset();
}


/**
 * 前后预览图像
 */
void MainWindow::on_actionLast_triggered()
{
    if(0 == images.length())
        return;
    if(currentIndex <= 0)//预览到第一张
        currentIndex = images.length();
    showImg(QImage(images[--currentIndex].absoluteFilePath()));//显示前一张
}
void MainWindow::on_actionNext_triggered()
{
    if(0 == images.length())
        return;
    if(currentIndex >= images.length() - 1)//预览到最后一张
        currentIndex = -1;
    showImg(QImage(images[++currentIndex].absoluteFilePath()));//显示后一张
}


/**
 * 获取当前预览图像
 */
bool MainWindow::getDisplayImage(QImage &img) const
{
    if(gpi)
    {
        QList<QGraphicsItem*> items =
                ui->graphicsView->scene()->items();
        QGraphicsPixmapItem *item =
                (QGraphicsPixmapItem*)items.at(0);
        img = item->pixmap().toImage();
        return true;
    }
    return false;
}


/**
 * rgb转灰度
 */
void MainWindow::on_actionRgb2Gray_triggered()
{
    Matrix<int> mat(3, 3, 2);
    mat(1,0)=4;mat(0,1)=5,mat(2,2)=3;
    bool m;
    cout<<"mat:\n"<<mat<<endl;
    Matrix<int> x = mat.inverted(mat, &m);
    if(m)
        cout<<"x:\n"<<x<<endl;
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::rgb2gray(img));
}


/**
 * 图像反转
 */
void MainWindow::on_actionReversal_triggered()
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::reversal(img));
}


/**
 * 对数变换
 */
void MainWindow::on_actionLogTrans_triggered()
{
    getCDlg->exec();
}
void MainWindow::getCFromDlg(double c)
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        //获得对话框传递过来的c，作为参数传给对数变换方法
        showImg(ImgProcessor::logTrans(img,c));
}


/**
 * 灰度直方图均衡
 */
void MainWindow::on_actionGray_HistEq_triggered()
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::grayHistEq(img));
}


/**
 * 彩色图像直方图均衡一
 */
void MainWindow::on_actionColor_HistEq01_triggered()
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::colorHistEq01(img));
}


/**
 * 彩色图像直方图均衡二
 */
void MainWindow::on_actionColor_HistEq02_triggered()
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::colorHistEq02(img));
}


/**
 * 频谱分析
 */
void MainWindow::on_actionSpectrum_Analysis_triggered()
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
    {
        Matrix<int> mat = Matrix<int>::fromQImage(img, 'y');
        size_t row = mat.getNRow();
        size_t col = mat.getNCol();
        Matrix<complex<double> > complexData = fft2d(mat, row, col);
        Matrix<double> data = Matrix<double>::transformationMat(complexData);
        Matrix<double>::fftshift(data);
        Matrix<int> resultData = Matrix<int>::normalizationMat(data);
        showImg(Matrix<int>::toQImage(resultData));
    }
}


/**
 * 线性滤波
 */
void MainWindow::on_actionLinear_triggered()
{
    getKernelDlg->exec();
}
void MainWindow::getKernelFromDlg(QString kernel, bool isGray)
{
    int nCol;
    vector<double> ret = parseMatrix2(kernel, nCol);

    if(-1 == nCol) return;

    Matrix<double> mat(ret.size() / nCol, nCol, 0.0);//用Matrix表示矩阵
    int k = 0;
    for(size_t i = 0; i < mat.getNRow(); i ++)
    {
        for(size_t j = 0; j < mat.getNCol(); j ++)
        {
            mat(i, j) = ret[k ++];
        }
    }
    ImgProcessor::kernelNomalization(mat);

    cout<<"mat:\n"<<mat<<endl;

    //获得填充方式
    paddingParameter = paddingDlg->getPaddingSet();

    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::linear(img, mat, isGray, paddingParameter));
}
vector<double> MainWindow::parseMatrix2(QString txt, int &nCol)
{
    vector<double> ret;
    QRegularExpression reRow("[-\\d\\.\\s,]+;?");
    QRegularExpression reCol("-?\\d*\\.?\\d+[,|\\s]?");
    QRegularExpressionMatchIterator remiRow = reRow.globalMatch(txt);
    nCol = -1;
    while(remiRow.hasNext()) {
        QString row = remiRow.next().captured().replace(";","").trimmed();
        if (row.isEmpty()) continue;
        QRegularExpressionMatchIterator remiCol = reCol.globalMatch(row);
        int tmp = 0;
        while(remiCol.hasNext()) {
            QString col = remiCol.next().captured().replace(",","").trimmed();
            if (col.isEmpty()) continue;
            bool ok;
            double d = col.toDouble(&ok);
            if(ok) {
                ret.push_back(d);
                tmp++;
            }
            else
                return vector<double>();
        }
        if(-1 == nCol) nCol = tmp;
        else if (nCol != tmp) return vector<double>();
    }
    return ret;
}


/**
 * 高斯滤波
 */
void MainWindow::on_actionGauss_Blur_triggered()
{
    getGaussParaDlg->exec();
}
void MainWindow::getParaFromGauss(int size, double sigma, bool isGray)
{
    cout<<"Gauss--->size: "<<size<<" sigma: "<<sigma<<endl;
    paddingParameter = paddingDlg->getPaddingSet();
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::gaussBlur(img, size, sigma, isGray, paddingParameter));
}


/**
 * 双边滤波
 */
void MainWindow::on_actionBilateral_Filter_triggered()
{
    getBilaParaDlg->exec();
}
void MainWindow::getParaFromBila(int size, double sigma, double colorSigma)
{
    cout<<"Bilateral--->size: "<<size<<" sigma: "<<sigma<<" colorSigma: "<<colorSigma<<endl;
    paddingParameter = paddingDlg->getPaddingSet();
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::bilateralFilter(img, size, sigma, colorSigma, paddingParameter));
}


/**
 * 中值滤波
 */
void MainWindow::on_actionMedian_Filter_triggered()
{
    medianSizeDlg->exec();
}
void MainWindow::getMedianSize(int size, bool isGray)
{
    paddingParameter = paddingDlg->getPaddingSet();
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::medianFilter(img, size, isGray, paddingParameter));
}


/**
 * 膨胀
 */
void MainWindow::on_actionExpansion_triggered()
{
    expansionSizeDlg->exec();
}
void MainWindow::getExpansionSize(int size, bool isGray)
{
    paddingParameter = paddingDlg->getPaddingSet();
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::expansion(img, size, isGray, paddingParameter));
}


/**
 * 腐蚀
 */
void MainWindow::on_actionCorrosion_triggered()
{
    corrosionSizeDlg->exec();
}
void MainWindow::getCorrosionSize(int size, bool isGray)
{
    paddingParameter = paddingDlg->getPaddingSet();
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::corrosion(img, size, isGray, paddingParameter));
}


/**
 * 填充方式对话框
 */
void MainWindow::on_actionSettings_triggered()
{
    paddingDlg->exec();
}

/**
 * 退出程序
 */
void MainWindow::on_actionExit_triggered()
{
    this->close();
}


/**
 * 四则基本变换
 */
void MainWindow::on_actionBasic_Transformation_triggered()
{
    transMatrixDlg->exec();
}
void MainWindow::getQMatrixFromDlg(Matrix<double> mat)
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::transform(img, mat));
}

/**
 * 保存图片
 */
void MainWindow::on_actionSave_triggered()
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
    {
        QString filename = QFileDialog::getSaveFileName(this,tr("保存图片到"),"",tr("Images(*.jpg *.png *.bmp)"));
        if(filename.isEmpty())
        {
            return;
        }
        else
        {
            if(!img.save(filename))
            {
                QMessageBox::information(this,tr("保存时出错"),tr("保存失败!"));
                return;
            }
            else
                QMessageBox::information(this,tr("成功保存"),tr("已成功保存图片"));
        }
    }
}

void MainWindow::GaussFreqFilter(double sigma, bool isLP)
{
    QImage img;
    bool ok;
    ok = getDisplayImage(img);
    if(ok)
        showImg(ImgProcessor::GaussFrequecy(img, sigma, isLP));
}

void MainWindow::on_actionGaussFrequency_filtering_triggered()
{
    gaussFreqFilterDlg->exec();
}
