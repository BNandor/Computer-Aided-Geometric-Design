#include "HyperbolicCompositePatch3.h"
using namespace std;
namespace cagd{

  HyperbolicCompositePatch3::PatchAttributes::~PatchAttributes(){
    if(patch)delete patch;
    if(img)delete img;
    if(derivatives_color)delete derivatives_color;
  }
  HyperbolicCompositePatch3::PatchAttributes::PatchAttributes(const PatchAttributes & other){
    patch = new HyperbolicPatch3(*other.patch);
    img = new TriangulatedMesh3(*other.img);
    memcpy(neighbours,other.neighbours,8*sizeof(PatchAttributes*));
  }
 HyperbolicCompositePatch3::~HyperbolicCompositePatch3(){
    for(int i=0;i<_patch_count;++i){
        if(_patches[i])
          delete _patches[i];
    }
  }
  GLboolean HyperbolicCompositePatch3::insert(GLdouble alpha,GLuint max_order_of_derivatives,const ColumnMatrix<DCoordinate3>& _data,Material material){
    if(_patch_count == _patches.size()) return GL_FALSE;
    PatchAttributes* patchattr = new PatchAttributes();
    patchattr->patch = new HyperbolicPatch3(alpha);

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
          (*(patchattr->patch)).SetData(i,j,_data[i*4 + j]);
        }
    }
    if(!patchattr->patch->UpdateVertexBufferObjectsOfData())return GL_FALSE;
    cout<<"updated vbo of data"<<endl;
    if(!patchattr->generateImage(max_order_of_derivatives))return GL_FALSE;
    cout<<"generated image"<<endl;
    if(!patchattr->updateVBO())return GL_FALSE;
    cout<<"updated vbo"<<endl;
    patchattr->material=material;
    patchattr->derivatives_color=&default_derivatives_colour;
     _patches[_patch_count]=patchattr;
     _patch_count++;
     return GL_TRUE;
  }

  GLboolean HyperbolicCompositePatch3::continueExisting(GLuint id,Direction direction,GLdouble alpha,Material material){
    if(id >= _patch_count)return GL_FALSE;

    PatchAttributes* cpatch = _patches[id];

    ColumnMatrix<DCoordinate3> coords(16);
    DCoordinate3 p[2][4];

    switch (direction) {
      case N:{
          for (int j=0;j<2;++j) {
            for (int i=0;i<4;++i) {
               cpatch->patch->GetData(j,i,p[j][i]);
            }
          }
          for (int i=0;i<4;++i) {
            coords[3*4 + i] = p[0][i];
            coords[2*4 + i] = (2*p[0][i]-p[1][i]);
            coords[1*4 + i] = (3*p[0][i]-2*p[1][i]);
            coords[0*4 + i] = (4*p[0][i]-3*p[1][i]);
          }

      }break;
      case S:{
          for (int j=0;j<2;++j) {
            for (int i=0;i<4;++i) {
               cpatch->patch->GetData(3-j,i,p[j][i]);
            }
          }
          for (int i=0;i<4;++i) {
            coords[0*4 + i] = p[0][i];
            coords[1*4 + i] = (2*p[0][i]-p[1][i]);
            coords[2*4 + i] = (3*p[0][i]-2*p[1][i]);
            coords[3*4 + i] = (4*p[0][i]-3*p[1][i]);
          }
      }break;
      case W:{
          for (int j=0;j<4;++j) {
            for (int i=0;i<2;++i) {
               cpatch->patch->GetData(j,i,p[i][j]);
            }
          }
          for (int i=0;i<4;++i) {
            coords[i*4 + 3] = p[0][i];
            coords[i*4 + 2] = (2*p[0][i]-p[1][i]);
            coords[i*4 + 1] = (3*p[0][i]-2*p[1][i]);
            coords[i*4 + 0] = (4*p[0][i]-3*p[1][i]);
          }
            }break;
      case E:{
          for (int j=0;j<4;++j) {
            for (int i=0;i<2;++i) {
               cpatch->patch->GetData(j,3-i,p[i][j]);
            }
          }
          for (int i=0;i<4;++i) {
            coords[i*4 + 0] = p[0][i];
            coords[i*4 + 1] = (2*p[0][i]-p[1][i]);
            coords[i*4 + 2] = (3*p[0][i]-2*p[1][i]);
            coords[i*4 + 3] = (4*p[0][i]-3*p[1][i]);
          }
            }break;
    }

    if(!insert(alpha,1,coords,material))return GL_FALSE;
    switch (direction) {
      case N:{
          cpatch->neighbours[0] = _patches[_patch_count-1];
           _patches[_patch_count-1]->neighbours[4]= cpatch;
        }break;
      case S:{
          cpatch->neighbours[4] = _patches[_patch_count-1];
           _patches[_patch_count-1]->neighbours[0]= cpatch;
        }break;

      case W:{
          cpatch->neighbours[6] = _patches[_patch_count-1];
           _patches[_patch_count-1]->neighbours[2]= cpatch;
        }break;
      case E:{
          cpatch->neighbours[2] = _patches[_patch_count-1];
           _patches[_patch_count-1]->neighbours[6]= cpatch;
        }break;
    }
    return GL_TRUE;
  }

//  GLuint HyperbolicCompositePatch3::join(GLuint firstId, GLuint secondId,Direction firstDirection,Direction secondDirection,GLdouble scale){
//    if(firstId >= _arc_count || secondId >= _arc_count) {
//        std::cout<<firstId<<" "<<secondId<<std::endl;
//        return 9;}
//    ColumnMatrix<DCoordinate3> coords(4);
//    DCoordinate3 p2,p3;
//    DCoordinate3 q2,q3;
//    if(firstDirection == Right){
//        p3 = (*(_arcs[firstId]->arc))[3];
//        p2 = (*(_arcs[firstId]->arc))[2];
//    }else{
//        p3 = (*(_arcs[firstId]->arc))[0];
//        p2 = (*(_arcs[firstId]->arc))[1];
//    }
//    if(secondDirection == Right){
//        q3 = (*(_arcs[secondId]->arc))[3];
//        q2 = (*(_arcs[secondId]->arc))[2];
//    }else{
//        q3 = (*(_arcs[secondId]->arc))[0];
//        q2 = (*(_arcs[secondId]->arc))[1];
//    }
//  coords[3] = q3;
//  coords[0] = p3;
//  coords[1] = p3*2 - p2;
//  coords[2] = q3*2 - q2;
//  GLdouble alpha_avg =( _arcs[firstId]->arc->getAlpha() + _arcs[secondId]->arc->getAlpha())/2.0;
//  Color4 color_avg((_arcs[firstId]->color->r() + _arcs[firstId]->color->r())/2.0,(_arcs[firstId]->color->g() + _arcs[firstId]->color->g())/2.0,(_arcs[firstId]->color->b() + _arcs[firstId]->color->b())/2.0,0);
//  if(!insert(alpha_avg,2,coords,scale,color_avg))return 2;
//  if(firstDirection == Right){
//    _arcs[firstId]->next = _arcs[_arc_count-1];
//    }else{
//      _arcs[firstId]->previous = _arcs[_arc_count-1];
//    }
//  if(secondDirection == Right){
//    _arcs[secondId]->next = _arcs[_arc_count-1];
//    }else{
//      _arcs[secondId]->previous = _arcs[_arc_count-1];
//    }
//  _arcs[_arc_count-1]->previous = _arcs[firstId];
//  _arcs[_arc_count-1]->next = _arcs[secondId];
//  return 1;
//  }

//  GLboolean HyperbolicCompositePatch3::updateArcForRendering( PatchAttributes* attr){
//    attr->arc->DeleteVertexBufferObjectsOfData();
//    if(!attr->arc->UpdateVertexBufferObjectsOfData())return GL_FALSE;
//    if(!attr->generateImage(2))return GL_FALSE;
//    if(!attr->updateVBO(derivative_scale))return GL_FALSE;
//    return GL_TRUE;
//  }

//  GLboolean HyperbolicCompositePatch3::updatePosition(int arcindex,int pointindex,DCoordinate3 newcoord){
//    if(arcindex < 0 || arcindex>=_arc_count)return GL_FALSE;
//    if(pointindex < 0 || pointindex>3)return GL_FALSE;
//    DCoordinate3 diff = newcoord - (*(_arcs[arcindex]->arc))[pointindex];
//    switch (pointindex) {
//      case 0:{
//          if(_arcs[arcindex]->previous){
//            if((*((_arcs[arcindex]->previous)->arc))[3]==(*((_arcs[arcindex])->arc))[0]){
//              (*((_arcs[arcindex]->previous)->arc))[3]=newcoord;
//              (*((_arcs[arcindex]->previous)->arc))[2]+=diff;
//            }else{
//               (*((_arcs[arcindex]->previous)->arc))[0]=newcoord;
//               (*((_arcs[arcindex]->previous)->arc))[1]+=diff;
//            }
//            (*((_arcs[arcindex])->arc))[1]+=diff;
//            if(!updateArcForRendering(_arcs[arcindex]->previous))return GL_FALSE;
//          }
//          (*((_arcs[arcindex])->arc))[0]=newcoord;
//          if(!updateArcForRendering(_arcs[arcindex]))return GL_FALSE;
//        }  break;
//      case 3:{
//          if(_arcs[arcindex]->next){
//            if((*((_arcs[arcindex]->next)->arc))[3]==(*((_arcs[arcindex])->arc))[3]){
//              (*((_arcs[arcindex]->next)->arc))[3]=newcoord;
//              (*((_arcs[arcindex]->next)->arc))[2]+=diff;
//            }else{
//               (*((_arcs[arcindex]->next)->arc))[0]=newcoord;
//               (*((_arcs[arcindex]->next)->arc))[1]+=diff;
//            }
//            (*((_arcs[arcindex])->arc))[2]+=diff;
//            if(!updateArcForRendering(_arcs[arcindex]->next))return GL_FALSE;
//          }
//          (*((_arcs[arcindex])->arc))[3]=newcoord;
//          if(!updateArcForRendering(_arcs[arcindex]))return GL_FALSE;
//        }  break;
//      case 2:{
//          if(_arcs[arcindex]->next){
//            if((*((_arcs[arcindex]->next)->arc))[3]==(*((_arcs[arcindex])->arc))[3]){
//              (*((_arcs[arcindex]->next)->arc))[2]-=diff;
//            }else{
//               (*((_arcs[arcindex]->next)->arc))[1]-=diff;
//            }
//            if(!updateArcForRendering(_arcs[arcindex]->next))return GL_FALSE;
//          }
//          (*((_arcs[arcindex])->arc))[2]=newcoord;
//          if(!updateArcForRendering(_arcs[arcindex]))return GL_FALSE;
//        }  break;
//      case 1:{
//          if(_arcs[arcindex]->previous){
//            if((*((_arcs[arcindex]->previous)->arc))[3]==(*((_arcs[arcindex])->arc))[0]){
//              (*((_arcs[arcindex]->previous)->arc))[2]-=diff;
//            }else{
//               (*((_arcs[arcindex]->previous)->arc))[1]-=diff;
//            }
//            if(!updateArcForRendering(_arcs[arcindex]->previous))return GL_FALSE;
//          }
//          (*((_arcs[arcindex])->arc))[1]=newcoord;
//          if(!updateArcForRendering(_arcs[arcindex]))return GL_FALSE;
//        }  break;

//    }
//    return GL_TRUE;
//  }

//  GLuint HyperbolicCompositePatch3::merge(GLuint firstId, GLuint secondId,Direction firstDirection,Direction secondDirection){
//    if(firstId < 0 || firstId >= _arc_count  )return GL_FALSE;
//    if(secondId < 0 || secondId >= _arc_count  )return GL_FALSE;

//      DCoordinate3 newpos;
//      DCoordinate3 p2,q1;

//      p2 = (firstDirection == Right) ?(*((_arcs[firstId])->arc))[2]: (*((_arcs[firstId])->arc))[1];
//      q1 = (secondDirection == Left) ?(*((_arcs[secondId])->arc))[1]: (*((_arcs[secondId])->arc))[2];
//      newpos = (0.5)*(p2+q1);

//      if(firstDirection == Right){
//        (*((_arcs[firstId])->arc))[3]=newpos;
//        }else{
//        (*((_arcs[firstId])->arc))[0]=newpos;
//      }
//      if(secondDirection == Right){
//        (*((_arcs[secondId])->arc))[3]=newpos;
//        }else{
//        (*((_arcs[secondId])->arc))[0]=newpos;
//      }
//      if(!updateArcForRendering(_arcs[firstId]))return GL_FALSE;
//      if(!updateArcForRendering(_arcs[secondId]))return GL_FALSE;

     //(firstDirection == Right)?(*((_arcs[firstId])->arc))[3]:(*((_arcs[firstId])->arc))[0] = newpos;notok
     //(secondDirection == Left)?(*((_arcs[secondId])->arc))[0]:(*((_arcs[secondId])->arc))[3] = newpos;
  //}

void HyperbolicCompositePatch3::renderAll(){
  for (int i=0;i<_patch_count;++i) {
    if(_patches[i]->img){
        glColor3f(0.5f,0.2f,0.0f);
        if(_patches[i]->patch){
          _patches[i]->patch->RenderData();
        }
          glEnable(GL_LIGHTING);
          glEnable(GL_LIGHT0);
          glEnable(GL_NORMALIZE);
          _patches[i]->material.Apply();
          _patches[i]->img->Render();
          glDisable(GL_LIGHTING);
          glDisable(GL_LIGHT0);
          glDisable(GL_NORMALIZE);
    }
  }
}
}
