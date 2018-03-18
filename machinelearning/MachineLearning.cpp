// MachineLearning.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctime>
#include "Matrix.h"
#include "DataSet.h"
#include "LinearRegression.h"
#include "LogisticRegression.h"
#include "GradientDescent.h"

void LinearRegressionTest()
{
    printf("Starting LinearRegressionTest\n\n");

    clock_t startTime = clock();

    DataSet<double> dataSet(2, 1);
    std::string fileName = "ex1data2.txt";
    if (!dataSet.LoadFromFile(fileName))
    {
        printf("Failed to load file: %s\n", fileName.c_str());
        return;
    }

    Matrix<double> theta;
    theta.Init(3, 1);
    theta.Set(0, 0, 0.0);
    theta.Set(1, 0, 0.0);
    theta.Set(2, 0, 0.0);

    GradientDescent<double> gd;
    gd.SetTrainingSet(dataSet);
    gd.SetLearningRate(0.1);
    gd.NormalizeFeatures();

    LinearRegression<double> lr;
    gd.SetRegressionMethod(&lr);

    theta = gd.ComputeTheta(400, theta);
    theta.Log( "theta" );

    Matrix<double> example;
    example.Init(1, 2);
    example.Set(0, 0, 1650.0f);
    example.Set(0, 1, 3.0f);

    Matrix<double> answer = gd.Predict(example, theta);
    answer.Log("prediction");

    clock_t timeElapsed = clock() - startTime;
    printf("Time Taken (ms): %d", timeElapsed);
}

void LogisticRegressionTest()
{
    printf("\n\nStarting LogisticRegressionTest\n\n");

    clock_t startTime = clock();

    DataSet<double> dataSet(2, 1);

    std::string fileName = "ex2data1.txt";
    if (!dataSet.LoadFromFile(fileName))
    {
        printf("Failed to load file: %s\n", fileName.c_str());
        return;
    }

    Matrix<double> theta;
    theta.Init(3, 1);
    theta.Set(0, 0, -24.0);
    theta.Set(1, 0, 0.2);
    theta.Set(2, 0, 0.2);

    GradientDescent<double> gd;
    gd.SetTrainingSet(dataSet);
    gd.SetLearningRate(0.1);
    //gd.NormalizeFeatures();
    gd.SetDebugLog(true);

    LogisticRegression<double> lr;
    gd.SetRegressionMethod(&lr);

    theta = gd.ComputeTheta(1, theta);
    theta.Log("theta");

    Matrix<double> example;
    example.Init(1, 2);
    example.Set(0, 0, 1650.0f);
    example.Set(0, 1, 3.0f);
    //-24; 0.2; 0.2
    //Matrix<double> answer = gd.Predict(example, theta);
    //answer.Log("prediction");

    clock_t timeElapsed = clock() - startTime;
    printf("Time Taken (ms): %d", timeElapsed);
}

int main()
{
    LinearRegressionTest();
    LogisticRegressionTest();
    return 0;
}

