#pragma once

#include "DCoordinates3.h"
#include <GL/glew.h>
#include "Matrices.h"
#include <iostream>

namespace cagd
{
    //--------------------
    // class GenericCurve3
    //--------------------
    class GenericCurve3
    {
        friend class LinearCombination3;

        //----------------------------
        // input/output from/to stream
        //----------------------------
        friend std::ostream& operator <<(std::ostream& lhs, const GenericCurve3& rhs);
        friend std::istream& operator >>(std::istream& lhs, GenericCurve3& rhs);

    protected:
        GLenum               _usage_flag;
        RowMatrix<GLuint>    _vbo_derivative;
        Matrix<DCoordinate3> _derivative;

    public:
        // default and special constructor
        GenericCurve3(
                GLuint maximum_order_of_derivatives = 2,
                GLuint point_count = 0,
                GLenum usage_flag = GL_STATIC_DRAW);

        // special constructor
        GenericCurve3(const Matrix<DCoordinate3>& derivative, GLenum usage_flag = GL_STATIC_DRAW);

        // copy constructor
        GenericCurve3(const GenericCurve3& curve);

        // assignment operator
        GenericCurve3& operator =(const GenericCurve3& rhs);

        // vertex buffer object handling methods
        GLvoid DeleteVertexBufferObjects();
        GLboolean RenderDerivatives(GLuint order, GLenum render_mode) const;
        GLboolean UpdateVertexBufferObjects(GLdouble scale=1.0,GLenum usage_flag = GL_STATIC_DRAW);

        GLfloat* MapDerivatives(GLuint order, GLenum access_mode = GL_READ_ONLY) const;
        GLboolean UnmapDerivatives(GLuint order) const;

        // get derivative by value
        DCoordinate3 operator ()(GLuint order, GLuint index) const;

        // get derivative by reference
        DCoordinate3& operator ()(GLuint order, GLuint index);

        // other update and query methods
        GLboolean SetDerivative(GLuint order, GLuint index, GLdouble x, GLdouble y, GLdouble z = 0.0);
        GLboolean SetDerivative(GLuint order, GLuint index, const DCoordinate3& d);
        GLboolean GetDerivative(GLuint order, GLuint index, GLdouble& x, GLdouble& y, GLdouble& z) const;
        GLboolean GetDerivative(GLuint order, GLuint index, DCoordinate3& d) const;

        GLuint GetMaximumOrderOfDerivatives() const;
        GLuint GetPointCount() const;
        GLenum GetUsageFlag() const;

        // destructor
        virtual ~GenericCurve3();
    };
}
