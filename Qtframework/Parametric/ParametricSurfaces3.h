#pragma once

#include <GL/glew.h>
#include "../Core/DCoordinates3.h"
#include "../Core/Matrices.h"
#include "../Core/TriangulatedMeshes3.h"

namespace cagd
{
    class ParametricSurface3
    {
    public:
        // function pointer definition to the zeroth, first or higher order partial derivatives
        // of the parametric surface
        typedef DCoordinate3 (*PartialDerivative)(GLdouble, GLdouble);

    protected:
        TriangularMatrix<PartialDerivative> _pd;    // function pointers
        GLdouble _u_min, _u_max;                    // definition domain in direction u
        GLdouble _v_min, _v_max;                    // definition domain in direction v

    public:
        // special constructor
        ParametricSurface3(
                const TriangularMatrix<PartialDerivative> &pd,
                GLdouble u_min, GLdouble u_max,
                GLdouble v_min, GLdouble v_max);

        // generates the approximated tesselated image of the parametric surface
        TriangulatedMesh3* GenerateImage(
                GLuint u_div_point_count,           // number of subdivision points in direction u
                GLuint v_div_point_count,           // number of subdivision points in direction v
                GLenum usage_flag = GL_STATIC_DRAW) const;
    };
}
