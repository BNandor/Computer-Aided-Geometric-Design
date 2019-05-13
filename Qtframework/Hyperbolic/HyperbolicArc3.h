#ifndef HYPERBOLICARC3_H
#define HYPERBOLICARC3_H
#include "../Core/LinearCombination3.h"
#include "../Core/Matrices.h"

using namespace cagd;
using namespace  std;
class HyperbolicArc3:public LinearCombination3{
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
public :
    HyperbolicArc3(GLdouble alpha):LinearCombination3(0,alpha,4),_alpha(alpha),constants(3){
      updateConstants();
    }
    HyperbolicArc3(const HyperbolicArc3& other):LinearCombination3(other),_alpha(other._alpha),constants(3){
    }
    virtual GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble>& values)const;
    virtual GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives& d)const;
    void setAlpha(GLdouble);
    GLdouble getAlpha(){return _alpha;}
};
#endif // HYPERBOLICARC3_H
