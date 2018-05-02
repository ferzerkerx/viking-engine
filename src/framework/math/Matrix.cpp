#include "Matrix.h"
#include <malloc.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>


Matrix::Matrix() {
    rows_ = 0;
    columns_ = 0;
}


Matrix::Matrix(int **dat, int rengl, int cols) {
    rows_ = 0;
    columns_ = 0;
    set_data(dat, rengl, cols);
}

Matrix::Matrix(Matrix *m) {
    rows_ = 0;
    columns_ = 0;
    int i = 0;
    std::vector<int> temp;
    for (i = 1; i <= m->columns(); i++) {
        temp = m->getVector(i);
        if (temp.empty() == 0) {
            addVector(temp);
        }
    }
}

Matrix::~Matrix() = default;

int Matrix::MatDet2x2(int **m) {
    return (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
}

int Matrix::MatDet3x3(int **m) {
    int posit = (m[0][0] * m[1][1] * m[2][2]) + (m[1][0] * m[2][1] * m[0][2]) + (m[2][0] * m[0][1] * m[1][2]);
    int negativ = (m[0][2] * m[1][1] * m[2][0]) + (m[1][2] * m[2][1] * m[0][0]) + (m[2][2] * m[1][0] * m[0][1]);
    return (posit - negativ);

}

void Matrix::set_data(int *m1, int rows, int cols) {
    int i = 0;
    int j = 0;

    if (!data.empty()) {
        limpia();
    }
    std::vector<int> temp;
    for (; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            temp.push_back(*(m1 + (i * cols) + j));
        }
        AddRow(temp);
        temp.clear();
    }
}

void Matrix::set_data(int **m1, int rengl, int cols) {
    int i = 0;
    int j = 0;

    if (!data.empty()) {
        limpia();
    }
    std::vector<int> temp;
    for (; i < rengl; i++) {
        for (j = 0; j < cols; j++) {
            temp.push_back(m1[i][j]);
        }
        AddRow(temp);
        temp.clear();
    }
}

void Matrix::limpia() {
    data.clear();
    rows_ = 0;
    columns_ = 0;
}

void Matrix::addVector(std::vector<int> v) {
    if (columns_ > 0) {
        if (rows_ != v.size()) {
            throw std::out_of_range("Vector has different size than matrix\n");
        }
    } else {
        rows_ = static_cast<int>(v.size());
    }
    columns_++;
    data[columns_ - 1] = v;
}

void Matrix::AddRow(std::vector<int> v) {
    if (rows_ > 0) {
        if (columns_ != v.size()) {
            throw std::out_of_range("Row has a different size than matrix\n");
        }
    } else {
        std::vector<int> temp;
        for (columns_ = 0; columns_ < v.size(); columns_++) {
            data[columns_] = temp;
        }
        columns_ = static_cast<int>(v.size());
    }
    rows_++;

    int i = 0;
    std::map<int, std::vector<int> >::iterator miter;


    for (miter = data.begin(); miter != data.end(); miter++, i++) {
        miter->second.push_back(v[i]);
    }

}


void Matrix::quitaVector(int n) {
    if (n < 1 || n > columns_) {
        throw std::out_of_range("n bigger than number of columns\n");
    }
    columns_--;
    int i = n - 1;
    data.erase(n - 1);
    for (; i < columns_; i++) {
        data[i] = data[i + 1];
    }
}

void Matrix::quitaRengl(int n) {
    if (n < 1 || n > rows_) {
        throw std::out_of_range("n is bigger than number of rows\n");
    }
    rows_--;
    int i = 0;
    std::vector<int>::iterator viter;
    for (i = 1; i <= columns_; i++) {
        viter = data[i - 1].begin() + (n - 1);
        data[i - 1].erase(viter);
    }
}

std::vector<int> Matrix::getVector(int n) {
    if (n < 1 || n > columns_) {
        throw std::out_of_range("n bigger than number of columns\n");
    }
    return data[n - 1];
}

std::vector<int> Matrix::rows(int n) {
    if (n < 1 || n > rows_) {
        throw std::out_of_range("n is bigger than number of rows\n");
    }
    std::vector<int> temp;
    int i = 0;
    for (i = 1; i <= columns_; i++) {
        temp.push_back(data[i - 1].at(static_cast<unsigned long>(n)));
    }
    return temp;
}

void Matrix::Print() {

    std::map<int, std::vector<int> >::const_iterator miter;
    int j = 0;

    for (; j < rows_; j++) {
        for (miter = data.begin(); miter != data.end(); miter++) {
            printf("%d ", miter->second[j]);
        }
        printf("\n");
    }
}


int Matrix::columns() {
    return columns_;
}

int Matrix::rows() {
    return rows_;
}

int Matrix::getVal(int rengl, int cols) {
    if (rengl > rows_ || rengl < 1) {
        throw std::out_of_range("invalid row\n");
    }

    if (cols > columns_ || cols < 1) {
        throw std::out_of_range("invalid column\n");
    }

    return data[cols - 1].at(static_cast<unsigned long>(rengl - 1));
}

Matrix Matrix::operator+(Matrix m2) {
    if (rows_ != m2.rows() || columns_ != m2.columns()) {
        throw std::invalid_argument("Matrixes have different sizes\n");
    }

    int i = 0;
    int j = 0;
    int **temp;
    temp = (int **) malloc(sizeof(int) * rows_);

    for (j = 1; j <= rows_; j++) {
        temp[j - 1] = (int *) malloc(sizeof(int) * columns_);
        for (i = 1; i <= columns_; i++) {
            temp[j - 1][i - 1] = getVal(j, i) + m2.getVal(j, i);
        }
    }
    Matrix res(temp, rows_, columns_);

    for (i = 0; i < rows_; i++) {
        free(temp[i]);
    }
    free(temp);
    return res;
}

Matrix Matrix::operator-(Matrix m2) {
    if (rows_ != m2.rows() || columns_ != m2.columns()) {
        throw std::invalid_argument("Matrixes have different sizes\n");
    }

    int i = 0;
    int j = 0;
    int **temp;
    temp = (int **) malloc(sizeof(int) * rows_);

    for (j = 1; j <= rows_; j++) {
        temp[j - 1] = (int *) malloc(sizeof(int) * columns_);
        for (i = 1; i <= columns_; i++) {
            temp[j - 1][i - 1] = getVal(j, i) - m2.getVal(j, i);
        }
    }
    Matrix res(temp, rows_, columns_);

    for (i = 0; i < rows_; i++) {
        free(temp[i]);
    }
    free(temp);
    return res;

}

Matrix Matrix::operator*(Matrix m2) {
    if (columns_ != m2.rows()) {
        throw std::invalid_argument("Matrixes have different sizes\n");
    }

    int i = 0;
    int **temp;
    int resul = 0;
    int k = 0;
    int l = 0;
    temp = (int **) malloc(sizeof(int) * rows_);

    for (i = 0; i < rows_; i++) {
        temp[i] = (int *) malloc(sizeof(int) * m2.columns());
    }

    i = 1;
    for (k = 1; k <= rows_; k++) {
        resul = 0;
        for (l = 1; l <= columns_; l++) {
            resul += getVal(k, l) * m2.getVal(l, i);
            if (l == columns_) {
                temp[k - 1][i - 1] = resul;
            }
        }
        i++;
        if (i <= m2.columns()) {
            k--;
        } else {
            i = 1;
        }
    }
    Matrix res(temp, rows_, m2.columns());

    for (i = 0; i < rows_; i++) {
        free(temp[i]);
    }
    free(temp);
    return res;
}

Matrix Matrix::operator*(int n) {
    int i = 0;
    int j = 0;
    int **temp;
    temp = (int **) malloc(sizeof(int) * rows_);

    for (j = 1; j <= rows_; j++) {
        temp[j - 1] = (int *) malloc(sizeof(int) * columns_);
        for (i = 1; i <= columns_; i++) {
            temp[j - 1][i - 1] = getVal(j, i) * n;

        }
    }
    Matrix res(temp, rows_, columns_);

    for (i = 0; i < rows_; i++) {
        free(temp[i]);
    }
    free(temp);
    return res;
}


Matrix Matrix::operator*=(int n) {
    int i = 0;
    int j = 0;

    for (j = 1; j <= rows_; j++) {
        for (i = 1; i <= columns_; i++) {
            data[i - 1].at(static_cast<unsigned long>(j)) *= n;
        }
    }
    return *this;
}

Matrix Matrix::operator*=(Matrix m2) {
    *this = this->operator*(m2);
    return *this;
}

Matrix Matrix::operator+=(Matrix m2) {
    *this = this->operator+(m2);
    return *this;
}

Matrix Matrix::operator-=(Matrix m2) {
    *this = this->operator-(m2);
    return *this;
}

int Matrix::Determinant() {

    if (rows_ == 1 && columns_ == 1) {
        return getVal(1, 1);
    }
    Matrix cofactores = CofactorMatrix();

    int res = 0;
    int i = 0;
    int j = 1;

    for (i = 1; i <= rows_; i++) {
        res += cofactores.getVal(i, j) * getVal(i, j);
    }

    return res;
}

int Matrix::Determinant(int i, int j) {
    if (rows_ == 1 && columns_ == 1) {
        return getVal(1, 1);
    }
    Matrix copy(this);
    copy.quitaRengl(i);
    copy.quitaVector(j);
    copy.Print();
    return copy.Determinant();

}

Matrix Matrix::CofactorMatrix() {
    int i = 0;
    int j = 0;
    int **temp;

    temp = (int **) malloc(sizeof(int *) * rows_);
    for (i = 1; i <= rows_; i++) {
        temp[i - 1] = (int *) malloc(sizeof(int) * columns_);
        for (j = 1; j <= columns_; j++) {
            temp[i - 1][j - 1] = (int) pow(-1, (i + j)) * Determinant(i, j);
        }
    }

    Matrix cof(temp, rows_, columns_);
    for (i = 0; i < rows_; i++) {
        free(temp[i]);
    }
    free(temp);
    return cof;
}