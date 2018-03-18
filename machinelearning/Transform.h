#pragma once

class MatrixTransform
{
public:
    template<typename T>
    static Matrix<T> Sigmoid( const Matrix<T>& m )
    {
        Matrix<T> retVal(m);

        for (unsigned int i = 0; i < m.DataSize(); ++i)
        {
            T oldValue = m.Get(i);
            T newValue = 1.0 / (1.0 + exp(-oldValue));
            retVal.Set(i, newValue);
        }

        return retVal;
    }

    template<typename T>
    static Matrix<T> Log(const Matrix<T>& m)
    {
        Matrix<T> retVal(m);

        for (unsigned int i = 0; i < m.DataSize(); ++i)
        {
            T oldValue = m.Get(i);
            T newValue = log( oldValue );
            retVal.Set(i, newValue);
        }

        return retVal;
    }
};
