#include "ParametricSurfaces3.h"

#include <cstdlib>
#include <cmath>

using namespace std;

namespace cagd
{
    // special  constructor
    ParametricSurface3::ParametricSurface3(
            const TriangularMatrix<PartialDerivative> &pd,
            GLdouble u_min, GLdouble u_max,
            GLdouble v_min, GLdouble v_max):
        _pd(pd),
        _u_min(u_min), _u_max(u_max),
        _v_min(v_min), _v_max(v_max)
    {
    }

    // generates the approximated tesselated image of the parametric surface
    TriangulatedMesh3* ParametricSurface3::GenerateImage(
        GLuint u_div_point_count,
        GLuint v_div_point_count,
        GLdouble scale,
        DCoordinate3 offset,
        GLenum usage_flag) const
    {
        if (_pd.GetRowCount() < 2 ||    // i.e., if we cannot evaluate the points and normal vectors of the surface
            u_div_point_count < 2 ||    // i.e., if the number of u-directional subdivion points is too small
            v_div_point_count < 2)      // i.e., if the number of v-directional subdivion points is too small
        {
            return 0;
        }

        TriangulatedMesh3 *result = 0;

        result = new (nothrow) TriangulatedMesh3(
                u_div_point_count * v_div_point_count,                  // number of unique vertices
                2 * (u_div_point_count - 1) * (v_div_point_count - 1),  // number of triangular faces
                usage_flag);

        if (!result)
        {
            return 0;
        }

        // distance between consecutive subdivision points
        GLdouble du = (_u_max - _u_min) / (u_div_point_count - 1);
        GLdouble dv = (_v_max - _v_min) / (v_div_point_count - 1);

        // distance between consecutive subdivision points for texture coordinates
        GLdouble ds = 1.0 / (u_div_point_count - 1);
        GLdouble dt = 1.0 / (v_div_point_count - 1);

        // current triangular face counter
        GLuint current_face = 0;

        for (GLuint i = 0; i < u_div_point_count; ++i)
        {
            GLdouble u = min(_u_min + i * du, _u_max);
            GLdouble s = min(i * ds, 1.0);

            for (GLuint j = 0; j < v_div_point_count; ++j)
            {
                GLdouble v = min(_v_min + j * dv, _v_max);
                GLdouble t = min(i * dt, 1.0);

                /*
                    3-2
                    |/|
                    0-1
                */

                // unique vertex identifiers
                GLuint index[4];

                index[0] = i * v_div_point_count + j;
                index[1] = index[0] + 1;
                index[2] = index[1] + v_div_point_count;
                index[3] = index[2] - 1;

                // surface point
                (*result)._vertex[index[0]] =  offset +scale*(_pd(0, 0)(u, v));

                // the surface normal is obtained as the cross product of the first order partial derivatives
                (*result)._normal[index[0]] =  _pd(1, 0)(u, v);
                (*result)._normal[index[0]] ^= _pd(1, 1)(u, v);
                (*result)._normal[index[0]].normalize();

                // texture coordinates
                (*result)._tex[index[0]].s() = s;
                (*result)._tex[index[0]].t() = t;

                // connectivity information
                if (i < u_div_point_count - 1 && j < v_div_point_count - 1)
                {
                    (*result)._face[current_face][0] = index[0];
                    (*result)._face[current_face][1] = index[1];
                    (*result)._face[current_face][2] = index[2];
                    ++current_face;

                    (*result)._face[current_face][0] = index[0];
                    (*result)._face[current_face][1] = index[2];
                    (*result)._face[current_face][2] = index[3];
                    ++current_face;
                }
            }
        }

        return result;
    }
}
