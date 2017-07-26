#include "imgprocessor.h"
#include<iostream>
using namespace std;

/**
 * 图像处理
 */

ImgProcessor::ImgProcessor()
{

}
/**
 * rgb转灰度
 */
QImage ImgProcessor::rgb2gray(const QImage &img)
{
    QImage ret(img);
    ret = Matrix<int>::toQImage(Matrix<int>::fromQImage(img, 'y'));
    return ret;

}

/**
 * 图像反转
 */
QImage ImgProcessor::reversal(const QImage &img)
{
    QImage ret(img);
    Matrix<int> rMat = 255 - Matrix<int>::fromQImage(img, 'r');
    Matrix<int> gMat = 255 - Matrix<int>::fromQImage(img, 'g');
    Matrix<int> bMat = 255 - Matrix<int>::fromQImage(img, 'b');
    ret = rMat.toQImage(rMat, gMat, bMat);
    return ret;
}

/**
 * 对数变换
 */
QImage ImgProcessor::logTrans(const QImage &img, double c)
{
    Matrix<int> grayMat = Matrix<int>::fromQImage(img, 'y');
    Matrix<int> logGrayMat = grayMat;
    for(size_t i = 0; i < grayMat.getNRow(); i ++)
    {
        for(size_t j = 0; j < grayMat.getNCol(); j ++)
        {
            logGrayMat(i, j) = (c * log(1 + grayMat(i, j) / 255.0)) * 255;
            if(logGrayMat(i, j) > 255) logGrayMat(i, j) = 255;
        }
    }
    return logGrayMat.toQImage(logGrayMat);
}

/**
 * 灰度直方图均衡
 */
QImage ImgProcessor::grayHistEq(const QImage &img)
{
    QImage ret(img);
    int width = img.width();
    int height = img.height();
    int N = width * height;


    //统计灰度级直方图
    int hist[256];
    std::fill(hist, hist + 256, 0);
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            hist[qGray(img.pixel(i, j))] ++;
        }
    }

    //直方图均衡，得到映射关系
    int map[256];
    double sum = 0;
    for(int i = 0; i < 256; i ++)
    {
        sum += hist[i];
        map[i] = round(sum / N * 255);
    }

    //将当前像素的灰度值映射到新的灰度值上去
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            int g = map[qGray(img.pixel(i, j))];
            ret.setPixel(i, j, qRgb(g, g, g));
        }
    }
    return ret;
}

/**
 * 彩色图像直方图均衡一
 */
QImage ImgProcessor::colorHistEq01(const QImage &img)
{
    QImage ret(img);
    int width = img.width();
    int height = img.height();
    int N = width * height;


    //直方图统计
    int hist[3][256];
    std::fill(hist[0], hist[0] + 3 * 256, 0);
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            //分别对RGB三个通道进行直方图统计
            hist[0][qRed(img.pixel(i, j))] ++;
            hist[1][qGreen(img.pixel(i, j))] ++;
            hist[2][qBlue(img.pixel(i, j))] ++;
        }
    }

    //直方图均衡
    int map[3][256];
    double sumR = 0;
    double sumG = 0;
    double sumB = 0;
    for(int i = 0; i < 256; i ++)
    {
        //分别对三个通道进行均衡
        sumR += hist[0][i];
        map[0][i] = round(sumR / N * 255);
        sumG += hist[1][i];
        map[1][i] = round(sumG / N * 255);
        sumB += hist[2][i];
        map[2][i] = round(sumB / N * 255);
    }

    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            int r = map[0][qRed(img.pixel(i ,j))];
            int g = map[1][qGreen(img.pixel(i, j))];
            int b = map[2][qBlue(img.pixel(i, j))];
            ret.setPixel(i, j, qRgb(r, g, b));
        }
    }
    return ret;
}

/**
 * 彩色图像直方图均衡二
 */
QImage ImgProcessor::colorHistEq02(const QImage &img)
{
    QImage ret(img);
    int width = ret.width();
    int height = ret.height();
    int N = width * height;

    //RGB2HSI转换到HSI颜色空间上
    HSI* Hsi = new HSI[N];
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            QRgb rgb = img.pixel(i, j);
            RGB2HSI(rgb, Hsi[i * height + j]);
        }
    }

    //对I分量进行直方图统计
    int hist[256];
    std::fill(hist, hist + 256, 0);
    for(int i = 0; i < N; i ++)
    {
        hist[int(Hsi[i].Intensity * 255)] ++;
    }

    //建立映射关系
    int map[256];
    double sum = 0;
    for(int i = 0; i < 256; i ++)
    {
        sum += hist[i];
        map[i] = round(sum / N * 255);
    }

    //得到新的I分量
    for(int i = 0; i < N; i ++)
    {
        //I分量的范围为0~1
        Hsi[i].Intensity = map[int(Hsi[i].Intensity * 255)] / 255.0;
    }

    //HSI2RGB转换回RGB颜色空间上
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            QRgb rgb;
            HSI2RGB(rgb, Hsi[i * height + j]);
            ret.setPixel(i, j, rgb);
        }
    }

    delete Hsi;

    return ret;
}
void ImgProcessor::RGB2HSI(QRgb &rgb, HSI &hsi)
{
    int R = qRed(rgb), G = qGreen(rgb), B = qBlue(rgb);
    double h, s, i;
    double var_R = ( R / 255.0 );                     //RGB from 0 to 255
    double var_G = ( G / 255.0 );
    double var_B = ( B / 255.0 );

    double var_Min = std::min( var_R, std::min(var_G, var_B) );    //Min. value of RGB
    double var_Max = std::max( var_R, std::max(var_G, var_B) );    //Max. value of RGB
    double del_Max = var_Max - var_Min;             //Delta RGB value

    i = ( var_Max + var_Min ) / 2;

    if ( del_Max == 0 )                     //This is a gray, no chroma...
    {
       h = 0;                                //HSL results from 0 to 1
       s = 0;
    }
    else                                    //Chromatic data...
    {
       if ( i < 0.5 ) s = del_Max / ( var_Max + var_Min );
       else           s = del_Max / ( 2 - var_Max - var_Min );

       double del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max;
       double del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max;
       double del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max;

       if      ( var_R == var_Max ) h = del_B - del_G;
       else if ( var_G == var_Max ) h = ( 1 / 3.0 ) + del_R - del_B;
       else if ( var_B == var_Max ) h = ( 2 / 3.0 ) + del_G - del_R;

       if ( h < 0 ) h += 1;
       if ( h > 1 ) h -= 1;
    }
    hsi.Hue = h;
    hsi.Intensity = i;
    hsi.Saturation = s;
}
void ImgProcessor::HSI2RGB(QRgb &rgb, HSI &hsi)
{
    int r, g, b;
    if ( hsi.Saturation == 0 )                       //HSL from 0 to 1
    {
       r = int(hsi.Intensity * 255);                      //RGB results from 0 to 255
       g = int(hsi.Intensity * 255);
       b = int(hsi.Intensity * 255);
    }
    else
    {
        double var_2 = 0;
        if ( hsi.Intensity < 0.5 ) var_2 = hsi.Intensity * ( 1 + hsi.Saturation );
        else           var_2 = ( hsi.Intensity + hsi.Saturation ) - ( hsi.Saturation * hsi.Intensity );

        double var_1 = 2 * hsi.Intensity - var_2;

        r = int(255 * Hue_2_RGB( var_1, var_2, hsi.Hue + ( 1 / 3.0 ) ));
        g = int(255 * Hue_2_RGB( var_1, var_2, hsi.Hue ));
        b = int(255 * Hue_2_RGB( var_1, var_2, hsi.Hue - ( 1 / 3.0 ) ));
    }
    rgb = qRgb(r, g, b);

}
double ImgProcessor::Hue_2_RGB(double v1, double v2, double vH)  //Function Hue_2_RGB
{
    if ( vH < 0 ) vH += 1;
    if ( vH > 1 ) vH -= 1;
    if ( ( 6 * vH ) < 1 ) return v1 + ( v2 - v1 ) * 6 * vH;
    if ( ( 2 * vH ) < 1 ) return v2;
    if ( ( 3 * vH ) < 2 ) return v1 + ( v2 - v1 ) * ( ( 2 / 3.0 ) - vH ) * 6;
    return v1;
}

/**
 * 矩阵归一化函数
 */
void ImgProcessor::kernelNomalization(Matrix<double> &kernel)
{
    double sum = 0;
    for(size_t i = 0; i < kernel.getNRow(); i ++)
    {
        for(size_t j = 0; j < kernel.getNCol(); j ++)
        {
            sum += kernel(i, j);
        }
    }
    if(0 != sum)
    {
        for(size_t i = 0; i < kernel.getNRow(); i ++)
        {
            for(size_t j = 0; j < kernel.getNCol(); j ++)
            {
                kernel(i, j) /= sum;
            }
        }
    }
}

/**
 * 线性滤波
 * 填充--滤波--裁剪
 * 参数 isGray 在灰度或rgb通道处理
 */
QImage ImgProcessor::linear(const QImage &img, Matrix<double> &kernel, bool isGray, char c)
{
    size_t nRow = kernel.getNRow();
    size_t nCol = kernel.getNCol();
    if(0 == nRow % 2 || 0 == nCol % 2)
        return img;
    QImage paddingImg = padding(img, nCol, nRow, c);//填充
    QImage linearImg = filterLinear(paddingImg, kernel, isGray);//滤波

    return linearImg;
}

/**
 * 选择填充方式
 */
QImage ImgProcessor::padding(const QImage &img, int nCol, int nRow, char c)
{
    QImage padding;
    switch (c) {
    case 'z':
        padding = paddingZero(img, nCol, nRow);
        break;
    case 'r':
        padding = paddingRepeat(img, nCol, nRow);
        break;
    case 'm':
        padding = paddingMirror(img, nCol, nRow);
        break;
    default:
        break;
    }
    return padding;
}

/**
 * 填充方式：补零
 */
QImage ImgProcessor::paddingZero(const QImage &img, int nCol, int nRow)
{
    QImage imgPaddingZero = QImage(img.width() + nCol - 1, img.height() + nRow - 1, img.format());
    int width = imgPaddingZero.width();
    int height = imgPaddingZero.height();
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            if(j < nRow / 2 || j >= height - nRow / 2
                    || i < nCol / 2 || i >= width - nCol / 2)
                imgPaddingZero.setPixel(i, j, qRgb(0, 0, 0));//边缘像素填充零值
            else
                imgPaddingZero.setPixel(i, j, QRgb(img.pixel(i - nCol / 2, j - nRow / 2)));
        }
    }
    return imgPaddingZero;
}

/**
 * 填充方式：重复
 */
QImage ImgProcessor::paddingRepeat(const QImage &img, int nCol, int nRow)
{
    QImage imgPaddingRepeat = QImage(img.width() + nCol - 1, img.height() + nRow - 1, img.format());
    int width = imgPaddingRepeat.width();
    int height = imgPaddingRepeat.height();
    //先填充上下两边以及中间
    for(int i = nCol / 2; i < width - nCol / 2; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            if(j < nRow / 2)
                imgPaddingRepeat.setPixel(i, j,
                                          QRgb(img.pixel(i - nCol / 2, 0)));
            else if(j >= height - nRow / 2)
                imgPaddingRepeat.setPixel(i, j,
                                          QRgb(img.pixel(i - nCol / 2, height - nRow)));
            else
                imgPaddingRepeat.setPixel(i, j,
                                          QRgb(img.pixel(i - nCol / 2, j - nRow / 2)));
        }
    }
    //填充左边缘
    for(int i = 0; i < nCol / 2; i ++)
    {
        for(int j = 0; j < height; j ++)
            imgPaddingRepeat.setPixel(i, j,
                                      QRgb(imgPaddingRepeat.pixel(nCol / 2, j)));
    }
    //填充右边缘
    for(int i = width - nCol / 2; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
            imgPaddingRepeat.setPixel(i, j,
                                      QRgb(imgPaddingRepeat.pixel(width - nCol / 2 - 1, j)));
    }
    return imgPaddingRepeat;   
}

/**
 * 填充方式：镜像
 */
QImage ImgProcessor::paddingMirror(const QImage &img, int nCol, int nRow)
{
    QImage imgPaddingMirror = QImage(img.width() + nCol - 1, img.height() + nRow - 1, img.format());
    int width = imgPaddingMirror.width();
    int height = imgPaddingMirror.height();
    //镜像上下
    for(int i = nCol / 2; i < width - nCol / 2; i ++)
    {
        int mTemp = nRow / 2 - 1;
        int nTemp = height - nRow;
        for(int j = 0; j < height; j ++)
        {
            if(j < nRow / 2)
                imgPaddingMirror.setPixel(i, j,
                                          QRgb(img.pixel(i - nCol / 2, mTemp --)));
            else if(j >= height - nRow / 2)
                imgPaddingMirror.setPixel(i, j,
                                          QRgb(img.pixel(i - nCol / 2, nTemp --)));
            else
                imgPaddingMirror.setPixel(i, j,
                                          QRgb(img.pixel(i - nCol / 2, j - nRow / 2)));
        }
    }
    //镜像左边
    int mTemp = nCol / 2;
    for(int i = nCol / 2; i >= 0; i --)
    {
        //由里圈往外圈
        for(int j = 0; j < height; j ++)
            imgPaddingMirror.setPixel(i, j,
                                      QRgb(imgPaddingMirror.pixel(mTemp, j)));
        mTemp ++;
    }
    //镜像右边
    int nTemp = nCol / 2;
    for(int i = width - nCol / 2; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
            imgPaddingMirror.setPixel(i, j,
                                      QRgb(imgPaddingMirror.pixel(width - nTemp - 1, j)));
        nTemp ++;
    }
    return imgPaddingMirror;
}


/**
 * 线性滤波
 */
QImage ImgProcessor::filterLinear(const QImage &img, Matrix<double>& kernel, bool isGray)
{
    if(!isGray)
    {
        Matrix<double> rMat = Matrix<double>::convolutionPadded(Matrix<double>::fromQImage(img, 'r'), kernel);
        Matrix<double> gMat = Matrix<double>::convolutionPadded(Matrix<double>::fromQImage(img, 'g'), kernel);
        Matrix<double> bMat = Matrix<double>::convolutionPadded(Matrix<double>::fromQImage(img, 'b'), kernel);

        Matrix<int> r = Matrix<int>::normalizationMat(rMat);
        Matrix<int> g = Matrix<int>::normalizationMat(gMat);
        Matrix<int> b = Matrix<int>::normalizationMat(bMat);

        return Matrix<int>::toQImage(r, g, b);
    }
    else
    {
        Matrix<double> yMat = Matrix<double>::convolutionPadded(Matrix<double>::fromQImage(img, 'y'), kernel);

        Matrix<int> y = Matrix<int>::normalizationMat(yMat);

        return Matrix<int>::toQImage(y);
    }
}

/**
 * 裁剪
 */
QImage ImgProcessor::trimImg(const QImage &img, int nCol, int nRow)
{
    QImage newImg = QImage(img.width() - nCol + 1, img.height() - nRow + 1, img.format());
    int width = newImg.width();
    int height = newImg.height();
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            newImg.setPixel(i, j, QRgb(img.pixel(i + nCol / 2, j + nRow / 2)));
        }
    }
    cout<<"Processing completed !"<<endl;
    return newImg;
}


/**
 * 获取矩阵区域像素值
 */
Matrix<int> ImgProcessor::getPixel(const QImage &img, int x, int y, int size, char c)
{
    Matrix<int> value(size, size, 0);
    for(int i = x - size / 2; i <= x + size / 2; i ++)
    {
        for(int j = y - size / 2; j <= y + size / 2; j ++)
        {
            switch(c)
            {
            case 'r':
                value(i + size / 2 - x, j + size / 2 - y) = qRed(img.pixel(i, j));
                break;
            case 'g':
                value(i + size / 2 - x, j + size / 2 - y) = qGreen(img.pixel(i, j));
                break;
            case 'b':
                value(i + size / 2 - x, j + size / 2 - y) = qBlue(img.pixel(i, j));
                break;
            case 'y':
                value(i + size / 2 - x, j + size / 2 - y) = qGray(img.pixel(i, j));
                break;
            default:
                break;
            }
        }
    }
    return value;
}

/**
 * 获取矩阵区域像素值,返回值vector类型
 */
vector<int> ImgProcessor::getPixel(const QImage &img, int x, int y, int nCol, int nRow, char c)
{
    vector<int> value;
    for(int i = x - nCol / 2; i <= x + nCol / 2; i ++)
    {
        for(int j = y - nRow / 2; j <= y + nRow / 2; j ++)
        {
            switch(c)
            {
            case 'r':
                value.push_back(qRed(img.pixel(i, j)));
                break;
            case 'g':
                value.push_back(qGreen(img.pixel(i, j)));
                break;
            case 'b':
                value.push_back(qBlue(img.pixel(i, j)));
                break;
            case 'y':
                value.push_back(qGray(img.pixel(i, j)));
                break;
            default:
                break;
            }
        }
    }
    return value;
}


/**
 * 高斯滤波
 */
QImage ImgProcessor::gaussBlur(const QImage &img, int size, double sigma, bool isGray, char c)
{
    Matrix<double> ret = createGaussKernel(size, sigma);
    kernelNomalization(ret);
    return linear(img, ret, isGray, c);
}
/**
 * 生成高斯滤波矩阵
 */
Matrix<double> ImgProcessor::createGaussKernel(int size, double sigma)
{
    Matrix<double> ret(size, size, 0.0);
    for(int i = - size / 2; i <= size / 2; i ++)
    {
        for(int j = - size / 2; j <= size / 2; j ++)
        {
            double value = exp(-(i * i + j * j) / (2 * sigma * sigma));
            ret(i + size / 2, j + size / 2) = value;
        }
    }
    cout<<"Gauss Matrix:\n"<<ret<<endl;
    return ret;
}


/**
 * 双边滤波
 */
QImage ImgProcessor::bilateralFilter(const QImage &img, int size, double sigma, double colorSigma, char c)
{
    QImage paddingImg = padding(img, size, size, c);
    int width = paddingImg.width();
    int height = paddingImg.height();
    Matrix<int> filterMat(img.width(), img.height(), 0);
    Matrix<int> yMat = Matrix<int>::fromQImage(paddingImg, 'y');
    //等式右边的第一项
    Matrix<double> gaussKernel = createGaussKernel(size, sigma);
    //等式右边的第二项
    vector<double> colorSpaceKernel = colorSpaceValue(colorSigma);//先计算好的，然后从中取值
    for(int i = size / 2; i < width - size / 2; i ++)
    {
        for(int j = size / 2; j < height - size / 2; j ++)
        {
            Matrix<int> gray = getPixel(paddingImg, i, j, size, 'y');//像素块

            //计算出每个矩阵块kernel
            Matrix<double> kernel(size, size, 0.0);
            for(size_t m = 0; m < size; m ++)
            {
                for(size_t n = 0; n < size; n ++)
                {
                    int delta = abs(gray(m, n) - yMat(i, j));
                    kernel(m, n) = gaussKernel(m, n) * colorSpaceKernel[delta];
                }
            }
            kernelNomalization(kernel);

            //滤波
            double sum = 0;
            for(size_t m = 0; m < size; m ++)
            {
                for(size_t n = 0; n < size; n ++)
                {
                    sum += kernel(m, n) * gray(m, n);
                }
            }
            int g = (int)sum;
            if(g > 255) g = 255;
            if(g < 0) g = 0;
            filterMat(i - size / 2, j - size / 2) = g;
        }
    }

    return Matrix<int>::toQImage(filterMat);
}

vector<double> ImgProcessor::colorSpaceValue(double sigma)
{
    vector<double> ret;
    for(int i = 0; i < 256; i ++)
    {
        ret.push_back(exp(- i * i / (2 * sigma * sigma)));
    }
    return ret;
}


/**
 * 中值滤波
 */
QImage ImgProcessor::medianFilter(const QImage &img, int size, bool isGray, char c)
{
    QImage paddingImg = padding(img, size, size, c);
    int width = paddingImg.width();
    int height = paddingImg.height();
    QImage filterImg(paddingImg);
    for(int i = size / 2; i < width - size / 2; i ++)
    {
        for(int j = size / 2; j < height - size / 2; j ++)
        {
            if(isGray)
            {
                vector<int> gray = getPixel(paddingImg, i, j, size, size, 'y');
                sort(gray.begin(),gray.end());//排序
                int y = gray.at(gray.size() / 2);//取中值
                filterImg.setPixel(i, j, qRgb(y, y, y));
            }
            else
            {
                vector<int> red = getPixel(paddingImg, i, j, size, size, 'r');
                sort(red.begin(),red.end());
                int r = red.at(red.size() / 2);
                vector<int> green = getPixel(paddingImg, i, j, size, size, 'g');
                sort(green.begin(),green.end());
                int g = green.at(green.size() / 2);
                vector<int> blue = getPixel(paddingImg, i, j, size, size, 'b');
                sort(blue.begin(),blue.end());
                int b = blue.at(blue.size() / 2);
                filterImg.setPixel(i, j, qRgb(r, g, b));
            }
        }
    }

    return filterImg;
}


/**
 * 膨胀
 */
QImage ImgProcessor::expansion(const QImage &img, int size, bool isGray, char c)
{
    QImage paddingImg = padding(img, size, size, c);
    int width = paddingImg.width();
    int height = paddingImg.height();
    QImage filterImg(paddingImg);
    for(int i = size / 2; i < width - size / 2; i ++)
    {
        for(int j = size / 2; j < height - size / 2; j ++)
        {
            if(isGray)
            {
                vector<int> gray = getPixel(paddingImg, i, j, size, size, 'y');
                sort(gray.begin(),gray.end());//排序
                int y = gray.at(gray.size() - 1);//取到最大值
                filterImg.setPixel(i, j, qRgb(y, y, y));
            }
            else
            {
                vector<int> red = getPixel(paddingImg, i, j, size, size, 'r');
                sort(red.begin(),red.end());
                int r = red.at(red.size() - 1);
                vector<int> green = getPixel(paddingImg, i, j, size, size, 'g');
                sort(green.begin(),green.end());
                int g = green.at(green.size() - 1);
                vector<int> blue = getPixel(paddingImg, i, j, size, size, 'b');
                sort(blue.begin(),blue.end());
                int b = blue.at(blue.size() - 1);
                filterImg.setPixel(i, j, qRgb(r, g, b));
            }
        }
    }

    return filterImg;
}


/**
 * 腐蚀
 */
QImage ImgProcessor::corrosion(const QImage &img, int size, bool isGray, char c)
{
    QImage paddingImg = padding(img, size, size, c);
    int width = paddingImg.width();
    int height = paddingImg.height();
    QImage filterImg(paddingImg);
    for(int i = size / 2; i < width - size / 2; i ++)
    {
        for(int j = size / 2; j < height - size / 2; j ++)
        {
            if(isGray)
            {
                vector<int> gray = getPixel(paddingImg, i, j, size, size, 'y');
                sort(gray.begin(),gray.end());//排序
                int y = gray.at(0);//取到最小值
                filterImg.setPixel(i, j, qRgb(y, y, y));
            }
            else
            {
                vector<int> red = getPixel(paddingImg, i, j, size, size, 'r');
                sort(red.begin(),red.end());
                int r = red.at(0);
                vector<int> green = getPixel(paddingImg, i, j, size, size, 'g');
                sort(green.begin(),green.end());
                int g = green.at(0);
                vector<int> blue = getPixel(paddingImg, i, j, size, size, 'b');
                sort(blue.begin(),blue.end());
                int b = blue.at(0);
                filterImg.setPixel(i, j, qRgb(r, g, b));
            }
        }
    }

    return filterImg;
}


/**
 * 仿射变换
 */
QImage ImgProcessor::transform(const QImage &img, const Matrix<double> &mat)
{  
    bool ok;
    Matrix<double> invertedMat = Matrix<double>::inverted(mat, &ok);
    if(ok)
    {
        cout<<mat<<endl;
        int width = img.width();
        int height = img.height();

        vector<double> xCoordinate;
        vector<double> yCoordinate;
        double x, y;
        for(int i = 0; i <= width; i += width)
        {
            for(int j = 0; j <= height; j += height)
            {
                mat.map(i, j, &x, &y);
                xCoordinate.push_back(x);
                yCoordinate.push_back(y);
            }
        }
        sort(xCoordinate.begin(),xCoordinate.end());
        sort(yCoordinate.begin(),yCoordinate.end());

        int newWidth = ceil(xCoordinate.at(xCoordinate.size() - 1) - xCoordinate.at(0));
        int newHeight = ceil(yCoordinate.at(yCoordinate.size() - 1) - yCoordinate.at(0));

        QImage transfImg(newWidth, newHeight, img.format());

        for(int i = 0; i < newWidth; i ++)
        {
            for(int j = 0; j < newHeight; j ++)
            {
                //添加上平移量再进行变换
                invertedMat.map(i + xCoordinate.at(0), j + yCoordinate.at(0), &x, &y);
                if(x < 0 || x >= width - 1  ||
                        y < 0 || y >= height - 1)
                    transfImg.setPixel(i, j, qRgb(0, 0, 0));
                else
                {
                    transfImg.setPixel(i, j, bilinearInterpolation(x, y, img));
                }
            }
        }
        return transfImg;
    }
    return img;
}

QRgb ImgProcessor::bilinearInterpolation(double x, double y, const QImage &img)
{
    int x0 = (int)x;
    int y0 = (int)y;

    QRgb Q11 = img.pixel(x0, y0);
    QRgb Q12 = img.pixel(x0 + 1, y0);
    QRgb Q21 = img.pixel(x0, y0 + 1);
    QRgb Q22 = img.pixel(x0 + 1, y0 + 1);

    Matrix<double> xMat(1, 2, 0.0);
    xMat(0, 0) = 1 - x + x0;
    xMat(0, 1) = x - x0;
    Matrix<double> yMat(2, 1, 0.0);
    yMat(0, 0) = 1 - y + y0;
    yMat(1, 0) = y - y0;

    int r = (xMat * pixelMat(qRed(Q21), qRed(Q11), qRed(Q22), qRed(Q12)) * yMat)(0, 0);
    int g = (xMat * pixelMat(qGreen(Q21), qGreen(Q11), qGreen(Q22), qGreen(Q12)) * yMat)(0, 0);
    int b = (xMat * pixelMat(qBlue(Q21), qBlue(Q11), qBlue(Q22), qBlue(Q12)) * yMat)(0, 0);

    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;

    return QRgb(qRgb(r, g, b));
}

Matrix<double> ImgProcessor::pixelMat(int v11, int v12, int v21, int v22)
{
    Matrix<double> mat(2, 2, 0.0);
    mat(0, 0) = (double)v11;
    mat(0, 1) = (double)v12;
    mat(1, 0) = (double)v21;
    mat(1, 1) = (double)v22;
    return mat;
}

QImage ImgProcessor::GaussFrequecy(const QImage &img, double sigma, bool isLP)
{
    Matrix<int> imgMat = Matrix<int>::fromQImage(img, 'y');

    Matrix<complex<double> > fMat = fft2d(imgMat, imgMat.getNRow(), imgMat.getNCol());
    fMat.fftshift(fMat);

    size_t width = fMat.getNRow();
    size_t height = fMat.getNCol();

    Matrix<double> hMat(width, height, 0.0);//生成滤波器
    for(size_t i = 0; i < width; i ++)
    {
        for(size_t j = 0; j < height; j ++)
        {
            double delta = (i - width / 2) * (i - width / 2) + (j - height / 2) * (j - height / 2);
            if(isLP)
                hMat(i, j) = exp(- delta / (2.0 * sigma * sigma));
            else
                hMat(i, j) = 1 - exp(- delta / (2.0 * sigma * sigma));
        }
    }

    Matrix<complex<double> > gMat = Matrix<double>::dotMultiply(fMat, hMat);
    Matrix<complex<double> >::fftshift(gMat);
    Matrix<complex<double> > resultMat = ifft2d(gMat, gMat.getNRow(), gMat.getNCol());
    Matrix<double> mat = resultMat.getAbs(resultMat);

    return Matrix<int>::toQImage(Matrix<int>::normalizationMat(mat).subMatrix(0, img.width() - 1, 0, img.height() - 1));
}

