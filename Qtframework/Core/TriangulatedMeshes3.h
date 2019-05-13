#pragma once

#include "DCoordinates3.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "TriangularFaces.h"
#include "TCoordinates4.h"
#include <vector>

namespace cagd
{    
    class TriangulatedMesh3
    {
        friend class ParametricSurface3;
        friend class TensorProductSurface3;

        // homework: output to stream:
        // vertex count, face count
        // list of vertices
        // list of unit normal vectors
        // list of texture coordinates
        // list of faces
        friend std::ostream& operator <<(std::ostream& lhs, const TriangulatedMesh3& rhs){
          lhs<<rhs.VertexCount()<<" "<<rhs.FaceCount()<<std::endl;
          for (std::vector<DCoordinate3>::const_iterator vit = rhs._vertex.begin(); vit != rhs._vertex.end(); ++vit)
          {
              lhs << *vit<<std::endl;
          }
          for (std::vector<DCoordinate3>::const_iterator vit = rhs._normal.begin(); vit != rhs._normal.end(); ++vit)
          {
              lhs << *vit<<std::endl;
          }
          for (std::vector<TCoordinate4>::const_iterator vit = rhs._tex.begin(); vit != rhs._tex.end(); ++vit)
          {
              lhs << *vit<<std::endl;
          }
          for (std::vector<TriangularFace>::const_iterator vit = rhs._face.begin(); vit != rhs._face.end(); ++vit)
          {
              lhs << *vit<<std::endl;
          }
          return lhs;
        }

        // homework: input from stream: inverse of the ostream operator
        friend std::istream& operator >>(std::istream& lhs, TriangulatedMesh3& rhs){
          GLuint num;
          lhs>>num;//vertex count
          rhs._vertex.resize(num);
          rhs._normal.resize(num);
          rhs._tex.resize(num);
          lhs>>num;rhs._face.resize(num);

          for (std::vector<DCoordinate3>::iterator vit = rhs._vertex.begin(); vit != rhs._vertex.end(); ++vit)
          {
              lhs>>(*vit);
          }
          for (std::vector<DCoordinate3>::iterator vit = rhs._normal.begin(); vit != rhs._normal.end(); ++vit)
          {
              lhs >> *vit;
          }
          for (std::vector<TCoordinate4>::iterator vit = rhs._tex.begin(); vit != rhs._tex.end(); ++vit)
          {
              lhs >> *vit;
          }
          for (std::vector<TriangularFace>::iterator vit = rhs._face.begin(); vit != rhs._face.end(); ++vit)
          {
              lhs >> *vit;
          }
          return lhs;
        }

    protected:
        // vertex buffer object identifiers
        GLenum                      _usage_flag;
        GLuint                      _vbo_vertices;
        GLuint                      _vbo_normals;
        GLuint                      _vbo_tex_coordinates;
        GLuint                      _vbo_indices;

        // corners of bounding box
        DCoordinate3                 _leftmost_vertex;
        DCoordinate3                 _rightmost_vertex;

        // geometry
        std::vector<DCoordinate3>    _vertex;
        std::vector<DCoordinate3>    _normal;
        std::vector<TCoordinate4>    _tex;
        std::vector<TriangularFace>  _face;

    public:
        // special and default constructor
        TriangulatedMesh3(GLuint vertex_count = 0, GLuint face_count = 0, GLenum usage_flag = GL_STATIC_DRAW);

        // copy constructor
        TriangulatedMesh3(const TriangulatedMesh3& mesh);

        // assignment operator
        TriangulatedMesh3& operator =(const TriangulatedMesh3& rhs);

        // deletes all vertex buffer objects
        GLvoid DeleteVertexBufferObjects();

        // renders the geometry
        GLboolean Render(GLenum render_mode = GL_TRIANGLES) const;

        // updates all vertex buffer objects
        GLboolean UpdateVertexBufferObjects(GLenum usage_flag = GL_STATIC_DRAW);

        // loads the geometry (i.e. the array of vertices and faces) stored in an OFF file
        // at the same time calculates the unit normal vectors associated with vertices
        GLboolean LoadFromOFF(const std::string& file_name, GLboolean translate_and_scale_to_unit_cube = GL_FALSE);

        // homework: saves the geometry into an OFF file
        GLboolean SaveToOFF(const std::string& file_name) const;

        // mapping vertex buffer objects
        GLfloat* MapVertexBuffer(GLenum access_flag = GL_READ_ONLY) const;
        GLfloat* MapNormalBuffer(GLenum access_flag = GL_READ_ONLY) const;  // homework
        GLfloat* MapTextureBuffer(GLenum access_flag = GL_READ_ONLY) const; // homework

        // unmapping vertex buffer objects
        GLvoid UnmapVertexBuffer() const;
        GLvoid UnmapNormalBuffer() const;   // homework
        GLvoid UnmapTextureBuffer() const;  // homework

        // get properties of geometry
        GLuint VertexCount() const{return _vertex.size();} // homework
        GLuint FaceCount() const{return _face.size();} // homework
        //mine
        GLuint edgeCount() const{return 0;}
        //eof mine
        // destructor
        virtual ~TriangulatedMesh3();
    };
}
