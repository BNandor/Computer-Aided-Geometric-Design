#include "HyperbolicPatch3.h"
#include "../Core/Constants.h"

using namespace  std;
using namespace cagd;

GLdouble HyperbolicPatch3::sinh(GLdouble x)const{
  return 0.5*(pow(E,x) - pow(E,-x));
}
GLdouble HyperbolicPatch3::cosh(GLdouble x)const{
  return 0.5*(pow(E,x) + pow(E,-x));
}

void HyperbolicPatch3::updateConstants(){
  constants[0] = 4.0*cosh(_alpha/2.0);
  constants[1] = pow(sinh(_alpha/2.0),4);
  constants[2] = 1.0 + 2.0 * pow(cosh(_alpha/2.0),2);
}
GLdouble HyperbolicPatch3::F3firstDerivative(GLdouble t)const{
  return (2.0/constants[1])*pow(sinh(t/2.0),3)*cosh(t/2.0);
}

GLdouble HyperbolicPatch3::F2firstDerivative(GLdouble t)const{
  return (constants[0]/constants[1])
      *(-0.5*cosh((_alpha - t)/2.0)*pow(sinh(t/2.0),3)
          + 1.5*sinh((_alpha - t)/2.0)*pow(sinh(t/2.0),2)*cosh(t/2.0) )
      +(constants[2]/constants[1])*(
        pow(sinh((_alpha - t)/2.0),2)*cosh(t/2.0)*sinh(t/2.0)
      -cosh((_alpha - t)/2.0)*sinh((_alpha - t)/2.0)*pow(sinh(t/2.0),2)
      );
}
GLdouble HyperbolicPatch3::F0firstDerivative(GLdouble t)const{
  return -F3firstDerivative(_alpha - t);
}
GLdouble HyperbolicPatch3::F1firstDerivative(GLdouble t)const{
  return -F2firstDerivative(_alpha - t);
}

GLdouble HyperbolicPatch3::F3secondDerivative(GLdouble t)const{
  return (1/constants[1])*((pow(sinh(t/2.0),4) + 3.0*pow(cosh(t/2.0),2)*pow(sinh(t/2.0),2)));
}
GLdouble HyperbolicPatch3::F2secondDerivative(GLdouble t)const{
  return 0.5*(2.0*(constants[0]/constants[1])*sinh((_alpha-t)/2.0)*pow(sinh(t/2.0),3) +
              pow(sinh(t/2.0),2)*(
                2.0*(constants[2]/constants[1])*(pow(sinh((_alpha-t)/2.0),2)) + (constants[2]/constants[1])*pow(cosh((_alpha-t)/2.0),2)
                -3.0*(constants[0]/constants[1])*cosh((_alpha-t)/2.0)*cosh(t/2.0)
                )
              +sinh(t/2.0)*(
                3.0*(constants[0]/constants[1])*sinh((_alpha-t)/2.0)*pow(cosh(t/2.0),2)
                -4.0*(constants[2]/constants[1])*cosh((_alpha-t)/2.0)*sinh((_alpha-t)/2.0)*cosh(t/2.0)
                )
              +(constants[2]/constants[1])*pow(sinh((_alpha-t)/2.0),2)*pow(cosh(t/2.0),2)
          );
}
GLdouble HyperbolicPatch3::F1secondDerivative(GLdouble t)const{
  return F2secondDerivative(_alpha - t);
}
GLdouble HyperbolicPatch3::F0secondDerivative(GLdouble t)const{
  return F3secondDerivative(_alpha - t);
}

GLboolean HyperbolicPatch3::UBlendingFunctionValues(
    GLdouble u, RowMatrix<GLdouble>& blending_values) const{
  if(u< _u_min || u > _u_max ){
      return GL_FALSE;
  }
  blending_values.ResizeColumns(4);
  blending_values[3] = pow(sinh(u/2.0),4)/constants[1];
  blending_values[0] = pow(sinh((_alpha - u)/2.0),4)/constants[1];
  blending_values[2] = (constants[0]/constants[1])*sinh((_alpha - u)/2.0)*pow(sinh(u/2.0),3)
              + (constants[2]/constants[1])*pow(sinh((_alpha-u)/2.0),2)*pow(sinh(u/2.0),2);
  blending_values[1] = (constants[0]/constants[1])*sinh((u)/2.0)*pow(sinh((_alpha-u)/2.0),3)
      + (constants[2]/constants[1])*pow(sinh((u)/2.0),2)*pow(sinh((_alpha-u)/2.0),2);
  return GL_TRUE;
}

GLboolean HyperbolicPatch3::VBlendingFunctionValues(
    GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const{
  return UBlendingFunctionValues(v_knot,blending_values);
}

GLdouble  HyperbolicPatch3::G3firstDerivative(GLdouble t)const{
  return F3firstDerivative(t);
}

GLdouble HyperbolicPatch3::G2firstDerivative(GLdouble t)const{
  return F2firstDerivative(t);
}
GLdouble HyperbolicPatch3::G1firstDerivative(GLdouble t)const{
  return F1firstDerivative(t);
}
GLdouble HyperbolicPatch3::G0firstDerivative(GLdouble t)const{
  return F0firstDerivative(t);
}

GLdouble HyperbolicPatch3::G3secondDerivative(GLdouble t)const{
  return F3secondDerivative(t);
}
GLdouble HyperbolicPatch3::G2secondDerivative(GLdouble t)const{
  return F2secondDerivative(t);
}
GLdouble HyperbolicPatch3::G1secondDerivative(GLdouble t)const{
  return F1secondDerivative(t);
}
GLdouble HyperbolicPatch3::G0secondDerivative(GLdouble t)const{
  return F0secondDerivative(t);
}

GLboolean HyperbolicPatch3::CalculatePartialDerivatives(
        GLuint maximum_order_of_partial_derivatives,
    GLdouble u, GLdouble v, PartialDerivatives& pd) const{
  if(u < 0 || u > _alpha || v < 0 || v > _alpha ){
      return GL_FALSE;
  }
  RowMatrix<GLdouble> u_blending_values(4),d1_u_blending_values(4);
  UBlendingFunctionValues(u,u_blending_values);
  d1_u_blending_values[0] =F0firstDerivative(u);
  d1_u_blending_values[1] =F1firstDerivative(u);
  d1_u_blending_values[2] =F2firstDerivative(u);
  d1_u_blending_values[3] =F3firstDerivative(u);

  RowMatrix<GLdouble> v_blending_values(4),d1_v_blending_values(4);
  VBlendingFunctionValues(v,v_blending_values);
  d1_v_blending_values[0] =G0firstDerivative(v);
  d1_v_blending_values[1] =G1firstDerivative(v);
  d1_v_blending_values[2] =G2firstDerivative(v);
  d1_v_blending_values[3] =G3firstDerivative(v);

  pd.ResizeRows(2);
  pd.LoadNullVectors();
  for (GLuint row=0;row < 4;++row) {
      DCoordinate3 aux_d0_v,aux_d1_v;
      for (GLuint column=0;column< 4;++column) {
        aux_d0_v += _data(row,column) * v_blending_values(column);
        aux_d1_v += _data(row,column) * d1_v_blending_values(column);
      }
      pd(0,0) += aux_d0_v * u_blending_values(row);
      pd(1,0) += aux_d0_v * d1_u_blending_values(row);
      pd(1,1) += aux_d1_v * u_blending_values(row);
  }
  return GL_TRUE;
}

void HyperbolicPatch3::setAlpha(GLdouble alpha){
  _alpha=alpha;
  updateConstants();
}
