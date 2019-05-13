#include "RealSquareMatrices.h"

using namespace cagd;
using namespace std;

RealSquareMatrix::RealSquareMatrix(GLuint size):
        Matrix<GLdouble>(size, size),
        _lu_decomposition_is_done(GL_FALSE)
{
}
//mine

RealSquareMatrix::RealSquareMatrix(const RealSquareMatrix& m): Matrix<GLdouble>(m),
  _lu_decomposition_is_done(m._lu_decomposition_is_done),_row_permutation(m._row_permutation){
}


RealSquareMatrix& RealSquareMatrix::operator =(const RealSquareMatrix& rhs){
//  for(GLuint i=0;i<rhs._row_count;i++){
//      for(GLuint j=0;j<rhs._row_count;j++){
//          Matrix<GLdouble>::_data[i][j]=rhs.Matrix<GLdouble>::_data[i][j];
//      }
//  }
  if(this != &rhs){
      Matrix<GLdouble>::operator=(rhs);
    _lu_decomposition_is_done=rhs._lu_decomposition_is_done;
    _row_permutation=rhs._row_permutation;
    }
  return *this;
}

GLboolean RealSquareMatrix::ResizeRows(GLuint row_count){

//  _data.resize(row_count,std::vector<GLdouble>(row_count));
//  for(GLuint i=0;i<row_count;i++){
//    _data[i].resize(row_count);
//  }
//  _row_count=row_count;
//  _column_count=row_count;
  return Matrix<GLdouble>::ResizeRows(row_count) && Matrix<GLdouble>::ResizeColumns(row_count);
}

GLboolean RealSquareMatrix::ResizeColumns(GLuint row_count){
//  _data.resize(row_count,std::vector<GLdouble>(row_count));
//  for(GLuint i=0;i<row_count;i++){
//    _data[i].resize(row_count);
//  }
//  _row_count=row_count;
//  _column_count=row_count;
//  return true;
  return Matrix<GLdouble>::ResizeRows(row_count) && Matrix<GLdouble>::ResizeColumns(row_count);
}
//EOF mine

GLboolean RealSquareMatrix::PerformLUDecomposition()
{
    if (_lu_decomposition_is_done)
        return GL_TRUE;

    if (_row_count <= 1)
        return GL_FALSE;

    const GLdouble tiny = numeric_limits<GLdouble>::min();

    GLuint size = (GLuint)_data.size();
    vector<GLdouble> implicit_scaling_of_each_row(size);

    _row_permutation.resize(size);

    GLdouble row_interchanges = 1.0;

    //-------------------------------------------------------
    // loop over rows to get the implicit scaling information
    //-------------------------------------------------------
    vector<GLdouble>::iterator its = implicit_scaling_of_each_row.begin();
    for (vector<vector<GLdouble> >::const_iterator itr = _data.begin(); itr < _data.end(); ++itr)
    {
        GLdouble big = 0.0;
        for (vector<GLdouble>::const_iterator itc = itr->begin(); itc < itr->end(); ++itc)
        {
            GLdouble temp = abs(*itc);
            if (temp > big)
                    big = temp;
        }

        if (big == 0.0)
        {
            // the matrix is singular
            return GL_FALSE;
        }
        *its = 1.0 / big;
        ++its;
    }

    //-----------------------------------
    // search for the largest pivot element
    //-----------------------------------
    for (GLuint k = 0; k < size; ++k)
    {
        GLuint imax = k;
        GLdouble big = 0.0;
        for (GLuint i = k; i < size; ++i)
        {
            GLdouble temp = implicit_scaling_of_each_row[i] * abs(_data[i][k]);
            if (temp > big)
            {
                big = temp;
                imax = i;
            }
        }

        // do we need to interchange rows?
        if (k != imax)
        {
            for (GLuint j = 0; j < size; ++j)
            {
                GLdouble temp = _data[imax][j];
                _data[imax][j] = _data[k][j];
                _data[k][j] = temp;
            }
            // change the parity of row_interchanges
            row_interchanges = -row_interchanges;
            // also interchange the scale factor
            implicit_scaling_of_each_row[imax] = implicit_scaling_of_each_row[k];
        }

        _row_permutation[k] = imax;
        if (_data[k][k] == 0.0)
            _data[k][k] = tiny;

        for (GLuint i = k + 1; i < size; ++i)
        {
            // divide by pivot element
            GLdouble temp = _data[i][k] /= _data[k][k];

            // reduce remaining submatrix
            for (GLuint j = k + 1; j < size; ++j)
                _data[i][j] -= temp * _data[k][j];
        }
    }

    _lu_decomposition_is_done = GL_TRUE;

    return GL_TRUE;
}
