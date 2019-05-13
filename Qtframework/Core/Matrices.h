#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>

namespace cagd
{
    // forward declaration of template class Matrix
    template <typename T>
    class Matrix;

    // forward declaration of template class RowMatrix
    template <typename T>
    class RowMatrix;

    // forward declaration of template class ColumnMatrix
    template <typename T>
    class ColumnMatrix;

	// forward declaration of template class TriangularMatrix
    template <typename T>
    class TriangularMatrix;

    // forward declarations of overloaded and templated input/output from/to stream operators
    template <typename T>
    std::ostream& operator << (std::ostream& lhs, const Matrix<T>& rhs);

    template <typename T>
    std::istream& operator >>(std::istream& lhs, Matrix<T>& rhs);

	template <typename T>
    std::istream& operator >>(std::istream& lhs, TriangularMatrix<T>& rhs);
    
    template <typename T>
    std::ostream& operator << (std::ostream& lhs, const TriangularMatrix<T>& rhs);

    //----------------------
    // template class Matrix
    //----------------------
    template <typename T>
    class Matrix
    {
        friend std::ostream& cagd::operator << <T>(std::ostream&, const Matrix<T>& rhs);
        friend std::istream& cagd::operator >> <T>(std::istream&, Matrix<T>& rhs);

    protected:
        GLuint                          _row_count;
        GLuint                          _column_count;
        std::vector< std::vector<T> >   _data;
    public:
        // special constructor (can also be used as a default constructor)
        Matrix(GLuint row_count = 1, GLuint column_count = 1);//_data(_row_count, std::vector<T> (_column_count))

        // copy constructor
        Matrix(const Matrix& m);

        // assignment operator
        Matrix& operator =(const Matrix& m);

        // get element by reference
        T& operator ()(GLuint row, GLuint column);

        // get copy of an element
        T operator ()(GLuint row, GLuint column) const;

        // get dimensions
        GLuint GetRowCount() const;
        GLuint GetColumnCount() const;

        // set dimensions
        virtual GLboolean ResizeRows(GLuint row_count);
        virtual GLboolean ResizeColumns(GLuint column_count);

        // update
        GLboolean SetRow(GLuint index, const RowMatrix<T>& row);
        GLboolean SetColumn(GLuint index, const ColumnMatrix<T>& column);

        // destructor
        virtual ~Matrix();
    };

    //-------------------------
    // template class RowMatrix
    //-------------------------
    template <typename T>
    class RowMatrix: public Matrix<T>
    {
    public:
        // special constructor (can also be used as a default constructor)
        RowMatrix(GLuint column_count = 1);

        // get element by reference
        T& operator ()(GLuint column);
        T& operator [](GLuint column);

        // get copy of an element
        T operator ()(GLuint column) const;
        T operator [](GLuint column) const;

        // a row matrix consists of a single row
        GLboolean ResizeRows(GLuint row_count);
    };

    //----------------------------
    // template class ColumnMatrix
    //----------------------------
    template <typename T>
    class ColumnMatrix: public Matrix<T>
    {
    public:
        // special constructor (can also be used as a default constructor)
        ColumnMatrix(GLuint row_count = 1);

        // get element by reference
        T& operator ()(GLuint row);
        T& operator [](GLuint row);

        // get copy of an element
        T operator ()(GLuint row) const;
        T operator [](GLuint row) const;

        // a column matrix consists of a single column
        GLboolean ResizeColumns(GLuint column_count);
    };


    //--------------------------------
    // template class TriangularMatrix
    //--------------------------------

    template <typename T>
    class TriangularMatrix
    {
        friend std::istream& cagd::operator >> <T>(std::istream&, TriangularMatrix<T>& rhs);
        friend std::ostream& cagd::operator << <T>(std::ostream&, const TriangularMatrix<T>& rhs);

    protected:
        GLuint                        _row_count;
        std::vector< std::vector<T> > _data;

    public:
        // special constructor (can also be used as a default constructor)
        TriangularMatrix(GLuint row_count = 1);

        // get element by reference
        T& operator ()(GLuint row, GLuint column);

        // get copy of an element
        T operator ()(GLuint row, GLuint column) const;

        // get dimension
        GLuint GetRowCount() const;

        // set dimension
        GLboolean ResizeRows(GLuint row_count);
    };

    //--------------------------------------------------
    // homework: implementation of template class Matrix
    //--------------------------------------------------
      template <typename T>
      Matrix<T>::Matrix(GLuint row_count , GLuint column_count):_row_count(row_count),_column_count(column_count),_data(row_count, std::vector<T> (column_count)){}

      template <typename T>
      Matrix<T>::Matrix(const Matrix& m):_row_count(m._row_count),_column_count(m._column_count), _data(m._data){//lehet olyat tenni, hogy _data=m._data ?
        _data=m._data;
      }

      template <typename T>//?Ha nem ugyanakkorak, akkor meg kell cserelni a mereteket ?
      Matrix<T>& Matrix<T>::operator =(const Matrix& m){
        if(this != &m){
          _row_count=m._row_count;
          _column_count=m._column_count;
          _data=m._data;          
        }
        return *this;
      }
      template <typename T>
      T& Matrix<T>::operator ()(GLuint row, GLuint column){
          return _data[row][column];
      }
      template <typename T>
      T Matrix<T>::operator ()(GLuint row, GLuint column) const{
          return _data[row][column];
      }

      // get dimensions
      template <typename T>
      GLuint Matrix<T>::GetRowCount() const{
        return _row_count;
      }

      template <typename T>
      GLuint Matrix<T>::GetColumnCount() const{
        return _column_count;
      }

      // set dimensions
      template <typename T>//Milyen bool erteket kell visszateriteni ?
      GLboolean Matrix<T>::ResizeRows(GLuint row_count){
          _row_count = row_count;
           _data.resize(_row_count,std::vector<T>(_column_count));//le kell nullazni az uj sorokat ? es ha a T nem szam ?
          return GL_TRUE;
      }

      template <typename T>//Milyen bool erteket kell visszateriteni ?
      GLboolean Matrix<T>::ResizeColumns(GLuint column_count){
        _column_count = column_count;
        for(GLuint i=0;i<_row_count;i++){
            _data[i].resize(_column_count); //le kell nullazni az uj oszlopokat ? es ha a T nem szam ?
        }
        return GL_TRUE;
      }

      // update
      template  <typename T>
      GLboolean Matrix<T>::SetRow(GLuint index, const RowMatrix<T>& row){
        if(index>=_row_count||row._column_count!=_column_count){
         return GL_FALSE;
        }

          _data[index]=row._data[0];
          return GL_TRUE;
      }

      template  <typename T>
      GLboolean Matrix<T>::SetColumn(GLuint index, const ColumnMatrix<T>& column){

        if( index>=_column_count || _row_count!=column._row_count){
            return GL_FALSE;
        }

        for(int i=0;i<_row_count;i++){
            _data[i][index]=column._data[i][0];
          }
        return GL_TRUE;
      }

      // destructor
      template <typename T>
      Matrix<T>::~Matrix(){
        _data.clear();
      }
    //-----------------------------------------------------
    // homework: implementation of template class RowMatrix
    //-----------------------------------------------------

      // special constructor (can also be used as a default constructor)
      template <typename T>
      RowMatrix<T>::RowMatrix(GLuint column_count):Matrix<T>(1,column_count){

      }

      // get element by reference
      template <typename T>
      T& RowMatrix<T>::operator ()(GLuint column){
          return Matrix<T>::_data[0][column];
      }

      template <typename T>
      T& RowMatrix<T>::operator [](GLuint column){
        return Matrix<T>::_data[0][column];
      }

      template <typename T>
      T RowMatrix<T>::operator ()(GLuint column) const{
        return Matrix<T>::_data[0][column];
      }
      template <typename T>
      T RowMatrix<T>::operator [](GLuint column) const{
        return Matrix<T>::_data[0][column];
      }

      // a row matrix consists of a single row
      template <typename T>
      GLboolean RowMatrix<T>::ResizeRows(GLuint row_count){//itt most akkor tobb sort kell letrehozni, vagy oszlopokat kell novelni ?          
          return (row_count == 1);
      }

    //--------------------------------------------------------
    // homework: implementation of template class ColumnMatrix
    //--------------------------------------------------------
      template <typename T>
      ColumnMatrix<T>::ColumnMatrix(GLuint row_count):Matrix<T>(row_count,1){}

      // get element by reference
      template <typename T>
      T& ColumnMatrix<T>::operator ()(GLuint row){
        return Matrix<T>::_data[row][0];
      }
      template <typename T>
      T& ColumnMatrix<T>::operator [](GLuint row){
        return Matrix<T>::_data[row][0];
      }

      // get copy of an element
      template <typename T>
      T ColumnMatrix<T>::operator ()(GLuint row) const{
        return Matrix<T>::_data[row][0];
      }

      template <typename T>
      T ColumnMatrix<T>::operator [](GLuint row) const{
        return Matrix<T>::_data[row][0];
      }

      // a column matrix consists of a single column
      template <typename T>
      GLboolean ColumnMatrix<T>::ResizeColumns(GLuint column_count){//Itt nem a sorokat akarjuk vajon valtoztatni, marmint, ha megvaltoztatjuk mar nem lesz oszlopmatrix       
        return (column_count == 1);
      }
    //------------------------------------------------------------
    // homework: implementation of template class TriangularMatrix
    //------------------------------------------------------------

      // special constructor (can also be used as a default constructor)
      template <typename T>
      TriangularMatrix<T>::TriangularMatrix(GLuint row_count ):_row_count(row_count),_data(row_count){
        for(int i=0;i<_row_count;i++){
            _data[i].resize(i+1);
        }
      }

      // get element by reference
      template <typename T>
      T& TriangularMatrix<T>::operator ()(GLuint row, GLuint column){
        return _data[row][column];
      }

      // get copy of an element
      template <typename T>
      T TriangularMatrix<T>::operator ()(GLuint row, GLuint column) const{
        return _data[row][column];
      }

      // get dimension
      template <typename T>
      GLuint TriangularMatrix<T>::GetRowCount() const{
        return _row_count;
      }

      // set dimension
      template <typename T>
      GLboolean TriangularMatrix<T>::ResizeRows(GLuint row_count){//Milyen boolean erteket kell teriteni ?
        _data.resize(row_count);
        for(GLuint i=_row_count;i<row_count;i++){
            _data[i].resize(i+1);
        }
        _row_count=row_count;
        return GL_TRUE;
      }
    //------------------------------------------------------------------------------
    // definitions of overloaded and templated input/output from/to stream operators
    //------------------------------------------------------------------------------

    // output to stream
    template <typename T>//todo resize matrices on input
    std::ostream& operator <<(std::ostream& lhs, const Matrix<T>& rhs)
    {
        lhs << rhs._row_count << " " << rhs._column_count << std::endl;
        for (typename std::vector< std::vector<T> >::const_iterator row = rhs._data.begin();
             row != rhs._data.end(); ++row)
        {
            for (typename std::vector<T>::const_iterator column = row->begin();
                 column != row->end(); ++column)
                    lhs << *column << " ";
            lhs << std::endl;
        }
        return lhs;
    }
    template <typename T>
    std::ostream& operator <<(std::ostream& lhs, const TriangularMatrix<T>& rhs){
      lhs << rhs._row_count  << std::endl;
      for (typename std::vector< std::vector<T> >::const_iterator row = rhs._data.begin();
           row != rhs._data.end(); ++row)
      {
          for (typename std::vector<T>::const_iterator column = row->begin();
               column != row->end(); ++column)
                  lhs << *column << " ";
          lhs << std::endl;
      }
      return lhs;
    }

    // input from stream
    template <typename T>
    std::istream& operator >>(std::istream& lhs, Matrix<T>& rhs)
    {
        // homework sima iteratorokat hasznalunk, a kiiratas szimmetrikusa kell
        lhs >> rhs._row_count  >> rhs._column_count;
        rhs.ResizeRows(rhs._row_count);
        rhs.ResizeColumns(rhs._column_count);
        for (typename std::vector< std::vector<T> >::iterator row = rhs._data.begin();
             row != rhs._data.end(); ++row)
        {
            for (typename std::vector<T>::iterator column = row->begin();
                 column != row->end(); ++column)
              lhs >> *column;
        }
        return lhs;
   }

    template <typename T>
    std::istream& operator >> (std::istream& lhs, TriangularMatrix<T>& rhs){
      lhs >> rhs._row_count;//resize matrix
      rhs.ResizeRows(rhs._row_count);
      for (typename std::vector< std::vector<T> >::iterator row = rhs._data.begin();
           row != rhs._data.end(); ++row)
      {
          for (typename std::vector<T>::iterator column = row->begin();
               column != row->end(); ++column)
            lhs >> *column;
      }
      return lhs;
    }
}
