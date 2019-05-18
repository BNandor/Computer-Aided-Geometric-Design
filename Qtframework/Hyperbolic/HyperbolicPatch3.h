#ifndef HYPERBOLICPATCH3_H
#define HYPERBOLICPATCH3_H
#include "../Core/TensorProductSurfaces3.h"
using namespace  cagd;
class HyperbolicPatch3:public TensorProductSurface3{
private:
  GLdouble _alpha;
  GLdouble sinh(GLdouble x)const;
  GLdouble cosh(GLdouble x)const;
  RowMatrix<GLdouble> constants;
  void updateConstants();

  GLdouble F3firstDerivative(GLdouble t)const;
  GLdouble F2firstDerivative(GLdouble t)const;
  GLdouble F1firstDerivative(GLdouble t)const;
  GLdouble F0firstDerivative(GLdouble t)const;

  GLdouble F3secondDerivative(GLdouble t)const;
  GLdouble F2secondDerivative(GLdouble t)const;
  GLdouble F1secondDerivative(GLdouble t)const;
  GLdouble F0secondDerivative(GLdouble t)const;

  GLdouble G3firstDerivative(GLdouble t)const;
  GLdouble G2firstDerivative(GLdouble t)const;
  GLdouble G1firstDerivative(GLdouble t)const;
  GLdouble G0firstDerivative(GLdouble t)const;

  GLdouble G3secondDerivative(GLdouble t)const;
  GLdouble G2secondDerivative(GLdouble t)const;
  GLdouble G1secondDerivative(GLdouble t)const;
  GLdouble G0secondDerivative(GLdouble t)const;
public:
  HyperbolicPatch3(GLdouble alpha):TensorProductSurface3(0, alpha,0,alpha),_alpha(alpha){
    updateConstants();
  }
  HyperbolicPatch3(const HyperbolicPatch3& other):TensorProductSurface3(other),_alpha(other._alpha){}
  virtual GLboolean UBlendingFunctionValues(
          GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const;

  virtual GLboolean VBlendingFunctionValues(
          GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const;

  // calculates the point and higher order (mixed) partial derivatives of the
  // tensor product surface
  //
  // $\mathbf{s}(u, v) = \sum_{i=0}^{n} \sum_{j = 0}^{m} \mathbf{p}_{i,j} F_{n,i}(u) G_{m,j}(v)$,
  //
  // where $n+1$ and $m+1$ denote the row and column counts of the matrix _data, respectively, while
  // $\left(u, v\right) \in \left[u_{\min}, u_{\max}\right] \times \left[v_{\min}, v_{\max}\right]$
  virtual GLboolean CalculatePartialDerivatives(
          GLuint maximum_order_of_partial_derivatives,
          GLdouble u, GLdouble v, PartialDerivatives& pd) const;
  void setAlpha(GLdouble);
  GLdouble GetAlpha(){return _alpha;}
};
#endif // HYPERBOLICPATCH3_H
