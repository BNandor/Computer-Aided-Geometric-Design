#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>

namespace cagd
{
    class TriangularFace
    {
    protected:
        GLuint _node[3];

    public:
        // default constructor
        TriangularFace();

        // homework: copy constructor
        TriangularFace(const TriangularFace& face){
          _node[0]=face._node[0];
          _node[1]=face._node[1];
          _node[2]=face._node[2];
        }

        // homework: assignment operator
        TriangularFace& operator =(const TriangularFace& rhs){
          _node[0]=rhs._node[0];
          _node[1]=rhs._node[1];
          _node[2]=rhs._node[2];
          return *this;
        }

        // homework: get node identifiers by value
        GLuint operator [](GLuint i) const{
          return _node[i];
        }

        // homework: get node identifiers by reference
        GLuint& operator [](GLuint i){
          return _node[i];
        }
    };

    // default constructor
    inline TriangularFace::TriangularFace()
    {
        _node[0] = _node[1] = _node[2] = 0;
    }

    // output to stream
    inline std::ostream& operator <<(std::ostream& lhs, const TriangularFace& rhs)
    {
        lhs << 3;//ez miert kell, amikor saveOFF olok, nem zavar be ?
        for (GLuint i = 0; i < 3; ++i)
            lhs  << " " << rhs[i];
        return lhs;
    }

    // homework
    inline std::istream& operator >>(std::istream& lhs, TriangularFace& rhs){
      GLuint num;
      lhs>>num;//reading that 3
      lhs>>num;rhs[0]=num;
      lhs>>num;rhs[1]=num;
      lhs>>num;rhs[2]=num;
      return lhs;
    }
}
