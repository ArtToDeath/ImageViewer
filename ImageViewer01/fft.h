#ifndef FFT_H
#define FFT_H

#include <complex>
#include <vector>

#include "matrix.h"

const double PI = 3.1415926;

size_t calcN(size_t length);
std::complex<double> pow(std::complex<double> base, int exponent);

//butterfly operation
std::vector<std::complex<double> > butterfly(std::vector<std::complex<double> > data, size_t N, bool isFFT);

// different function with different input
std::vector<std::complex<double> > fft(std::vector<int> data, size_t N=0);
std::vector<std::complex<double> > fft(std::vector<double> data, size_t N=0);
std::vector<std::complex<double> > fft(std::vector<std::complex<double> > data, size_t N=0);

Matrix<std::complex<double> > fft2d(const Matrix<int>&, size_t, size_t);
Matrix<std::complex<double> > fft2d(const Matrix<double>&, size_t, size_t);
Matrix<std::complex<double> > fft2d(const Matrix<std::complex<double> >&, size_t, size_t);
Matrix<std::complex<double> > fftRow(const Matrix<std::complex<double> >&);

std::vector<std::complex<double> > ifft(std::vector<std::complex<double> > data, size_t N=0);
Matrix<std::complex<double> > ifft2d(const Matrix<std::complex<double> >&, size_t, size_t);
Matrix<std::complex<double> > ifftRow(const Matrix<std::complex<double> >&);

#endif // FFT_H
