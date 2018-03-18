#pragma once

#include "Matrix.h"

template<typename T>
class Regression
{
public:
    virtual Matrix<T> Hypothesis(const Matrix<T>& X, const Matrix<T>& theta) = 0;
    virtual Matrix<T> Gradient(const Matrix<T>& xTranspose, const Matrix<T>& hypothesis, const Matrix<T>& y) = 0;

    virtual double Cost(const Matrix<T>& X, const Matrix<T>& theta, const Matrix<T>& y)
    {
        return 0.0;
    }

};

