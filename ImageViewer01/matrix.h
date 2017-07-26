#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdexcept>
#include <QImage>

using namespace std;

template<typename T>
class Matrix
{
private:
    // point to the data
    T** _data;
    // number of row and column
    size_t _nRow, _nCol;
    // flag to indicate row-first or column-first
    int _t;
    size_t _startR, _startC;
    bool checkRange(size_t r, size_t c) const;
    void swap(Matrix<T>& mat);

public:
    // construct a r-row and c-column matrix with all init element
    Matrix(size_t r, size_t c, T init);
    // copy constructor
    Matrix(const Matrix<T>& mat);
    Matrix<T>& operator=(Matrix<T> mat);
    ~Matrix();
    // get element at r-row and c-column
    // return reference to make it modifiable
    T& operator()(size_t r, size_t c) const;
    // output the matrix
    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Matrix<U>& rhs);
    size_t getNRow() const;
    size_t getNCol() const;
    void setStartR(size_t);
    void setStartC(size_t);
    Matrix<T> subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const;
    std::vector<T> getRow(size_t r) const;
    std::vector<T> getCol(size_t c) const;
    bool setRow(size_t r, const std::vector<T>& d);
    bool setCol(size_t c, const std::vector<T>& d);
    // transpose it in-place
    Matrix<T>& transpose();
    //add
    Matrix<T> operator+(const Matrix<T>& mat);
    Matrix<T> operator+(const T& t) const;
    template<typename U>
    friend Matrix<U> operator+(const U& t, const Matrix<U>& mat);
    //subtract
    Matrix<T> operator-(const Matrix<T>& mat);
    Matrix<T> operator-(const T& t);
    template<typename U>
    friend Matrix<U> operator-(const U& t, const Matrix<U>& mat);
    //multiply
    Matrix<T> operator*(const Matrix<T>& mat);
    Matrix<T> operator*(const T& t) const;
    template<typename U>
    friend Matrix<U> operator*(const U& t, const Matrix<U>& mat);
    //divide
    Matrix<T> operator/(const T& t) const;

    static Matrix<T> convolution(const Matrix<T>&, const Matrix<T>&);//矩阵的卷积
    static Matrix<T> convolutionPadded(const Matrix<T>&, const Matrix<T>&);//空间滤波用到的卷积，填充过的图像卷积
    static Matrix<T> dotMultiply(const Matrix<T>&, const Matrix<T>&);//点乘
    static Matrix<complex<double> > dotMultiply(const Matrix<double> &, const Matrix<complex<double> > &);
    static Matrix<complex<double> > dotMultiply(const Matrix<complex<double> > &, const Matrix<double> &);
    static Matrix<double> getAbs(const Matrix<complex<double> >&);

    static Matrix<T> fromQImage(const QImage&, char);
    static QImage toQImage(const Matrix<int>&);
    static QImage toQImage(const Matrix<int>&, const Matrix<int>&, const Matrix<int>&);
    static Matrix<double> transformationMat(const Matrix<complex<double> >&);
    static void fftshift(Matrix<T>&);
    static Matrix<int> normalizationMat(const Matrix<double>&);

    static Matrix<T> inverted(const Matrix<T>&, bool *ok = 0);
    void map(int, int, double*, double*) const;

};

template<typename T>
Matrix<T>::Matrix(size_t r, size_t c, T init):
_nRow(r), _nCol(c), _t(0), _startR(0), _startC(0)
{
    _data = new T*[r];
    for(size_t i = 0; i < r; i++)
    {
        _data[i] = new T[c];
        for(size_t j = 0; j < c; j++)
        {
            _data[i][j] = init;
        }
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mat):
_nRow(mat._nRow), _nCol(mat._nCol), _t(mat._t), _startR(mat._startR), _startC(mat._startC)
{
    if(_t == 1) _nRow = mat._nCol, _nCol = mat._nRow;
    _data = new T*[_nRow];
    for(size_t i = 0; i < _nRow; i++)
    {
        _data[i] = new T[_nCol];
    }

    _nRow = mat._nRow, _nCol = mat._nCol;

    for(size_t i = 0; i < _nRow; i++)
    {
        for(size_t j = 0; j < _nCol; j++)
        {
            if(_t == 0)
                _data[i][j] = mat(i,j);
            else
                _data[j][i] = mat(i, j);
        }
    }
}

template<typename T>
void Matrix<T>::swap(Matrix<T>& mat) {
    std::swap(_data, mat._data);
    std::swap(_nRow, mat._nRow);
    std::swap(_nCol, mat._nCol);
    std::swap(_t, mat._t);
    std::swap(_startR, mat._startR);
    std::swap(_startC, mat._startC);
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> mat)
{
    swap(mat);
    return *this;
}

template<typename T>
size_t Matrix<T>::getNRow() const
{
    return _nRow;
}

template<typename T>
size_t Matrix<T>::getNCol() const
{
    return _nCol;
}

template<typename T>
void Matrix<T>::setStartR(size_t startR)
{
    _startR = startR;
}

template<typename T>
void Matrix<T>::setStartC(size_t startC)
{
    _startC = startC;
}

template<typename T>
bool Matrix<T>::checkRange(size_t r, size_t c) const
{
    if(r < 0 || r >= _nRow || c < 0 || c >= _nCol)
        return false;
    return true;
}

template<typename T>
T& Matrix<T>::operator()(size_t r, size_t c) const
{
    if(checkRange(r, c))    //check the validity of range
    {
        r = (r + _startR) % _nRow;
        c = (c + _startC) % _nCol;
        if(_t == 0)    //the original matrix
        {
           return _data[r][c];
        }
        else if(_t == 1)    //have transposed
        {
           return _data[c][r];
        }
    }

    throw range_error("the range of is illegal");
}

template<typename T>
Matrix<T> Matrix<T>::subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const
{
    if(!(checkRange(rs, cs) && checkRange(re, ce)))
    {
        throw range_error("the range of is illegal");
    }
    else
    {
        if(rs <= re && cs <= ce)
        {
            Matrix<T> subM(re-rs+1,ce-cs+1,0);    //the submatrix
            for(size_t i = rs; i <= re; i ++)
            {
                for(size_t j = cs; j <= ce; j ++)
                {
                    if(_t == 0)    //get the value from the original matrix
                        subM(i - rs, j - cs) = _data[i][j];
                    else if(_t == 1)    //get the value from the transpose matrix
                        subM(i - rs, j - cs) = _data[j][i];
                }
            }
            return subM;
        }
        else
            throw range_error("the order of submatrix is illegal");
    }
}

template<typename T>
std::vector<T> Matrix<T>::getRow(size_t r) const
{
    if(checkRange(r, _nCol - 1))
    {
        std::vector<T> row;
        for(size_t i = 0; i < _nCol; i ++)
        {
            if(_t == 0)    //the original matrix
                row.push_back(_data[r][i]);
            else if(_t == 1)    //have transposed
                row.push_back(_data[i][r]);
        }
        return row;
    }
}

template<typename T>
std::vector<T> Matrix<T>::getCol(size_t c) const
{
    if(checkRange(_nRow - 1, c))
    {
        std::vector<T> col;
        for(size_t i = 0; i < _nRow; i ++)
        {
            if(_t == 0)    //the original matrix
                col.push_back(_data[i][c]);
            else if(_t == 1)    //have transposed
                col.push_back(_data[c][i]);
        }
        return col;
    }
}

template<typename T>
bool Matrix<T>::setRow(size_t r, const std::vector<T>& d)
{
    if(checkRange(r, _nCol - 1))
    {
        for(size_t i = 0; i < d.size(); i ++)
        {
            if(i > _nCol) break;    //the values of the vector are too much
            if(_t == 0)    //the original matrix
                _data[r][i] = d.at(i);
            else if(_t == 1)    //have transposed
                _data[i][r] = d.at(i);
        }
        return true;
    }
    return false;
}

template<typename T>
bool Matrix<T>::setCol(size_t c, const std::vector<T>& d)
{
    if(checkRange(_nRow - 1, c))
    {
        for(size_t i = 0; i < d.size(); i ++)
        {
            if(i > _nRow) break;    //the values of the vector are too much
            if(_t == 0)    //the original matrix
                _data[i][c] = d.at(i);
            else if(_t == 1)    //have transposed
                _data[c][i] = d.at(i);
        }
        return true;
    }
    return false;
}

template<typename T>
Matrix<T>::~Matrix()
{
    if(_t == 1)
    {
        size_t temp = _nRow;
        _nRow = _nCol;
        _nCol = temp;
    }
    for(size_t i = 0; i < _nRow; i ++)
        delete _data[i];
    delete _data;    //delete _data
    // note the value of _t
}

template<typename T>
Matrix<T>& Matrix<T>::transpose()
{
    if(_t == 0)
        _t = 1;
    else if(_t == 1)
        _t = 0;

    //swap _nRow and _nCol
    size_t temp = _nRow;
    _nRow = _nCol;
    _nCol = temp;

    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& rhs)
{
    for(size_t i=0; i<rhs._nRow; ++i) {
        for(size_t j=0; j<rhs._nCol; ++j) {
            out << rhs(i,j) << "\t";
        }
        out << std::endl;
    }
    return out;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& mat)
{
    Matrix<T> matrixAdd(mat);
    matrixAdd._t = 0;
    for(size_t i = 0; i < _nRow; i ++)
    {
        for(size_t j = 0; j < _nCol; j ++)
        {
            matrixAdd(i, j) = _data[i][j] + mat(i, j);
        }
    }
    return matrixAdd;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const T& t) const
{
    Matrix<T> matrixAdd(*this);
    matrixAdd._t = 0;
    for(size_t i = 0; i < _nRow; i ++)
    {
        for(size_t j = 0; j < _nCol; j ++)
        {
            matrixAdd(i, j) = _data[i][j] + t;
        }
    }
    return matrixAdd;
}

template<typename T>
Matrix<T> operator+(const T& t, const Matrix<T>& mat)
{
    return mat + t;
}


template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& mat)
{
    Matrix<T> matSubtract(*this);
    matSubtract._t = 0;
    for(size_t i = 0; i < _nRow; i ++)
    {
        for(size_t j = 0; j < _nCol; j ++)
        {
            matSubtract(i, j) = _data[i][j] - mat(i, j);
        }
    }
    return matSubtract;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const T& t)
{
    Matrix<T> matSubtract(*this);
    matSubtract._t = 0;
    for(size_t i = 0; i < _nRow; i ++)
    {
        for(size_t j = 0; j < _nCol; j ++)
        {
            matSubtract(i, j) = _data[i][j] - t;
        }
    }
    return matSubtract;
}

template<typename T>
Matrix<T> operator-(const T& t, const Matrix<T>& mat)
{
    Matrix<T> matSubtract(mat.getNRow(), mat.getNCol(), t);
    matSubtract._t = 0;
    for(size_t i = 0; i < mat.getNRow(); i ++)
    {
        for(size_t j = 0; j < mat.getNCol(); j ++)
        {
            matSubtract(i, j) -= mat(i, j);
        }
    }
    return matSubtract;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mat)
{
    if(_nCol == mat._nRow)
    {
        Matrix<T> matMultiply(_nRow, mat._nCol, 0);
        matMultiply._t = 0;
        for(size_t i = 0; i < _nRow; i ++)
        {
            for(size_t j = 0; j < mat._nCol; j ++)
            {
                for(size_t k = 0; k < _nCol; k ++)
                {
                    if(_t == 1)
                    {
                        if(mat._t == 1)
                            matMultiply(i, j) += _data[k][i] * mat(j, k);
                        else
                            matMultiply(i, j) += _data[k][i] * mat(k, j);
                    }
                    else
                    {
                        if(mat._t == 1)
                            matMultiply(i, j) += _data[i][k] * mat(j, k);
                        else
                            matMultiply(i, j) += _data[i][k] * mat(k, j);
                    }
                }
            }
        }
        return matMultiply;
    }
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T& t) const
{
    Matrix<T> matMultiply(*this);
    matMultiply._t = 0;
    for(size_t i = 0; i < _nRow; i ++)
    {
        for(size_t j = 0; j < _nCol; j ++)
        {
            if(_t == 0)
                matMultiply(i, j) = _data[i][j] * t;
            else
                matMultiply(i, j) = _data[j][i] * t;
        }
    }
    return matMultiply;
}

template<typename T>
Matrix<T> operator*(const T& t, const Matrix<T>& mat)
{
    return mat * t;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const T& t) const
{
    Matrix<T> matDivide(*this);
    matDivide._t = 0;
    for(size_t i = 0; i < _nRow; i ++)
    {
        for(size_t j = 0; j < _nCol; j ++)
        {
            if(_t == 0)
                matDivide(i, j) = _data[i][j] / t;
            else
                matDivide(i, j) = _data[j][i] / t;
        }
    }
    return matDivide;
}

template<typename T>
Matrix<T> Matrix<T>::fromQImage(const QImage& img, char c)
{
    int width = img.width();
    int height = img.height();
    Matrix<T> mat(width, height, 0);
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < height; j ++)
        {
            if(c == 'r')
                mat(i, j) = qRed(img.pixel(i, j));
            else if(c == 'g')
                mat(i, j) = qGreen(img.pixel(i, j));
            else if(c == 'b')
                mat(i, j) = qBlue(img.pixel(i, j));
            else if(c == 'y')
                mat(i, j) = qGray(img.pixel(i, j));
            else break;
        }
    }
    return mat;
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<int>& yMat)
{
    QImage img(yMat.getNRow(), yMat.getNCol(), QImage::Format_Grayscale8);
    for(size_t i = 0; i < yMat.getNRow(); i ++)
    {
        for(size_t j = 0; j < yMat.getNCol(); j ++)
        {
            if(yMat(i, j) > 255) yMat(i, j) = 255;
            if(yMat(i, j) < 0) yMat(i, j) = 0;
            img.setPixel(i, j, qRgb(yMat(i, j), yMat(i, j), yMat(i, j)));
        }
    }
    return img;
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<int>& rMat, const Matrix<int>& gMat, const Matrix<int>& bMat)
{
    QImage img(rMat.getNRow(), rMat.getNCol(), QImage::Format_RGB32);
    for(size_t i = 0; i < rMat.getNRow(); i ++)
    {
        for(size_t j = 0; j < rMat.getNCol(); j ++)
        {
            if(rMat(i, j) > 255) rMat(i, j) = 255;
            if(rMat(i, j) < 0) rMat(i, j) = 0;
            if(gMat(i, j) > 255) gMat(i, j) = 255;
            if(gMat(i, j) < 0) gMat(i, j) = 0;
            if(bMat(i, j) > 255) bMat(i, j) = 255;
            if(bMat(i, j) < 0) bMat(i, j) = 0;
            img.setPixel(i, j, qRgb(rMat(i, j), gMat(i, j), bMat(i, j)));
        }
    }
    return img;
}

template<typename T>
Matrix<double> Matrix<T>::transformationMat(const Matrix<complex<double> >& data)
{
    Matrix<double> resultData(data.getNRow(), data.getNCol(), 0.0);
    for(size_t i = 0; i < data.getNRow(); i ++)
    {
        for(size_t j = 0; j < data.getNCol(); j ++)
        {
            resultData(i, j) = log(1 + abs(data(i, j)));
        }
    }
    return resultData;
}

template<typename T>
Matrix<int> Matrix<T>::normalizationMat(const Matrix<double>& data)
{
    Matrix<int> resultData(data.getNRow(), data.getNCol(), 0);
    double max = data(0, 0);
    double min = data(0, 0);
    for(size_t i = 0; i < data.getNRow(); i ++)
    {
        for(size_t j = 0; j < data.getNCol(); j ++)
        {
            if(data(i, j) > max) max = data(i, j);
            if(data(i, j) < min) min = data(i, j);
        }
    }

    for(size_t i = 0; i < data.getNRow(); i ++)
    {
        for(size_t j = 0; j < data.getNCol(); j ++)
        {
            int value = (int)((data(i, j) - min) / (max - min) * 255);
            if(value > 255) value = 255;
            if(value < 0) value = 0;
            resultData(i, j) = value;
        }
    }
    return resultData;
}

template<typename T>
void Matrix<T>::fftshift(Matrix<T> &data)
{
    data.setStartR(data.getNRow() / 2);
    data.setStartC(data.getNCol() / 2);
}

template<typename T>
Matrix<T> Matrix<T>::convolution(const Matrix<T> &fMat, const Matrix<T> &hMat)
{
    size_t M = fMat.getNRow() + hMat.getNRow() - 1;
    size_t N = fMat.getNCol() + hMat.getNCol() - 1;

    Matrix<T> gMat(M, N, 0);

    Matrix<T> rHMat = hMat;
    //对h旋转180
    for(size_t i = 0; i < hMat.getNRow(); i ++)
    {
        for(size_t j = 0; j < hMat.getNCol(); j ++)
        {
            rHMat(i, j) = hMat(hMat.getNRow() - 1 - i, hMat.getNCol() - 1 - j);
        }
    }
    //扫描
    for(size_t x = 0; x < M; x ++)
    {
        for(size_t y = 0; y < N; y ++)
        {
            T sum = 0;
            T temp = 0;
            for(size_t m = 0; m < hMat.getNRow(); m ++)
            {
                for(size_t n = 0; n < hMat.getNCol(); n ++)
                {
                    try{
                        temp = fMat(m - hMat.getNRow() + 1 + x, n - hMat.getNCol() + 1 + y);
                    }
                    catch(range_error e){
                        temp = 0;
                        if(x == 1 && y == 1)
                            cout<<e.what()<<sum<<endl;
                    }
                    sum += rHMat(m, n) * temp;
                    temp = 0;
                }
            }
            gMat(x, y) = sum;
        }
    }
    return gMat;
}

template<typename T>
Matrix<T> Matrix<T>::convolutionPadded(const Matrix<T> &fMat, const Matrix<T> &hMat)
{
    //因为已经填充过了
    size_t M = fMat.getNRow() - hMat.getNRow() + 1;
    size_t N = fMat.getNCol() - hMat.getNCol() + 1;

    Matrix<T> gMat(M, N, 0);

    Matrix<T> rHMat = hMat;
    //对h旋转180
    for(size_t i = 0; i < hMat.getNRow(); i ++)
    {
        for(size_t j = 0; j < hMat.getNCol(); j ++)
        {
            rHMat(i, j) = hMat(hMat.getNRow() - 1 - i, hMat.getNCol() - 1 - j);
        }
    }
    //扫描
    for(size_t x = 0; x < M; x ++)
    {
        for(size_t y = 0; y < N; y ++)
        {
            T sum = 0;
            for(size_t m = 0; m < hMat.getNRow(); m ++)
            {
                for(size_t n = 0; n < hMat.getNCol(); n ++)
                {
                    sum += fMat(m + x, n + y) * rHMat(m, n);
                }
            }
            gMat(x, y) = sum;
        }
    }
    return gMat;
}

/**
 * 点乘
 */
template<typename T>
Matrix<T> Matrix<T>::dotMultiply(const Matrix<T> &xMat, const Matrix<T> &yMat)
{
    size_t M, N;
    M = xMat.getNRow() < yMat.getNRow() ?  xMat.getNRow() : yMat.getNRow();
    N = xMat.getNCol() < yMat.getNCol() ? xMat.getNCol() : yMat.getNCol();
    Matrix<T> resultMat(M, N, 0);
    for(size_t i = 0; i < M; i ++)
    {
        for(size_t j = 0; j < N; j ++)
        {
            resultMat(i, j) = xMat(i, j) * yMat(i, j);
        }
    }
    return resultMat;
}

template<typename T>
Matrix<complex<double> > Matrix<T>::dotMultiply(const Matrix<double> &xMat, const Matrix<complex<double> > &yMat)
{
    size_t M, N;
    M = xMat.getNRow() < yMat.getNRow() ?  xMat.getNRow() : yMat.getNRow();
    N = xMat.getNCol() < yMat.getNCol() ? xMat.getNCol() : yMat.getNCol();
    Matrix<complex<double> > resultMat(M, N, 0);
    for(size_t i = 0; i < M; i ++)
    {
        for(size_t j = 0; j < N; j ++)
        {
            resultMat(i, j) = xMat(i, j) * yMat(i, j);
        }
    }
    return resultMat;
}

template<typename T>
Matrix<complex<double> > Matrix<T>::dotMultiply(const Matrix<complex<double> > &xMat, const Matrix<double> &yMat)
{
    size_t M, N;
    M = xMat.getNRow() < yMat.getNRow() ?  xMat.getNRow() : yMat.getNRow();
    N = xMat.getNCol() < yMat.getNCol() ? xMat.getNCol() : yMat.getNCol();
    Matrix<complex<double> > resultMat(M, N, 0);
    for(size_t i = 0; i < M; i ++)
    {
        for(size_t j = 0; j < N; j ++)
        {
            resultMat(i, j) = xMat(i, j) * yMat(i, j);
        }
    }
    return resultMat;
}

template<typename T>
Matrix<double> Matrix<T>::getAbs(const Matrix<complex<double> > &data)
{
    Matrix<double> result(data.getNRow(), data.getNCol(), 0);
    for(size_t i = 0; i < data.getNRow(); i ++)
    {
        for(size_t j = 0; j < data.getNCol(); j ++)
        {
            result(i, j) = abs(data(i, j));
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::inverted(const Matrix<T>& mat, bool* ok)
{
    if(mat.getNCol() != 3 || mat.getNRow() != 3)
    {
        *ok = false;
        return Matrix(mat);
    }

    //行列式
    T det = mat(0, 0)*mat(1, 1)*mat(2, 2) + mat(0, 1)*mat(1, 2)*mat(2, 0) + mat(0, 2)*mat(1, 0)*mat(2, 1) -
            mat(0, 2)*mat(1, 1)*mat(2, 0) - mat(0, 1)*mat(1, 0)*mat(2, 2) - mat(0, 0)*mat(1, 2)*mat(2, 1);
    if(0 == det)
    {
        *ok = false;
        return Matrix(mat);
    }

    *ok = true;
    Matrix<T> invertedMat(3, 3, 0);

    //伴随矩阵乘以行列式的倒数
    invertedMat(0, 0) = (mat(1, 1)*mat(2, 2) - mat(1, 2)*mat(2, 0))/det;
    invertedMat(1, 0) = -1 * (mat(1, 0)*mat(2, 2) - mat(1, 2)*mat(2, 0))/det;
    invertedMat(2, 0) = (mat(1, 0)*mat(2, 1) - mat(1, 1)*mat(2, 0))/det;
    invertedMat(0, 1) = -1 * (mat(0, 1)*mat(2, 2) - mat(0, 2)*mat(2, 1))/det;
    invertedMat(1, 1) = (mat(0, 0)*mat(2, 2) - mat(0, 2)*mat(2, 0))/det;
    invertedMat(2, 1) = -1 * (mat(0, 0)*mat(2, 1) - mat(0, 1)*mat(2, 0))/det;
    invertedMat(0, 2) = (mat(0, 1)*mat(1, 2) - mat(0, 2)*mat(1, 1))/det;
    invertedMat(1, 2) = -1 * (mat(0, 0)*mat(1, 2) - mat(0, 2)*mat(1, 0))/det;
    invertedMat(2, 2) = (mat(0, 0)*mat(1, 1) - mat(0, 1)*mat(1, 0))/det;

    return invertedMat;
}

template<typename T>
void Matrix<T>::map(int i, int j, double *x, double *y) const
{
    *x = _data[0][0] * i + _data[1][0] * j + _data[2][0];
    *y = _data[0][1] * i + _data[1][1] * j + _data[2][1];
}

#endif // MATRIX

