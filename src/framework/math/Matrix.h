#include <map>
#include <vector>

class Matrix {

private:
    int columnas;
    int renglones;
    std::map<int, std::vector<int> > data;

    int determinante(int i, int j);


public:
    Matrix();

    Matrix(int **dat, int rengl, int cols);

    explicit Matrix(Matrix *m);

    ~Matrix();

    int getNumCols();

    int getNumRengl();

    int MatDet2x2(int **m);

    int MatDet3x3(int **m);

    Matrix operator+(Matrix m2);

    Matrix operator-(Matrix m2);

    Matrix operator*(Matrix m2);

    Matrix operator*(int n);

    Matrix operator*=(int n);

    Matrix operator*=(Matrix m2);

    Matrix operator+=(Matrix m2);

    Matrix operator-=(Matrix m2);

    int determinante();

    void setData(int *m1, int rengl, int cols);

    void setData(int **m1, int rengl, int cols);

    int getVal(int rengl, int cols);

    void limpia();

    void addVector(std::vector<int> v);

    std::vector<int> getVector(int n);

    std::vector<int> getRengl(int n);

    void quitaVector(int n);

    void quitaRengl(int n);

    void addRengl(std::vector<int> v);

    void print();

    Matrix getMatrizCofactores();

};


