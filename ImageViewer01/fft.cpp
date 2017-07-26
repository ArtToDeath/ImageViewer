#include "fft.h"
#include <iostream>
#include <cmath>
#define PI 3.1415926

using namespace std;

size_t calcN(size_t length) {
    if((length & (length - 1)) == 0) //按位与判断2的整次幂
        return length;
    else
    {   //快速得到大于length的最小2的整次幂
        size_t N = length;
        do
        {
            length = length>>1;
            N = N | length;
        }while(length != 0);
        N = N + 1;
        return N;
    }
}

complex<double> pow(complex<double> base, int exponent) {
    if(exponent == 0)
        return complex<double>(1);
    else if(exponent == 1)
        return base;
    else
        return base * pow(base, exponent - 1);
}

std::vector<std::complex<double> >
butterfly(std::vector<std::complex<double> > data, size_t N, bool isFFT)
{
    vector<complex<double> > resultData;

    // change length to make it beign just the power of 2
    // append 0 if necessary
    N = calcN(N);
    while(N > data.size())
    {
        data.push_back(complex<double>(0));
    }

    if(N < 2) return data;
    else if(N == 2)
    {
        resultData.push_back(data[0] + data[1]);
        resultData.push_back(data[0] - data[1]);
        return resultData;
    }
    else
    {
        vector<complex<double> > resultDataEven;
        vector<complex<double> > resultDataOdd;

        for(size_t i = 0; i < N; i ++)
        {
            if(i % 2 == 0)
                resultDataEven.push_back(data[i]);
            else
                resultDataOdd.push_back(data[i]);
        }
        resultDataEven = butterfly(resultDataEven, N / 2, isFFT);
        resultDataOdd = butterfly(resultDataOdd, N / 2, isFFT);

        for(size_t k = 0; k < N / 2; k ++ )
        {
            if(isFFT)
                resultDataOdd[k] = resultDataOdd[k] * pow(complex<double>(cos(-2*PI/N), sin(-2*PI/N)), k);
            else
                resultDataOdd[k] = resultDataOdd[k] * pow(complex<double>(cos(2*PI/N), sin(2*PI/N)), k);

            resultData.push_back(resultDataEven[k] + resultDataOdd[k]);
        }
        for(size_t k = 0; k < N / 2; k ++ )
        {
            resultData.push_back(resultDataEven[k] - resultDataOdd[k]);
        }
        return resultData;
    }
}

vector<complex<double> >
fft(vector<int> data, size_t N)
{
    // convert data to vector<complex<double> >
    vector<complex<double> > complexData;
    for(size_t i = 0; i < data.size(); i ++)
    {
        complexData.push_back(complex<double>(data.at(i)));
    }
    return fft(complexData, N);
}

vector<complex<double> >
fft(vector<double> data, size_t N)
{
    // convert data to vector<complex<double> >
    vector<complex<double> > complexData;
    for(size_t i = 0; i < data.size(); i ++)
    {
        complexData.push_back(complex<double>(data.at(i)));
    }
    return fft(complexData, N);
}

vector<complex<double> >
fft(vector<complex<double> > data, size_t N)
{
    return butterfly(data, N, true);
}

/*
fft2d
*/
Matrix<complex<double> >
fft2d(const Matrix<int>& data, size_t row, size_t col)
{
    Matrix<complex<double> > complexData(row, col, complex<double>(0));
    for(size_t i = 0; i < row; i ++)
    {
        for(size_t j = 0; j < col; j ++)
        {
            complexData(i, j) = complex<double>(data(i, j));
        }
    }
    return fft2d(complexData, row, col);
}

Matrix<complex<double> >
fft2d(const Matrix<double>& data, size_t row, size_t col)
{
    Matrix<complex<double> > complexData(row, col, complex<double>(0));
    for(size_t i = 0; i < row; i ++)
    {
        for(size_t j = 0; j < col; j ++)
        {
            complexData(i, j) = complex<double>(data(i, j));
        }
    }
    return fft2d(complexData, row, col);
}

Matrix<complex<double> >
fft2d(const Matrix<complex<double> >& data, size_t row, size_t col)
{
    size_t rowTemp = row;
    size_t colTemp = col;
    row = calcN(rowTemp);
    col = calcN(colTemp);
    Matrix<complex<double> > paddedZeroData(row, col, complex<double>(0));
    for(size_t i = 0; i < rowTemp; i ++)
    {
        for(size_t j = 0; j < colTemp; j ++)
        {
            paddedZeroData(i, j) = data(i, j);
        }
    }
    return fftRow(fftRow(paddedZeroData).transpose()).transpose();
}

Matrix<complex<double> >
fftRow(const Matrix<complex<double> >& data)
{
    Matrix<complex<double> > resultData(data);
    for(size_t i = 0; i < data.getNRow(); i ++)
    {
        vector<complex<double> > nRow = data.getRow(i);
        resultData.setRow(i, fft(nRow, nRow.size()));
    }
    return resultData;
}

/*ifft*/
vector<complex<double> >
ifft(vector<complex<double> > data, size_t N)
{
    vector<complex<double> > resultData = butterfly(data, N, false);
    for(size_t i = 0; i < resultData.size(); i ++)
    {
        resultData[i] /= calcN(N);
    }
    return resultData;
}

Matrix<std::complex<double> >
ifft2d(const Matrix<std::complex<double> >& data, size_t row, size_t col)
{
    size_t rowTemp = row;
    size_t colTemp = col;
    row = calcN(rowTemp);
    col = calcN(colTemp);
    Matrix<complex<double> > paddedZeroData(row, col, complex<double>(0));
    for(size_t i = 0; i < rowTemp; i ++)
    {
        for(size_t j = 0; j < colTemp; j ++)
        {
            paddedZeroData(i, j) = data(i, j);
        }
    }
    return ifftRow(ifftRow(paddedZeroData).transpose()).transpose();
}

Matrix<complex<double> >
ifftRow(const Matrix<complex<double> >& data)
{
    Matrix<complex<double> > resultData(data);
    for(size_t i = 0; i < data.getNRow(); i ++)
    {
        vector<complex<double> > nRow = data.getRow(i);
        resultData.setRow(i, ifft(nRow, nRow.size()));
    }
    return resultData;
}
