#pragma once

#include <vector>
#include <algorithm>
#include <assert.h>

template<typename T> class Row;
template<typename T> class Column;

template<typename T> T operator*(const Row<T>& row, const Column<T>& column)
{
    unsigned int numElements = row.NumElements();
    T currentValue = 0.0; // TODO: handle other types of elements here and not only numbers

    for (unsigned int i = 0; i < numElements; ++i)
    {
        currentValue += row[i] * column[i];
    }

    return currentValue;
}

//
// Internal representation is just one dimensional
// [row0, row1 ... rowN]
//

template<typename T>
class Matrix
{

public:
    Matrix()
        : m_Rows( 0 )
        , m_Columns( 0 )
    {
    }

    Matrix( const Matrix<T>& other )
    {
        *this = other;
    }

    Matrix(unsigned int rows, unsigned int columns)
    {
        Init(rows, columns);
    }

    Matrix<T>& CombineRight(const Matrix& other)
    {
        unsigned int columnsBefore = m_Columns;

        Matrix<T> newMatrix;
        newMatrix.Init(NumRows(), NumColumns() + other.NumColumns());

        for (unsigned int row = 0; row < newMatrix.NumRows(); ++row)
        {
            for (unsigned int col = 0; col < newMatrix.NumColumns(); ++col)
            {
                if (col < columnsBefore)
                {
                    newMatrix.Set(row, col, Get(row, col));
                }
                else
                {
                    newMatrix.Set(row, col, other.Get(row, col - columnsBefore));
                }
            }
        }

        *this = newMatrix;
        return *this;
    }

    void Init(unsigned int rows, unsigned int columns)
    {
        m_Rows = rows;
        m_Columns = columns;

        m_Data.resize(rows * columns);
    }

    void SetRow(unsigned int row, const std::vector<T>& data)
    {
        assert(m_Columns == data.size());

        for (unsigned int i = 0; i < data.size(); ++i)
        {
            unsigned int index = GetIndex(row, i);
            m_Data[index] = data[i];
        }
    }

    void SetAll(T value)
    {
        std::fill(m_Data.begin(), m_Data.end(), value);
    }

    unsigned int NumRows() const
    {
        return m_Rows;
    }

    unsigned int NumColumns() const
    {
        return m_Columns;
    }

    unsigned int DataSize() const
    {
        return m_Data.size();
    }
    Row<T> GetRow(unsigned int index) const
    {
        return Row<T>(this, index);
    }


    Column<T> GetColumn(unsigned int index) const
    {
        return Column<T>(this, index);
    }

    unsigned int GetIndex(unsigned int row, unsigned int column) const
    {
        return row * m_Columns + column;
    }

    void Set(unsigned int row, unsigned int column, T value )
    {
        Set(GetIndex(row, column), value);
    }

    void Set(unsigned int index, T value)
    {
        m_Data[index] = value;
    }

    T Get(unsigned int row, unsigned int column) const
    {
        return Get( GetIndex(row, column) );
    }

    T Get(unsigned int index) const
    {
        return m_Data[index];
    }

    bool SameSize(const Matrix& other) const
    {
        return NumColumns() == other.NumColumns() && NumRows() == other.NumRows();
    }

    Matrix<T>& Transpose()
    {
        Matrix<T> temp;
        temp.Init(NumColumns(), NumRows());
        
        for( unsigned int row = 0; row < NumRows(); ++row )
        {
            for (unsigned int col = 0; col < NumColumns(); ++col)
            {
                temp.Set(col, row, Get(row, col));
            }
        }

        *this = temp;
        return *this;
    }

    void Sqrt()
    {
        for (unsigned int i = 0; i < m_Data.size(); ++i)
        {
            Set(i, std::sqrt(Get(i)));
        }
    }

    Matrix<T>& DeductRow( const Matrix<T>& rowMatrix )
    {
        assert(rowMatrix.NumColumns() == NumColumns());

        Matrix<T> temp;
        temp.Init(NumRows(), NumColumns());

        for (unsigned int row = 0; row < NumRows(); ++row)
        {
            for (unsigned int col = 0; col < NumColumns(); ++col)
            {
                temp.Set(row, col, Get( row, col ) - rowMatrix.Get(0, col));
            }
        }

        *this = temp;
        return *this;
    }

    Matrix<T>& DivideByRow(const Matrix<T>& rowMatrix)
    {
        assert(rowMatrix.NumColumns() == NumColumns());

        Matrix<T> temp;
        temp.Init(NumRows(), NumColumns());

        for (unsigned int row = 0; row < NumRows(); ++row)
        {
            for (unsigned int col = 0; col < NumColumns(); ++col)
            {
                temp.Set(row, col, Get(row, col) / rowMatrix.Get(0, col));
            }
        }

        *this = temp;
        return *this;
    }

    T Sum()
    {
        T sum = 0.0;
        for (unsigned int index = 0; index < m_Data.size(); ++index)
        {
            sum += Get(index);
        }

        return sum;
    }

    void Log( const std::string name)
    {
        std::string log = name;
        log += ":\n";

        for (unsigned int row = 0; row < NumRows(); ++row)
        {
            log += "[";

            for (unsigned int col = 0; col < NumColumns(); ++col)
            {
                const unsigned int bufferSize = 16;
                char value[bufferSize];
                sprintf_s(&value[0], bufferSize, "%.3f", (float)Get(row, col));
                log += value;

                if (col < NumColumns() - 1)
                {
                    log += ", ";
                }
            }

            log += "]\n";
        }


        printf("%s\n", log.c_str());
    }

    template<typename T> friend Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs);
    template<typename T> friend Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs);
    template<typename T> friend Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs);
    template<typename T> friend Matrix<T> operator*(const Matrix<T>& lhs, T rhs);
    template<typename T> friend Matrix<T> operator*(T rhs, const Matrix<T>& lhs);


    template<typename T> friend Matrix<T> ElementwiseMultiply(const Matrix<T>& lhs, const Matrix<T>& rhs);

    template<typename T> friend class Row;
    template<typename T> friend class Colummn;

private:
    unsigned int m_Rows;
    unsigned int m_Columns;

    std::vector<T> m_Data;
};

template<typename T> Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    assert(lhs.SameSize(rhs));

    Matrix<T> result(lhs.NumRows(), lhs.NumColumns());
    for (unsigned int index = 0; index < lhs.m_Data.size(); ++index)
    {
        lhs.Set(index, lhs.Get(index) + rhs.Get(index));
    }
    
    return result;
}

template<typename T> Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    assert(lhs.SameSize(rhs));

    Matrix<T> result(lhs.NumRows(), lhs.NumColumns());
    for (unsigned int index = 0; index < lhs.m_Data.size(); ++index)
    {
        result.Set(index, lhs.Get(index) - rhs.Get(index));
    }

    return result;
}


template<typename T> Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    Matrix<T> result(lhs.NumRows(), rhs.NumColumns());

    assert(lhs.NumColumns() == rhs.NumRows());

    for (unsigned int r = 0; r < lhs.NumRows(); ++r)
    {
        Row<T> row = lhs.GetRow(r);

        for (unsigned int c = 0; c < rhs.NumColumns(); ++c)
        {
            Column<T> column = rhs.GetColumn(c);

            T value = row * column;

            result.Set(r, c, value);
        }
    }


    return result;
}

template<typename T> Matrix<T> operator*(T lhs, const Matrix<T>& rhs )
{
    return operator*(rhs, lhs);
}

template<typename T> Matrix<T> operator*(const Matrix<T>& lhs, T rhs)
{
    Matrix<T> result(lhs.NumRows(), lhs.NumColumns());
    for (unsigned int index = 0; index < lhs.m_Data.size(); ++index)
    {
        result.Set(index, lhs.Get(index) * rhs);
    }

    return result;
}

template<typename T> Matrix<T> ElementwiseMultiply(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    assert(lhs.SameSize(rhs));

    Matrix<T> result(lhs.NumRows(), lhs.NumColumns());
    for (unsigned int index = 0; index < lhs.m_Data.size(); ++index)
    {
        result.Set(index, lhs.Get(index) * rhs.Get(index));
    }

    return result;
}

template<typename T>
class Row
{
public:
    Row(const Matrix<T>* m, unsigned int index)
        : m_Matrix(m)
        , m_RowIndex(index)
    {
    }
    
    unsigned int NumElements() const 
    {
        return m_Matrix->NumColumns();
    }

    T operator [] (unsigned int index) const
    {
        return m_Matrix->Get(m_RowIndex, index);
    }

private:
    unsigned int m_RowIndex;
    const class Matrix<T>* m_Matrix;
};

template<typename T>
class Column
{
public:
    Column(const Matrix<T>* m, unsigned int index)
        : m_Matrix(m)
        , m_ColIndex(index)
    {
    }

    unsigned int NumElements() const
    {
        return m_Matrix->NumRows();
    }
    
    T operator [] (unsigned int index) const
    {
        return m_Matrix->Get(index, m_ColIndex);
    }

private:
    unsigned int m_ColIndex;

    const class Matrix<T>* m_Matrix;
};