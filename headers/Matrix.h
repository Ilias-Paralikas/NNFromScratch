// Matrix class with it's methods to initialize it, but not a constructor because you need to
// have them in an array, so you declare them and then you initialize, or set them
#include <iostream>
#include <fstream>
#include <cstring>
#pragma once
using namespace std;

class Matrix
{
private:
    bool IsSet = false;
    int rows, columns, total;

public:
    float *data;

public:
    ~Matrix()
    {
        if (IsSet)
        {
            delete[] this->data;
            //cout << "Matrix deleted\n";
        }
    }

    void Set(int WantedRows, int Wantedcolumns)
    {
        this->rows = WantedRows;
        this->columns = Wantedcolumns;
        this->total = WantedRows * Wantedcolumns;
        if (IsSet)
            delete[] this->data;
        this->data = new float[total];
        this->IsSet = true;
    }

    void ReadMatrix(ifstream &network)
    {
        network.read((char *)this->data, total * sizeof(float));
    }
    void WriteMatrix(ofstream &network)
    {
        network.write((const char *)this->data, total * sizeof(float));
    }

    void Multiply(const Matrix &A, const Matrix &B)
    {
        float buff;
        if (A.columns != B.rows) //they have to share this dimension
            throw "Columns of Matrix A must be the same as rows of Matrix B";
        if (this->rows != A.rows || this->columns != B.columns)
            throw "Matrix must have rows of A and columns of B";
        for (int j = 0; j < this->columns; ++j)
        {
            for (int i = 0; i < this->rows; ++i)
            {

                buff = 0;
                for (int k = 0; k < A.columns; ++k)
                {
                    buff += A.data[i * A.columns + k] * B.data[k * B.columns + j];
                }
                this->data[i * this->columns + j] = buff;
            }
        }
    }

    void MultiplyByElement(const Matrix &A, const Matrix &B)
    {
        if (A.columns != B.columns || A.rows != B.rows || this->columns != A.columns || this->rows != A.rows)
            //they have to share this dimension
            throw "Matrix A && B must have same dimenstions";
        for (int i = 0; i < this->total; ++i)
        {
            this->data[i] = A.data[i] * B.data[i];
        }
    }

    void InPlaceAdd(const Matrix &A)
    {
        if (this->columns != A.columns || this->rows != A.rows) //they have to share this dimension
            throw "Matrix A && B must have same dimenstions";
        for (int i = 0; i < this->total; ++i)
        {
            this->data[i] += A.data[i];
        }
    }

    void InPlaceSubtract(const Matrix &A)
    {
        if (this->columns != A.columns || this->rows != A.rows) //they have to share this dimension
            throw "Matrix A && B must have same dimenstions";
        for (int i = 0; i < this->total; ++i)
        {
            this->data[i] -= A.data[i];
        }
    }

    void Equal(const Matrix &A)
    {
        if (this->columns != A.columns || this->rows != A.rows) //they have to share this dimension
            throw "Matrix A && B must have same dimenstions";
        memcpy(this->data, A.data, total * sizeof(float));
    }

    void InPlaceScaler(const float scalar)
    {
        for (int i = 0; i < this->total; ++i)
        {
            this->data[i] *= scalar;
        }
    }

    float Sum()
    {
        float answer = 0;
        for (int i = 0; i < this->total; i++)
        {
            answer += this->data[i];
        }
        return answer;
    }

    void Map(float (*f)(float))
    {
        for (int i = 0; i < this->columns; ++i)
        {
            for (int j = 0; j < this->rows; ++j)
            {
                this->data[i * this->columns + j] = f(this->data[i * this->columns + j]);
            }
        }
    }

    void TransposeVector()
    {
        int temp = this->columns;
        this->columns = this->rows;
        this->rows = temp;
    }

    void ShowMatrix()
    {
        for (int i = 0; i < this->rows; ++i)
        {
            for (int j = 0; j < this->columns; ++j)
            {
                cout << this->data[i * this->columns + j] << ",";
            }
            cout << endl;
        }
    }
};