#pragma once

#include "DCoordinates3.h"
#include "GenericCurves3.h"
#include "Matrices.h"

namespace cagd
{
    //-------------------------
    // class LinearCombination3
    //-------------------------
    class LinearCombination3
    {
    public:
        class Derivatives: public ColumnMatrix<DCoordinate3>
        {
        public:
            // special/default constructor
            Derivatives(GLuint maximum_order_of_derivatives = 2);

            // copy constructor
            Derivatives(const Derivatives& d);

            // assignment operator
            Derivatives& operator =(const Derivatives& rhs);

            // all inherited Descartes coordinates are set to the null vector
            GLvoid LoadNullVectors();
        };

    protected:
        GLuint                      _vbo_data;
        GLenum                      _data_usage_flag;
        GLdouble                    _u_min, _u_max;
        ColumnMatrix<DCoordinate3>  _data;

    public:
        // special constructor
        LinearCombination3(
                GLdouble u_min, GLdouble u_max,
                GLuint data_count,
                GLenum data_usage_flag = GL_STATIC_DRAW);

        // copy constructor
        LinearCombination3(const LinearCombination3& lc);

        // assignment operator
        LinearCombination3& operator =(const LinearCombination3& rhs);

        // vbo handling methods
        virtual GLvoid DeleteVertexBufferObjectsOfData();
        virtual GLboolean RenderData(GLenum render_mode = GL_LINE_STRIP) const;
        virtual GLboolean UpdateVertexBufferObjectsOfData(GLenum usage_flag = GL_STATIC_DRAW);

        // get data by value
        DCoordinate3 operator [](GLuint index) const;

        // get data by reference
        DCoordinate3& operator [](GLuint index);

        // set/get definition domain
        GLvoid SetDefinitionDomain(GLdouble u_min, GLdouble u_max);
        GLvoid GetDefinitionDomain(GLdouble& u_min, GLdouble& u_max) const;

        //----------------
        // abstract method
        //----------------
        // calculates a row matrix which consists of function values {F_i(u)}_{i=0}^{data_count-1}
        virtual GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble>& values) const = 0;

        //----------------
        // abstract method
        //----------------
        // calculates the point and its associated (higher) order derivatives of the linear
        // combination sum_{i=0}^{data_count -1} _data[i] F_i(u) at the parameter value u
        virtual GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives& d) const = 0;

        // generate image/arc
        virtual GenericCurve3* GenerateImage(GLuint max_order_of_derivatives, GLuint div_point_count, GLenum usage_flag = GL_STATIC_DRAW) const;

        // assure interpolation
        virtual GLboolean UpdateDataForInterpolation(const ColumnMatrix<GLdouble>& knot_vector, const ColumnMatrix<DCoordinate3>& data_points_to_interpolate);

        // destructor
        virtual ~LinearCombination3();
    };
}
