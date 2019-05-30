#ifndef INDICATINGSPHERE_H
#define INDICATINGSPHERE_H
#include "../Parametric/ParametricSurfaces3.h"
#include "Test/TestFunctions.h"

namespace  cagd {
  class IndicatingSphere{
  private:
  TriangularMatrix<ParametricSurface3::PartialDerivative> _derivatives;
  ParametricSurface3* surface;  
  GLdouble sphereScale;

  public:
  TriangulatedMesh3 * _image;
    IndicatingSphere (GLdouble scale):_derivatives(3),sphereScale(scale){
      _derivatives(0,0)=surface3::d00;
      _derivatives(1,0)=surface3::d10;
      _derivatives(1,1)=surface3::d01;
      _image=0;
      surface = new ParametricSurface3(_derivatives,surface3::u_min,surface3::u_max,surface3::v_min,surface3::v_max);
    }
    ~IndicatingSphere(){
      if(surface)delete surface;
      surface=0;
      if(_image)delete _image;
      _image=0;
    }
    GLboolean updateImage(DCoordinate3 offset){
      if(!surface){
          std::cerr<<"error,in sphere generation, no surface"<<std::endl;
          return GL_FALSE;
        }
      if(_image)delete _image;
      _image = surface->GenerateImage(20,20,sphereScale,offset);
      if(!_image){
          std::cerr<<"error generating sphere image"<<std::endl;
          return GL_FALSE;
        }
      if(!_image->UpdateVertexBufferObjects()){
          std::cerr<<"error updating vbo of sphere"<<std::endl;
          return GL_FALSE;
        }
      return GL_TRUE;
    }
  };
}
#endif // INDICATINGSPHERE_H
