#ifndef HYPERBOLICCOMPOSITECURVES3_H
#define HYPERBOLICCOMPOSITECURVES3_H
#include "HyperbolicArc3.h"
#include "../Core/GenericCurves3.h"
#include "../Core/Colors4.h"
#include "./IndicatingSphere.h"
#include <vector>
#include <fstream>

using namespace std;

namespace cagd {
  class HyperbolicCompositeCurve3 {
  public:
    enum Direction{Left=-1,Right=1};
    static const GLuint div_point_count = 100;
    constexpr static const GLdouble derivative_scale = 0.3;
    IndicatingSphere* leftSphere;
    IndicatingSphere* rightSphere;

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
          if(img)delete img;
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

    HyperbolicCompositeCurve3(GLuint max_curve_count):_arcs(max_curve_count),_arc_count(0){
      leftSphere = new IndicatingSphere(0.01);
      rightSphere = new IndicatingSphere(0.01);
    }

    GLboolean insert(GLdouble alpha,GLuint max_order_of_derivatives,const ColumnMatrix<DCoordinate3>& _data,GLdouble scale,Color4 color=Color4(0.5,0.5,0.5,0));
    GLboolean continueExisting(GLuint id,Direction direction,GLdouble alpha, GLuint max_order_derivative,GLdouble scale );
    GLuint join(GLuint firstId, GLuint SecondID,Direction firstDirection,Direction secondDirection,GLdouble scale);
    GLuint merge(GLuint firstId, GLuint SecondID,Direction firstDirection,Direction secondDirection);
    GLboolean updatePosition(int arcindex,int pointindex,DCoordinate3 newcoord);
    GLboolean updateArcForRendering( ArcAttributes*);

    // Sphere stuff
    void updateSpheresLocationByindex(GLuint index);
    void renderAll(GLuint max_order_of_derivatives);
    ~HyperbolicCompositeCurve3();
    int getNeighbourIndex(ArcAttributes* neighbour){
      for (int i=0;i<_arc_count;++i) {
          if(_arcs[i]==neighbour)
            return i;
        }
      return -1;
    }
    void saveToFile(string filename){
      ofstream file;
      file.open((filename).c_str());
      if(!file.is_open()){
          cerr<<"error creating "<<filename<<endl;
          return;
        }
      file<<_arc_count<<endl;
      for (int i = 0; i < _arc_count; ++i) {
          for (int j=0;j<4;++j) {
            file<<_arcs[i]->arc->operator[](j);
            file<<endl;
          }
      }

      for (int i = 0; i < _arc_count; ++i) {
          file<<i<<" "<<getNeighbourIndex(_arcs[i]->previous)<<endl;
          file<<i<<" "<<getNeighbourIndex(_arcs[i]->next)<<endl;
      }
      file.close();
      cout<<"File"<<filename<<"saved"<<endl;
    }

    void readFromFile(string filename){
      cout<<"reading from"<<filename<<endl;
      ifstream file;
      file.open((filename).c_str());
      if(!file.is_open()){
        cerr<<filename<<" could not be opened"<<endl;
        return;
        }
      int was = _arc_count;
      _arc_count=0;
      for(int i=0;i<was;++i){
          if(_arcs[i])
            delete _arcs[i];
      }
      int num_of_arcs;
      file>>num_of_arcs;
      cout<<num_of_arcs<<endl;

      ColumnMatrix<DCoordinate3> points(4);
      for (int i = 0; i < num_of_arcs; ++i) {
          for(int j=0;j<4;++j){
          file>>points[j];
          cout<<points[j]<<endl;
          }
          insert(5, 1, points,0.6,Color4(0,0,1));
      }
      int index,neighboureIndex;
      for (int i = 0; i < num_of_arcs; ++i) {
        file>>index>>neighboureIndex;
        if(neighboureIndex!=-1){
          _arcs[index]->previous=_arcs[neighboureIndex];
          }
        file>>index>>neighboureIndex;
        if(neighboureIndex!=-1){
          _arcs[index]->next=_arcs[neighboureIndex];
          }
      }
    }
  };
}
#endif // HYPERBOLICCOMPOSITECURVES3_H

