#pragma once
#include "Regression.h"
#include "Transform.h"

template<typename T>
class LogisticRegression : public Regression<T>
{
public:
    virtual Matrix<T> Hypothesis(const Matrix<T>& X, const Matrix<T>& theta)
    {
        return MatrixTransform::Sigmoid(X * theta);
    }

    virtual Matrix<T> Gradient(const Matrix<T>& xTranspose, const Matrix<T>& hypothesis, const Matrix<T>& y) override
    {
        // This is the same as for linear regression
        double scalingFactor = 1.0f / (double)(y.NumRows());
        Matrix<T> m = xTranspose * (hypothesis - y) * scalingFactor;
        return m;
    }

    virtual double Cost(const Matrix<T>& X, const Matrix<T>& theta, const Matrix<T>& y) override
    {
        Matrix<T> hypothesis = Hypothesis(X, theta);
        Matrix<T> logHypothesis = MatrixTransform::Log(hypothesis);

        Matrix<T> ones(y.NumRows(), y.NumColumns());
        ones.SetAll(1.0);

        Matrix<T> logOnes = MatrixTransform::Log(ones);
        Matrix<T> minusY = -1.0 * y;

        Matrix<T> logOneMinusHypothesis = MatrixTransform::Log( ones - hypothesis );
        Matrix<T> oneMinusY = ones - y;

        Matrix<T> m = ElementwiseMultiply(minusY, logHypothesis) - ElementwiseMultiply(oneMinusY, logOneMinusHypothesis);
        
        double cost = m.Sum();
        return cost / (double)y.NumRows();

        //h = sigmoid(X * theta);
        //v = sum(-y.*log(h) - (1 - y).*log(1 - h));
        //J = sum(v) / m;
    }

private:

};