#include "ParametricCurves3.h"

using namespace cagd;
using namespace std;

//-----------------------------------------
// implementation of class ParametricCurve3
//-----------------------------------------

// special constructor
ParametricCurve3::ParametricCurve3(const RowMatrix<Derivative>& derivatives, GLdouble u_min, GLdouble u_max):
        _u_min(u_min), _u_max(u_max), _derivatives(derivatives)
{
}

// calculate derivative at parameter u:
// 0th order derivative corresponds to the curve point at parameter u
// 1st order derivative corresponds to the tangent vector at parameter u
// 2nd order derivative corresponds to the acceleration vector at parameter u
// etc.
DCoordinate3 ParametricCurve3::operator ()(GLuint order, GLdouble u) const
{
    return _derivatives[order](u);
}

// generate image of the parametric curve
GenericCurve3* ParametricCurve3::GenerateImage(GLuint div_point_count, GLenum usage_flag) const
{
    GenericCurve3* result = nullptr;

    result = new GenericCurve3(_derivatives.GetColumnCount() - 1, div_point_count, usage_flag);

    if (!result)
	{
        return nullptr;
	}

    // set derivatives at the endpoints of the parametric curve
    for (GLuint order = 0; order < _derivatives.GetColumnCount(); ++order)
    {
        (*result)(order, 0) = _derivatives[order](_u_min);
        (*result)(order, div_point_count - 1) = _derivatives[order](_u_max);
    }

    // calculate derivatives at inner curve points
    GLdouble u_step = (_u_max - _u_min) / (div_point_count - 1);
    GLdouble u = _u_min;

    for (GLuint i = 1; i < div_point_count - 1; i++)
    {
        u += u_step;

        for (GLuint order = 0; order < _derivatives.GetColumnCount(); ++order)
        {
            (*result)(order, i) = _derivatives[order](u);
        }
    }

    return result;
}

// set/get definition domain
GLvoid ParametricCurve3::SetDefinitionDomain(GLdouble u_min, GLdouble u_max)
{
    _u_min=u_min;
    _u_max=u_max;
}

GLvoid ParametricCurve3::GetDefinitionDomain(GLdouble& u_min, GLdouble& u_max) const
{
    u_min=_u_min;
    u_max=_u_max;
}

// set derivatives
GLvoid ParametricCurve3::SetDerivatives(const RowMatrix<Derivative> &derivatives)
{
    _derivatives=derivatives;
}
