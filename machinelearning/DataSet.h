#pragma once
#include <fstream>
#include "Matrix.h"

template <typename T>
class Example
{
public:
    Matrix<T> m_Data;
};

template<typename T>
class DataSet
{
public:
    DataSet(unsigned int exampleDimension, unsigned int answerDimension)
        : m_ExampleDim(exampleDimension)
        , m_AnswerDim(answerDimension)
    {
    }

    DataSet()
    {
    }

    bool LoadFromFile(const std::string& path)
    {
        std::ifstream file;
        file.open(path);

        if (file)
        {
            std::vector<std::vector<float>> examples;
            std::vector<std::vector<float>> answers;

            unsigned int line = 0;
            float x1;
            char delim;

            while (file >> x1)
            {

                std::vector<float> example;
                example.push_back(x1);
                for (unsigned int i = 1; i < m_ExampleDim; ++i)
                {
                    file >> delim;

                    float x;
                    file >> x;

                    example.push_back(x);
                }

                std::vector<float> answer;

                for (unsigned int i = 0; i < m_AnswerDim; ++i)
                {
                    file >> delim;

                    float y;
                    file >> y;

                    answer.push_back(y);
                }

                examples.push_back(example);
                answers.push_back(answer);
            }
                
            if (!examples.empty())
            {
                m_Examples.Init(examples.size(), m_ExampleDim);
                m_Answers.Init(answers.size(), m_AnswerDim);

                for (unsigned int i = 0; i < examples.size(); ++i)
                {
                    for (unsigned int j = 0; j < m_ExampleDim; ++j)
                    {
                        m_Examples.Set(i, j, examples[i][j]);
                    }

                    for (unsigned int j = 0; j < m_AnswerDim; ++j)
                    {
                        m_Answers.Set(i, j, answers[i][j]);
                    }
                }

                return true;
            }

            file.close();
        }

        return false;
    }

    Matrix<T>& GetAnswers()
    {
        return m_Answers;
    }

    Matrix<T>& GetExamples()
    {
        return m_Examples;
    }

private:
    unsigned int m_ExampleDim;
    unsigned int m_AnswerDim;

    Matrix<T> m_Examples;
    Matrix<T> m_Answers;
};