#include "HyperbolicArc3.h"
#include "../Core/Constants.h"

using namespace  std;
using namespace cagd;
GLdouble HyperbolicArc3::sinh(GLdouble x)const{
  return 0.5*(pow(E,x) - pow(E,-x));
}
GLdouble HyperbolicArc3::cosh(GLdouble x)const{
  return 0.5*(pow(E,x) + pow(E,-x));
}

void HyperbolicArc3::updateConstants(){
  constants[0] = 4.0*cosh(_alpha/2.0);
  constants[1] = pow(sinh(_alpha/2.0),4);
  constants[2] = 1.0 + 2.0 * pow(cosh(_alpha/2.0),2);  
}

GLboolean HyperbolicArc3::BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble>& values)const{
  if(u< _u_min || u> _u_max ){
      return GL_FALSE;
  }
  values.ResizeColumns(4);
  values[3] = pow(sinh(u/2.0),4)/constants[1];
  values[0] = pow(sinh((_alpha - u)/2.0),4)/constants[1];
  values[2] = (constants[0]/constants[1])*sinh((_alpha - u)/2.0)*pow(sinh(u/2.0),3)
              + (constants[2]/constants[1])*pow(sinh((_alpha-u)/2.0),2)*pow(sinh(u/2.0),2);
  values[1] = (constants[0]/constants[1])*sinh((u)/2.0)*pow(sinh((_alpha-u)/2.0),3)
      + (constants[2]/constants[1])*pow(sinh((u)/2.0),2)*pow(sinh((_alpha-u)/2.0),2);
  return GL_TRUE;
}
GLdouble HyperbolicArc3::F3firstDerivative(GLdouble t)const{
  return (2.0/constants[1])*pow(sinh(t/2.0),3)*cosh(t/2.0);
}

GLdouble HyperbolicArc3::F2firstDerivative(GLdouble t)const{
  return (constants[0]/constants[1])
      *(-0.5*cosh((_alpha - t)/2.0)*pow(sinh(t/2.0),3)
          + 1.5*sinh((_alpha - t)/2.0)*pow(sinh(t/2.0),2)*cosh(t/2.0) )
      +(constants[2]/constants[1])*(
        pow(sinh((_alpha - t)/2.0),2)*cosh(t/2.0)*sinh(t/2.0)
      -cosh((_alpha - t)/2.0)*sinh((_alpha - t)/2.0)*pow(sinh(t/2.0),2)
      );
}
GLdouble HyperbolicArc3::F0firstDerivative(GLdouble t)const{
  return -F3firstDerivative(_alpha - t);
}
GLdouble HyperbolicArc3::F1firstDerivative(GLdouble t)const{
  return -F2firstDerivative(_alpha - t);
}

GLdouble HyperbolicArc3::F3secondDerivative(GLdouble t)const{
  return (1/constants[1])*((pow(sinh(t/2.0),4) + 3.0*pow(cosh(t/2.0),2)*pow(sinh(t/2.0),2)));
}
GLdouble HyperbolicArc3::F2secondDerivative(GLdouble t)const{
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
GLdouble HyperbolicArc3::F1secondDerivative(GLdouble t)const{
  return F2secondDerivative(_alpha - t);
}
GLdouble HyperbolicArc3::F0secondDerivative(GLdouble t)const{
  return F3secondDerivative(_alpha - t);
}
GLboolean HyperbolicArc3::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives& d)const{
  if(u< _u_min || u> _u_max || max_order_of_derivatives >2){
    return GL_FALSE;
   }
  d.ResizeRows(max_order_of_derivatives+1);
  d.LoadNullVectors();
  RowMatrix<GLdouble> fvals;
  BlendingFunctionValues(u,fvals);
  for (int i=0;i<4;i++) {//calculate curve points, 0-th order derivatives
    d[0]+=_data[i]*fvals[i];
  }
  d[1]+=_data[0]*F0firstDerivative(u);
  d[1]+=_data[1]*F1firstDerivative(u);
  d[1]+=_data[2]*F2firstDerivative(u);
  d[1]+=_data[3]*F3firstDerivative(u);

  if(max_order_of_derivatives>=2){
    d[2]+=_data[0]*F0secondDerivative(u);
    d[2]+=_data[1]*F1secondDerivative(u);
    d[2]+=_data[2]*F2secondDerivative(u);
    d[2]+=_data[3]*F3secondDerivative(u);
  }
  return GL_TRUE;
}
void HyperbolicArc3::setAlpha(GLdouble alpha){
  _alpha=alpha;
  _u_max=_alpha;
  updateConstants();
}
