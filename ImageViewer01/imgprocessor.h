#ifndef IMGPROCESSOR_H
#define IMGPROCESSOR_H

#include <QImage>
#include <QRgb>
#include "math.h"
#include <algorithm>
#include <QMatrix>
//#include "matrix.h"
#include "fft.h"
#include <iostream>
using namespace std;

struct HSI
{
    double Hue;
    double Saturation;
    double Intensity;
};

class ImgProcessor
{
public:
    ImgProcessor();
    static QImage rgb2gray(const QImage& img);//const为了安全，&提高效率
    static QImage reversal(const QImage& img);
    static QImage logTrans(const QImage& img, double c);
    static QImage grayHistEq(const QImage& img);
    static QImage colorHistEq01(const QImage& img);
    static QImage colorHistEq02(const QImage& img);
    static void RGB2HSI(QRgb&, HSI&);
    static void HSI2RGB(QRgb&, HSI&);
    static double Hue_2_RGB(double v1, double v2, double vH );

    static QImage linear(const QImage& img, Matrix<double>&, bool, char);
    static QImage gaussBlur(const QImage& img, int, double, bool, char);
    static QImage bilateralFilter(const QImage& img, int, double, double, char);

    static void kernelNomalization(Matrix<double>& kernel);

    static QImage paddingZero(const QImage&, int, int);
    static QImage paddingRepeat(const QImage&, int, int);
    static QImage paddingMirror(const QImage&, int, int);

    static QImage padding(const QImage&, int, int, char);
    static QImage filterLinear(const QImage&, Matrix<double>&, bool);
    static QImage trimImg(const QImage&, int, int);
    static Matrix<int> getPixel(const QImage&, int, int, int, char);
    static vector<int> getPixel(const QImage&, int, int, int, int, char);

    static Matrix<double> createGaussKernel(int, double);

    static vector<double> colorSpaceValue(double);

    static QImage medianFilter(const QImage&, int, bool, char);
    static QImage expansion(const QImage&, int, bool, char);
    static QImage corrosion(const QImage&, int, bool, char);

    static QImage transform(const QImage&, const Matrix<double>&);
    static QRgb bilinearInterpolation(double, double, const QImage&);
    static Matrix<double> pixelMat(int, int, int, int);

    static QImage GaussFrequecy(const QImage&, double, bool);
};

#endif // IMGPROCESSOR_H
