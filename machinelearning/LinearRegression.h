#pragma once
#include "Regression.h"
#include "DataSet.h"

template<typename T>
class LinearRegression : public Regression<T>
{
public:
    virtual Matrix<T> Hypothesis(const Matrix<T>& X, const Matrix<T>& theta)
    {
        return X * theta;
    }

    virtual Matrix<T> Gradient(const Matrix<T>& xTranspose, const Matrix<T>& hypothesis, const Matrix<T>& y) override
    {
        double scalingFactor = 1.0f / (double)(y.NumRows());
        return xTranspose * (hypothesis - y) * scalingFactor;
    }
};