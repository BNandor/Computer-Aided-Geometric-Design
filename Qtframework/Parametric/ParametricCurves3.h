#pragma once

#include "../Core/DCoordinates3.h"
#include "../Core/GenericCurves3.h"
#include "../Core/Matrices.h"

namespace cagd
{
    //-----------------------
    // class ParametricCurve3
    //-----------------------
    class ParametricCurve3
    {
    public:
        typedef DCoordinate3 (*Derivative)(GLdouble);

    private:
        // definition domain
        GLdouble _u_min, _u_max;

        // derivatives of coordinate functions
        RowMatrix<Derivative> _derivatives;

    public:
        // special constructor
        ParametricCurve3(const RowMatrix<Derivative>& derivatives, GLdouble u_min, GLdouble u_max);

        // calculate derivative at the parameter value u
        DCoordinate3 operator ()(GLuint order, GLdouble u) const;

        // generate image/arc
        GenericCurve3* GenerateImage(GLuint div_point_count, GLenum usage_flag = GL_STATIC_DRAW) const;

        // set/get definition domain
        GLvoid SetDefinitionDomain(GLdouble u_min, GLdouble u_max);
        GLvoid GetDefinitionDomain(GLdouble& u_min, GLdouble& u_max) const;

        // set derivatives
        GLvoid SetDerivatives(const RowMatrix<Derivative>& derivatives);
    };
}
