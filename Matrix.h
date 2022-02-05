#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <mutex>
#include <thread>
/*
class computes 2D x 2D matrice multiplication

*/
template <typename T>
class Matrix
{


  public:
      Matrix(unsigned rows, unsigned cols, const int& initial);
//    copy constructor
      Matrix(const Matrix<T>& rhs);
//    defualt constructor with defualt initialisation to maintain invariance
      Matrix();

      virtual ~Matrix() = default;

      Matrix<T>&    operator=(const Matrix<T>& rhs);

      // Matrix multiplication operations
      Matrix<T>     operator*(const Matrix<T>& rhs);
      Matrix<T>&    operator*=(const Matrix<T>& rhs);
      Matrix<T>     operator^(const Matrix<T>& rhs);
      Matrix<T>&    operator^=(const Matrix<T>& rhs);

      // Access individual elements
      T&            operator()(const unsigned& row, const unsigned& col);
      const T&      operator()(const unsigned& row, const unsigned& col) const;

      // Access row and column sizes
      unsigned      get_rows() const;
      unsigned      get_cols() const;

  private:
      std::vector<std::vector<T> > mat;
      unsigned      m_rows;
      unsigned      m_cols;
      std::mutex    m_mutex;

};
#include "Matrix.cpp"
#endif

