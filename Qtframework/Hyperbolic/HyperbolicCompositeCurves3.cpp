#include "HyperbolicCompositeCurves3.h"
using namespace std;
namespace cagd{

  HyperbolicCompositeCurve3::ArcAttributes::~ArcAttributes(){
    if(arc)delete arc;
    if(img)delete img;
    if(color)delete color;   
  }
  HyperbolicCompositeCurve3::ArcAttributes::ArcAttributes(const ArcAttributes & other){
    arc = new HyperbolicArc3(*other.arc);
    img = new GenericCurve3(*other.img);
    next = other.next;
    previous = other.previous;
  }
 HyperbolicCompositeCurve3::~HyperbolicCompositeCurve3(){
    for(int i=0;i<_arc_count;++i){
        if(_arcs[i])
          delete _arcs[i];
    }
  }
  HyperbolicCompositeCurve3::ArcAttributes& HyperbolicCompositeCurve3::ArcAttributes::operator=(const ArcAttributes & other){
    if(&other == this )return *this;
    if(arc)    delete arc;
    if(img)    delete img;
    if(color)    delete color;

    arc = new HyperbolicArc3(*other.arc);
    img = new GenericCurve3(*other.img);
    next = other.next;
    previous = other.previous;
    return *this;
  }

  GLboolean HyperbolicCompositeCurve3::insert(GLdouble alpha,GLuint max_order_of_derivatives,const ColumnMatrix<DCoordinate3>& _data,GLdouble scale,Color4 color){
    if(_arc_count == _arcs.size()) return GL_FALSE;
    ArcAttributes* arcattr = new ArcAttributes();
    arcattr->arc = new HyperbolicArc3(alpha);
    for(int i=0;i<4;i++){
      (*(arcattr->arc))[i] =_data[i];
    }
    if(!arcattr->arc->UpdateVertexBufferObjectsOfData())return GL_FALSE;
    if(!arcattr->generateImage(max_order_of_derivatives))return GL_FALSE;
    if(!arcattr->updateVBO(scale))return GL_FALSE;
    arcattr->color=new Color4(color);
    arcattr->derivatives_color=new Color4(0.0,0.5,0.0);
     _arcs[_arc_count]=arcattr;
     _arc_count++;
     return GL_TRUE;
  }

  GLboolean HyperbolicCompositeCurve3::continueExisting(GLuint id,Direction direction,GLdouble alpha, GLuint max_order_derivative,GLdouble scale ){
    if(id >= _arc_count)return GL_FALSE;
    ColumnMatrix<DCoordinate3> coords(4);
    DCoordinate3 p3,p2;
    if(direction == Right){
        p3 = (*(_arcs[id]->arc))[3];
        p2 = (*(_arcs[id]->arc))[2];
      }else{
        p3 = (*(_arcs[id]->arc))[0];
        p2 = (*(_arcs[id]->arc))[1];
      }
    coords[0]=p3;
    coords[1]=(p3*2) - p2;
    coords[2]=(p3*3) - p2*2;
    coords[3]=(p3*4) - p2*3;

    if(!insert(alpha,max_order_derivative,coords,scale))return GL_FALSE;
    if(direction == Right){
      _arcs[id]->next = _arcs[_arc_count-1];
      _arcs[_arc_count-1]->previous = _arcs[id];
      }else{
        _arcs[id]->previous = _arcs[_arc_count-1];
        _arcs[_arc_count-1]->next= _arcs[id];
      }
    return GL_TRUE;
  }

  GLuint HyperbolicCompositeCurve3::join(GLuint firstId, GLuint secondId,Direction firstDirection,Direction secondDirection,GLdouble scale){
    if(firstId >= _arc_count || secondId >= _arc_count) {
        std::cout<<firstId<<" "<<secondId<<std::endl;
        return 9;}
    ColumnMatrix<DCoordinate3> coords(4);
    DCoordinate3 p2,p3;
    DCoordinate3 q2,q3;
    if(firstDirection == Right){
        p3 = (*(_arcs[firstId]->arc))[3];
        p2 = (*(_arcs[firstId]->arc))[2];
    }else{
        p3 = (*(_arcs[firstId]->arc))[0];
        p2 = (*(_arcs[firstId]->arc))[1];
    }
    if(secondDirection == Right){
        q3 = (*(_arcs[secondId]->arc))[3];
        q2 = (*(_arcs[secondId]->arc))[2];
    }else{
        q3 = (*(_arcs[secondId]->arc))[0];
        q2 = (*(_arcs[secondId]->arc))[1];
    }
  coords[3] = q3;
  coords[0] = p3;
  coords[1] = p3*2 - p2;
  coords[2] = q3*2 - q2;
  GLdouble alpha_avg =( _arcs[firstId]->arc->getAlpha() + _arcs[secondId]->arc->getAlpha())/2.0;
  Color4 color_avg((_arcs[firstId]->color->r() + _arcs[firstId]->color->r())/2.0,(_arcs[firstId]->color->g() + _arcs[firstId]->color->g())/2.0,(_arcs[firstId]->color->b() + _arcs[firstId]->color->b())/2.0,0);
  if(!insert(alpha_avg,2,coords,scale,color_avg))return 2;
  if(firstDirection == Right){
    _arcs[firstId]->next = _arcs[_arc_count-1];
    }else{
      _arcs[firstId]->previous = _arcs[_arc_count-1];
    }
  if(secondDirection == Right){
    _arcs[secondId]->next = _arcs[_arc_count-1];
    }else{
      _arcs[secondId]->previous = _arcs[_arc_count-1];
    }
  _arcs[_arc_count-1]->previous = _arcs[firstId];
  _arcs[_arc_count-1]->next = _arcs[secondId];
  return 1;
  }

  GLboolean HyperbolicCompositeCurve3::updateArcForRendering( ArcAttributes* attr){
    attr->arc->DeleteVertexBufferObjectsOfData();
    if(!attr->arc->UpdateVertexBufferObjectsOfData())return GL_FALSE;
    if(!attr->generateImage(2))return GL_FALSE;
    if(!attr->updateVBO(derivative_scale))return GL_FALSE;
    return GL_TRUE;
  }

  GLboolean HyperbolicCompositeCurve3::updatePosition(int arcindex,int pointindex,DCoordinate3 newcoord){
    if(arcindex < 0 || arcindex>=_arc_count)return GL_FALSE;
    if(pointindex < 0 || pointindex>3)return GL_FALSE;
    DCoordinate3 diff = -(*(_arcs[arcindex]->arc))[pointindex] + newcoord;
    switch (pointindex) {
      case 0:{
          if(_arcs[arcindex]->previous){
            if((*((_arcs[arcindex]->previous)->arc))[3]==(*((_arcs[arcindex])->arc))[0]){
              (*((_arcs[arcindex]->previous)->arc))[3]=newcoord;
              (*((_arcs[arcindex]->previous)->arc))[2]+=diff;
            }else{
               (*((_arcs[arcindex]->previous)->arc))[0]=newcoord;
               (*((_arcs[arcindex]->previous)->arc))[1]+=diff;
            }
            (*((_arcs[arcindex])->arc))[1]+=diff;
            if(!updateArcForRendering(_arcs[arcindex]->previous))return GL_FALSE;
          }
          (*((_arcs[arcindex])->arc))[0]=newcoord;
          if(!updateArcForRendering(_arcs[arcindex]))return GL_FALSE;
        }  break;
      case 3:{
          if(_arcs[arcindex]->next){
            if((*((_arcs[arcindex]->next)->arc))[3]==(*((_arcs[arcindex])->arc))[3]){
              (*((_arcs[arcindex]->next)->arc))[3]=newcoord;
              (*((_arcs[arcindex]->next)->arc))[2]+=diff;
            }else{
               (*((_arcs[arcindex]->next)->arc))[0]=newcoord;
               (*((_arcs[arcindex]->next)->arc))[1]+=diff;
            }
            (*((_arcs[arcindex])->arc))[2]+=diff;
            if(!updateArcForRendering(_arcs[arcindex]->next))return GL_FALSE;
          }
          (*((_arcs[arcindex])->arc))[3]=newcoord;
          if(!updateArcForRendering(_arcs[arcindex]))return GL_FALSE;
        }  break;

    }
    return GL_TRUE;
  }
void HyperbolicCompositeCurve3::renderAll(GLuint max_order_of_derivatives){
  for (int i=0;i<_arc_count;++i) {
    if(_arcs[i]->img){
      glColor3f(_arcs[i]->color->r(),_arcs[i]->color->g(),_arcs[i]->color->b());
      _arcs[i]->img->RenderDerivatives(0,GL_LINE_STRIP);
      //glPointSize(5.0f);
      for (int j=1;j<=max_order_of_derivatives;++j) {
          glColor3f(_arcs[i]->derivatives_color->r(),_arcs[i]->derivatives_color->g(),_arcs[i]->derivatives_color->b());
          _arcs[i]->img->RenderDerivatives(j,GL_LINES);
          //_arc->RenderDerivatives(1,GL_POINTS);
      }
      glColor3f(0.2f,0.6f,0.6f);
      _arcs[i]->arc->RenderData();
      }
  }
}

}
