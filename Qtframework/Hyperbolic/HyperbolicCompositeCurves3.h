#ifndef HYPERBOLICCOMPOSITECURVES3_H
#define HYPERBOLICCOMPOSITECURVES3_H
#include "HyperbolicArc3.h"
#include "../Core/GenericCurves3.h"
#include "../Core/Colors4.h"
#include <vector>

using namespace std;

namespace cagd {
  class HyperbolicCompositeCurve3 {
  public:
    enum Direction{Left=-1,Right=1};
    static const GLuint div_point_count = 100;
    constexpr static const GLdouble derivative_scale = 0.3;

    class ArcAttributes{
    public:
        HyperbolicArc3 * arc;
        GenericCurve3 * img;
        Color4 * color;
        Color4 * derivatives_color;
        ArcAttributes * next;
        ArcAttributes * previous;
        ArcAttributes():arc(0),img(0),color(0),next(0),previous(0){
        }
        ArcAttributes(const ArcAttributes & other);
        ArcAttributes& operator=(const ArcAttributes & other);
        ~ArcAttributes();

        GLboolean generateImage(GLuint max_order_of_derivatives){
          img = arc->GenerateImage(max_order_of_derivatives,div_point_count);
          return img != 0;
        }
        GLboolean updateVBO(GLdouble scale){
           if(!img)return GL_FALSE;
           return img->UpdateVertexBufferObjects(scale);
        }
    };

  protected:
    vector<ArcAttributes*> _arcs;
    GLuint _arc_count;
  public:
    int getSize(){return _arc_count;}
    ArcAttributes* getArc(int index){return _arcs[index];}
    HyperbolicCompositeCurve3(GLuint max_curve_count):_arcs(max_curve_count),_arc_count(0){}
    GLboolean insert(GLdouble alpha,GLuint max_order_of_derivatives,const ColumnMatrix<DCoordinate3>& _data,GLdouble scale,Color4 color=Color4(0.5,0.5,0.5,0));
    GLboolean continueExisting(GLuint id,Direction direction,GLdouble alpha, GLuint max_order_derivative,GLdouble scale );
    GLuint join(GLuint firstId, GLuint SecondID,Direction firstDirection,Direction secondDirection,GLdouble scale);
    GLuint merge(GLuint firstId, GLuint SecondID,Direction firstDirection,Direction secondDirection);
    GLboolean updatePosition(int arcindex,int pointindex,DCoordinate3 newcoord);
    GLboolean updateArcForRendering( ArcAttributes*);
    void renderAll(GLuint max_order_of_derivatives);
    ~HyperbolicCompositeCurve3();
  };  
}
#endif // HYPERBOLICCOMPOSITECURVES3_H

