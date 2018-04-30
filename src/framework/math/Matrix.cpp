#include "Matrix.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


Matrix::Matrix() {
    renglones = 0;
    columnas = 0;
}


Matrix::Matrix(int **dat, int rengl, int cols) {
    renglones = 0;
    columnas = 0;
    setData(dat, rengl, cols);
}

Matrix::Matrix(Matrix *m) {
    renglones = 0;
    columnas = 0;
    int i = 0;
    std::vector<int> temp;
    for (i = 1; i <= m->getNumCols(); i++) {
        temp = m->getVector(i);
        if (!temp.size() == 0) {
            addVector(temp);
        }
    }
}

Matrix::~Matrix() {


}

int Matrix::MatDet2x2(int **m) {
    return (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
}

int Matrix::MatDet3x3(int **m) {

    int posit = (m[0][0] * m[1][1] * m[2][2]) + (m[1][0] * m[2][1] * m[0][2]) + (m[2][0] * m[0][1] * m[1][2]);
    int negativ = (m[0][2] * m[1][1] * m[2][0]) + (m[1][2] * m[2][1] * m[0][0]) + (m[2][2] * m[1][0] * m[0][1]);
    return (posit - negativ);

}

void Matrix::setData(int *m1, int rengl, int cols) {

    int i = 0, j = 0;

    if (data.size() != 0) {
        limpia();
    }
    std::vector<int> temp;
    for (; i < rengl; i++) {
        for (j = 0; j < cols; j++) {
            temp.push_back(*(m1 + (i * cols) + j));
        }
        addRengl(temp);
        temp.clear();
    }
}

void Matrix::setData(int **m1, int rengl, int cols) {
    int i = 0, j = 0;

    if (data.size() != 0) {
        limpia();
    }
    std::vector<int> temp;
    for (; i < rengl; i++) {
        for (j = 0; j < cols; j++) {
            temp.push_back(m1[i][j]);
        }
        addRengl(temp);
        temp.clear();
    }
}

void Matrix::limpia() {
    data.clear();
    renglones = 0;
    columnas = 0;
}

void Matrix::addVector(std::vector<int> v) {
    if (columnas > 0) {
        if (renglones != v.size()) {
            printf("Error no puedo meter vectores de diferente tamano%d, %d\n", renglones, v.size());
            throw 200;
        }
    } else {
        renglones = v.size();
    }
    columnas++;
    data[columnas - 1] = v;
}

void Matrix::addRengl(std::vector<int> v) {
    if (renglones > 0) {
        if (columnas != v.size()) {
            printf("Error no puedo meter renglones de diferente tamano  %d, %d\n", columnas, v.size());
            throw 200;
        }
    } else {
        std::vector<int> temp;
        for (columnas = 0; columnas < v.size(); columnas++) {
            data[columnas] = temp;
        }
        columnas = v.size();
    }
    renglones++;

    int i = 0;
    std::map<int, std::vector<int> >::iterator miter;


    for (miter = data.begin(); miter != data.end(); miter++, i++) {
        miter->second.push_back(v[i]);
    }

}


void Matrix::quitaVector(int n) {
    if (n < 1 || n > columnas) {
        throw std::out_of_range("n es mayor del numero de columnas\n");
    }
    columnas--;
    int i = n - 1;
    data.erase(n - 1);
    for (; i < columnas; i++) {
        data[i] = data[i + 1];
    }
}

void Matrix::quitaRengl(int n) {
    if (n < 1 || n > renglones) {
        throw std::out_of_range("n es mayor del numero de renglones\n");
    }
    renglones--;
    int i = 0;
    std::vector<int>::iterator viter;
    for (i = 1; i <= columnas; i++) {
        viter = data[i - 1].begin() + (n - 1);
        data[i - 1].erase(viter);
    }
}

std::vector<int> Matrix::getVector(int n) {
    if (n < 1 || n > columnas) {
        throw std::out_of_range("n es mayor del numero de columnas\n");
    }
    return data[n - 1];
}

std::vector<int> Matrix::getRengl(int n) {
    if (n < 1 || n > renglones) {
        throw std::out_of_range("n es mayor del numero de renglones\n");
    }
    std::vector<int> temp;
    int i = 0;
    for (i = 1; i <= columnas; i++) {
        temp.push_back(data[i - 1].at(n));
    }
    return temp;
}

void Matrix::print() {

    std::map<int, std::vector<int> >::const_iterator miter;
    int j = 0;

    for (; j < renglones; j++) {
        for (miter = data.begin(); miter != data.end(); miter++) {
            printf("%d ", miter->second[j]);
        }
        printf("\n");
    }
}


int Matrix::getNumCols() {
    return columnas;
}

int Matrix::getNumRengl() {
    return renglones;
}

int Matrix::getVal(int rengl, int cols) {
    if (rengl > renglones || rengl < 1) {
        throw std::out_of_range("rengl no valido\n");
    }

    if (cols > columnas || cols < 1) {
        throw std::out_of_range("col no valido\n");
    }

    return data[cols - 1].at(rengl - 1);
}

Matrix Matrix::operator+(Matrix m2) {
    if (renglones != m2.getNumRengl() || columnas != m2.getNumCols()) {
        throw std::invalid_argument("error no se pueden sumar las matrices por ser de distinto tamano\n");
    }

    int i = 0, j = 0, **temp;
    temp = (int **) malloc(sizeof(int) * renglones);

    for (j = 1; j <= renglones; j++) {
        temp[j - 1] = (int *) malloc(sizeof(int) * columnas);
        for (i = 1; i <= columnas; i++) {
            temp[j - 1][i - 1] = getVal(j, i) + m2.getVal(j, i);
        }
    }
    Matrix res(temp, renglones, columnas);

    for (i = 0; i < renglones; i++) {
        free(temp[i]);
    }
    free(temp);
    return res;
}

Matrix Matrix::operator-(Matrix m2) {

    if (renglones != m2.getNumRengl() || columnas != m2.getNumCols()) {
        throw std::invalid_argument("error no se pueden restar las matrices por ser de distinto tamano\n");
    }

    int i = 0, j = 0, **temp;
    temp = (int **) malloc(sizeof(int) * renglones);

    for (j = 1; j <= renglones; j++) {
        temp[j - 1] = (int *) malloc(sizeof(int) * columnas);
        for (i = 1; i <= columnas; i++) {
            temp[j - 1][i - 1] = getVal(j, i) - m2.getVal(j, i);
        }
    }
    Matrix res(temp, renglones, columnas);

    for (i = 0; i < renglones; i++) {
        free(temp[i]);
    }
    free(temp);
    return res;

}

Matrix Matrix::operator*(Matrix m2) {
    if (columnas != m2.getNumRengl()) {
        printf("error no se pueden multiplicar las matrices por ser de distinto tamano\n");
        throw 200;
    }

    int i = 0;
    int j = 0;
    int **temp;
    int resul = 0;
    int k = 0;
    int l = 0;
    temp = (int **) malloc(sizeof(int) * renglones);

    for (i = 0; i < renglones; i++) {
        temp[i] = (int *) malloc(sizeof(int) * m2.getNumCols());
    }

    i = 1;
    for (k = 1; k <= renglones; k++) {
        resul = 0;
        for (l = 1; l <= columnas; l++) {
            resul += getVal(k, l) * m2.getVal(l, i);
            if (l == columnas) {
                temp[k - 1][i - 1] = resul;
            }
        }
        i++;
        if (i <= m2.getNumCols()) {
            k--;
        } else {
            i = 1;
        }
    }
    Matrix res(temp, renglones, m2.getNumCols());

    for (i = 0; i < renglones; i++) {
        free(temp[i]);
    }
    free(temp);
    return res;
}

Matrix Matrix::operator*(int n) {
    int i = 0, j = 0, **temp;
    temp = (int **) malloc(sizeof(int) * renglones);

    for (j = 1; j <= renglones; j++) {
        temp[j - 1] = (int *) malloc(sizeof(int) * columnas);
        for (i = 1; i <= columnas; i++) {
            temp[j - 1][i - 1] = getVal(j, i) * n;

        }
    }
    Matrix res(temp, renglones, columnas);

    for (i = 0; i < renglones; i++) {
        free(temp[i]);
    }
    free(temp);
    return res;
}


Matrix Matrix::operator*=(int n) {
    int i = 0, j = 0;

    for (j = 1; j <= renglones; j++) {
        for (i = 1; i <= columnas; i++) {
            data[i - 1].at(j) *= n;
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

int Matrix::determinante() {

    if (renglones == 1 && columnas == 1) {
        return getVal(1, 1);
    }
    Matrix cofactores = getMatrizCofactores();

    int res = 0;
    int i = 0;
    int j = 1;

    for (i = 1; i <= renglones; i++) {
        res += cofactores.getVal(i, j) * getVal(i, j);
    }

    return res;
}

int Matrix::determinante(int i, int j) {
    if (renglones == 1 && columnas == 1) {
        return getVal(1, 1);
    } else {
        Matrix nueva(this);
        nueva.quitaRengl(i);
        nueva.quitaVector(j);
        nueva.print();
        return nueva.determinante();
    }
}

Matrix Matrix::getMatrizCofactores() {
    int i = 0, j = 0, **temp;

    temp = (int **) malloc(sizeof(int *) * renglones);
    for (i = 1; i <= renglones; i++) {
        temp[i - 1] = (int *) malloc(sizeof(int) * columnas);
        for (j = 1; j <= columnas; j++) {
            temp[i - 1][j - 1] = (int) pow(-1, (i + j)) * determinante(i, j);
        }
    }

    Matrix cof(temp, renglones, columnas);
    for (i = 0; i < renglones; i++) {
        free(temp[i]);
    }
    free(temp);
    return cof;
}