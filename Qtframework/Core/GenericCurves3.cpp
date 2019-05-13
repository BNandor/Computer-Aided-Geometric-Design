#include "GenericCurves3.h"

using namespace cagd;
using namespace std;

//--------------------------------------
// implementation of class GenericCurve3
//--------------------------------------

// default and special constructor
GenericCurve3::GenericCurve3(GLuint maximum_order_of_derivatives, GLuint point_count, GLenum usage_flag):
        _usage_flag(usage_flag),
        _vbo_derivative(RowMatrix<GLuint>(maximum_order_of_derivatives + 1)),
        _derivative(Matrix<DCoordinate3>(maximum_order_of_derivatives + 1, point_count))
{
}

// special constructor
GenericCurve3::GenericCurve3(const Matrix<DCoordinate3>& derivative, GLenum usage_flag):
        _usage_flag(usage_flag),
        _vbo_derivative(RowMatrix<GLuint>(derivative.GetRowCount())),
        _derivative(derivative)
{
}

// copy constructor
GenericCurve3::GenericCurve3(const GenericCurve3& curve):
        _usage_flag(curve._usage_flag),
        _vbo_derivative(RowMatrix<GLuint>(curve._vbo_derivative.GetColumnCount())),
        _derivative(curve._derivative)
{
    GLboolean vbo_update_is_possible = GL_TRUE;
    for (GLuint i = 0; i < curve._vbo_derivative.GetColumnCount(); ++i)
        vbo_update_is_possible &= curve._vbo_derivative(i);

    if (vbo_update_is_possible)
        UpdateVertexBufferObjects(_usage_flag);
}

// assignment operator
GenericCurve3& GenericCurve3::operator =(const GenericCurve3& rhs)
{
    if (this != &rhs)
    {
        DeleteVertexBufferObjects();

        _usage_flag = rhs._usage_flag;
        _derivative = rhs._derivative;

        GLboolean vbo_update_is_possible = GL_TRUE;
        for (GLuint i = 0; i < rhs._vbo_derivative.GetColumnCount(); ++i)
            vbo_update_is_possible &= rhs._vbo_derivative(i);

        if (vbo_update_is_possible)
            UpdateVertexBufferObjects(_usage_flag);
    }
    return *this;
}

// vertex buffer object handling methods
GLvoid GenericCurve3::DeleteVertexBufferObjects()
{
    for (GLuint i = 0; i < _vbo_derivative.GetColumnCount(); ++i)
    {
        if (_vbo_derivative(i))
        {
            glDeleteBuffers(1, &_vbo_derivative(i));
            _vbo_derivative(i) = 0;
        }
    }
}

GLboolean GenericCurve3::RenderDerivatives(GLuint order, GLenum render_mode) const
{
    GLuint max_order = _derivative.GetRowCount();
    if (order >= max_order || !_vbo_derivative(order))
        return GL_FALSE;

    GLuint point_count = _derivative.GetColumnCount();

    glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivative(order));
            glVertexPointer(3, GL_FLOAT, 0, (const GLvoid *)0);

            if (!order)
            {
                if (render_mode != GL_LINE_STRIP &&
                    render_mode != GL_LINE_LOOP  &&
                    render_mode != GL_POINTS)
                {
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glDisableClientState(GL_VERTEX_ARRAY);
                    return GL_FALSE;
                }

                glDrawArrays(render_mode, 0, point_count);
            }
            else
            {
                if (render_mode != GL_LINES && render_mode != GL_POINTS)
                {
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glDisableClientState(GL_VERTEX_ARRAY);
                    return GL_FALSE;
                }

                glDrawArrays(render_mode, 0, 2 * point_count);
            }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);

    return GL_TRUE;
}

GLboolean GenericCurve3::UpdateVertexBufferObjects(GLdouble scale,GLenum usage_flag)
{
    if (usage_flag != GL_STREAM_DRAW  && usage_flag != GL_STREAM_READ  && usage_flag != GL_STREAM_COPY  &&
        usage_flag != GL_DYNAMIC_DRAW && usage_flag != GL_DYNAMIC_READ && usage_flag != GL_DYNAMIC_COPY &&
        usage_flag != GL_STATIC_DRAW  && usage_flag != GL_STATIC_READ  && usage_flag != GL_STATIC_COPY)
        return GL_FALSE;

    DeleteVertexBufferObjects();

    _usage_flag = usage_flag;

    for(GLuint d = 0; d < _vbo_derivative.GetColumnCount(); ++d)
    {
        glGenBuffers(1, &_vbo_derivative(d));

        if (!_vbo_derivative(d))
        {
            for (GLuint i = 0; i < d; ++i)
            {
                glDeleteBuffers(1, &_vbo_derivative(i));
                _vbo_derivative(i) = 0;
            }

            return GL_FALSE;
        }
    }

    GLuint curve_point_count = _derivative.GetColumnCount();

    GLfloat *coordinate = 0;

    // curve points
    GLuint curve_point_byte_size = 3 * curve_point_count * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivative(0));
    glBufferData(GL_ARRAY_BUFFER, curve_point_byte_size, 0, _usage_flag);

    coordinate = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!coordinate)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        DeleteVertexBufferObjects();
        return GL_FALSE;
    }

    for (GLuint i = 0; i < curve_point_count; ++i)
    {
        for (GLuint j = 0; j < 3; ++j)
        {
            *coordinate = (GLfloat)_derivative(0,i)[j];
            ++coordinate;
        }
    }

    if (!glUnmapBuffer(GL_ARRAY_BUFFER))
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        DeleteVertexBufferObjects();
        return GL_FALSE;
    }

    // higher order derivatives
    GLuint higher_order_derivative_byte_size = 2 * curve_point_byte_size;

    for (GLuint d = 1; d < _derivative.GetRowCount(); ++d)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivative(d));
        glBufferData(GL_ARRAY_BUFFER, higher_order_derivative_byte_size, 0, _usage_flag);

        coordinate = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        if (!coordinate)
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            DeleteVertexBufferObjects();
            return GL_FALSE;
        }

        for (GLuint i = 0; i < curve_point_count; ++i)
        {
            DCoordinate3 sum = _derivative(0, i);
            sum += scale*_derivative(d, i);

            for (GLint j = 0; j < 3; ++j)
            {
                *coordinate = (GLfloat)_derivative(0, i)[j];
                *(coordinate + 3) = (GLfloat)sum[j];
                ++coordinate;
            }

            coordinate += 3;
        }

        if (!glUnmapBuffer(GL_ARRAY_BUFFER))
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            DeleteVertexBufferObjects();
            return GL_FALSE;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return GL_TRUE;
}

GLfloat* GenericCurve3::MapDerivatives(GLuint order, GLenum access_mode) const
{
    if (order >= _derivative.GetRowCount())
        return 0;

    if (access_mode != GL_READ_ONLY && access_mode != GL_WRITE_ONLY && access_mode != GL_READ_WRITE)
        return 0;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivative(order));

    return (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, access_mode);
}

GLboolean GenericCurve3::UnmapDerivatives(GLuint order) const
{
    if (order >= _derivative.GetRowCount())
        return GL_FALSE;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivative(order));

    return glUnmapBuffer(GL_ARRAY_BUFFER);
}

// get derivative by value
DCoordinate3 GenericCurve3::operator ()(GLuint order, GLuint index) const
{
    return _derivative(order, index);
}

// get derivative by reference
DCoordinate3& GenericCurve3::operator ()(GLuint order, GLuint index)
{
    return _derivative(order, index);
}

// other update and query methods
GLboolean GenericCurve3::SetDerivative(GLuint order, GLuint index, GLdouble x, GLdouble y, GLdouble z)
{
    if (order >= _derivative.GetRowCount() || index >= _derivative.GetColumnCount())
        return GL_FALSE;

    _derivative(order, index)[0] = x;
    _derivative(order, index)[1] = y;
    _derivative(order, index)[2] = z;

    return GL_TRUE;
}

GLboolean GenericCurve3::SetDerivative(GLuint order, GLuint index, const DCoordinate3& d)
{
    if (order >= _derivative.GetRowCount() || index >= _derivative.GetColumnCount())
        return GL_FALSE;

    _derivative(order, index) = d;

    return GL_TRUE;
}

GLboolean GenericCurve3::GetDerivative(GLuint order, GLuint index, GLdouble& x, GLdouble& y, GLdouble& z) const
{
    if (order >= _derivative.GetRowCount() || index >= _derivative.GetColumnCount())
        return GL_FALSE;

    x = _derivative(order, index)[0];
    y = _derivative(order, index)[1];
    z = _derivative(order, index)[2];

    return GL_TRUE;
}

GLboolean GenericCurve3::GetDerivative(GLuint order, GLuint index, DCoordinate3& d) const
{
    if (order >= _derivative.GetRowCount() || index >= _derivative.GetColumnCount())
        return GL_FALSE;

    d = _derivative(order, index);

    return GL_TRUE;
}

GLuint GenericCurve3::GetMaximumOrderOfDerivatives() const
{
    return _derivative.GetRowCount() - 1;
}

GLuint GenericCurve3::GetPointCount() const
{
    return _derivative.GetColumnCount();
}

GLenum GenericCurve3::GetUsageFlag() const
{
    return _usage_flag;
}

// destructor
GenericCurve3::~GenericCurve3()
{
    DeleteVertexBufferObjects();
}

//----------------------------
// input/output from/to stream
//----------------------------
ostream& cagd::operator <<(ostream& lhs, const GenericCurve3& rhs)
{
    return lhs << rhs._usage_flag << " " << rhs._derivative << endl;
}

std::istream& cagd::operator >>(std::istream& lhs, GenericCurve3& rhs)
{
    rhs.DeleteVertexBufferObjects();

    return lhs >> rhs._usage_flag >> rhs._derivative;
}
