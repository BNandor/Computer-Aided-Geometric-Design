#ifndef CYCLICCURVES3_H
#define CYCLICCURVES3_H
#include "../Core/LinearCombination3.h"
#include "../Core/Matrices.h"

namespace cagd {
  class CyclicCurve3 : public LinearCombination3{
    protected:
      GLuint _n;
      GLdouble _c_n;
      GLdouble _lambda_n;

      TriangularMatrix<GLdouble> _bc;

      GLdouble _CalculateNormalizingCoefficient(GLuint n);

      GLvoid   _CalculateBinomialCoefficient(GLuint m, TriangularMatrix<GLdouble> &bc);

  public:

      CyclicCurve3(GLuint n, GLenum data_usage_flag = GL_STATIC_DRAW);

      GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const;
      GLboolean CalculateDerivatives(
          GLuint max_order_of_derivatives, GLdouble u, Derivatives &d)const;
      GLboolean UpdateDataForInterpolation(const ColumnMatrix<GLdouble>& knot_vector, const ColumnMatrix<DCoordinate3>& data_points_to_interpolate);
  };
}
#endif // CYCLICCURVES3_H
