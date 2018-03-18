#pragma once

#include "DataSet.h"
#include "Regression.h"

template<typename T>
class GradientDescent
{

public:
    GradientDescent()
        : m_LearningRate( 0.0f )
        , m_RegressionMethod(nullptr)
        , DebugLog(false)
    {
    }

    void SetLearningRate(double learningRate)
    {
        m_LearningRate = learningRate;
    }

    void SetRegressionMethod(Regression<T>* regression)
    {
        m_RegressionMethod = regression;
    }

    void SetTrainingSet(const DataSet<T>& trainingSet)
    {
        m_DataSet = trainingSet;
    }

    void SetDebugLog( bool debug )
    {
        DebugLog = true;
    }

    void NormalizeFeatures()
    {
        CalculateMean();
        CalculateStd();

        Matrix<T>& examples = m_DataSet.GetExamples();
        examples = examples.DeductRow(m_Mean);
        examples = examples.DivideByRow(m_StandardDeviation);
    }

    Matrix<T> ComputeTheta( unsigned int iterations, const Matrix<T>& initialTheta )
    {
        Matrix<T> theta(initialTheta);

        // Insert a column of ones before the examples
        Matrix<T> examples = m_DataSet.GetExamples();

        Matrix<T> X;
        X.Init(examples.NumRows(), 1);
        X.SetAll(1.0f);
        X.CombineRight(examples);

        Matrix<T> xTranspose( X );
        xTranspose.Transpose();

        for (unsigned int i = 0; i < iterations; ++i)
        {
            Matrix<T> hypothesis = m_RegressionMethod->Hypothesis( X, theta );
            Matrix<T> gradient = m_RegressionMethod->Gradient(xTranspose, hypothesis, m_DataSet.GetAnswers());

            if (DebugLog)
            {
                hypothesis.Log("hypothesis");
                gradient.Log("gradient");

                double cost = m_RegressionMethod->Cost(X, theta, m_DataSet.GetAnswers());
                printf("Cost: %.3f\n", cost);
            }

            theta = theta - gradient * m_LearningRate;
        }

        return theta;
    }

    Matrix<T> Predict(const Matrix<T>& example, const Matrix<T>& theta)
    {
        Matrix<T> normalizedExample = example - m_Mean;
        normalizedExample.DivideByRow(m_StandardDeviation);

        Matrix<T> n;
        n.Init(1, 1);
        n.SetAll(1.0f);
        n.CombineRight(normalizedExample);

        Matrix<T> answer = n * theta;
        return answer;
    }

private:

    void CalculateMean()
    {
        Matrix<T> examples = m_DataSet.GetExamples();

        m_Mean.Init(1, examples.NumColumns());
        m_Mean.SetAll(0.0f);

        for (unsigned int i = 0; i < examples.NumRows(); ++i)
        {
            for (unsigned int j = 0; j < examples.NumColumns(); ++j)
            {
                T previousValue = m_Mean.Get(0, j);
                m_Mean.Set(0, j, previousValue + examples.Get(i, j));
            }
        }

        double scalingFactor = 1.0f / (double)(examples.NumRows());
        m_Mean = m_Mean * scalingFactor;

    }

    void CalculateStd()
    {
        Matrix<T> examples = m_DataSet.GetExamples();

        m_StandardDeviation.Init(1, examples.NumColumns());
        m_StandardDeviation.SetAll(0.0f);

        for (unsigned int i = 0; i < examples.NumRows(); ++i)
        {
            for (unsigned int j = 0; j < examples.NumColumns(); ++j)
            {
                T previousValue = m_StandardDeviation.Get(0, j);
                m_StandardDeviation.Set(0, j, previousValue + std::pow( examples.Get(i,j) - m_Mean.Get(0, j), 2.0 ));
            }
        }

        double scalingFactor = 1.0f / ( (double)(examples.NumRows() - 1 ) );
        m_StandardDeviation = m_StandardDeviation * (T)scalingFactor;

        m_StandardDeviation.Sqrt();
    }

    double m_LearningRate;
    DataSet<T> m_DataSet;

    Matrix<T> m_Mean;
    Matrix<T> m_StandardDeviation;

    Regression<T>* m_RegressionMethod;

    bool DebugLog;
};
