#include <map>
#include <vector>

class Matrix {

private:
    int columns_;
    int rows_;
    std::map<int, std::vector<int> > data;

    int Determinant(int i, int j);


public:
    Matrix();

    Matrix(int **dat, int rengl, int cols);

    explicit Matrix(Matrix *m);

    ~Matrix();

    int columns();

    int rows();

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

    int Determinant();

    void set_data(int *m1, int rows, int cols);

    void set_data(int **m1, int rengl, int cols);

    int getVal(int rengl, int cols);

    void limpia();

    void addVector(std::vector<int> v);

    std::vector<int> getVector(int n);

    std::vector<int> rows(int n);

    void quitaVector(int n);

    void quitaRengl(int n);

    void AddRow(std::vector<int> v);

    void Print();

    Matrix CofactorMatrix();

};


