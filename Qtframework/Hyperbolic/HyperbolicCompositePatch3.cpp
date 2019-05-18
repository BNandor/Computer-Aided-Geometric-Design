#include "HyperbolicCompositePatch3.h"
using namespace std;
namespace cagd{

  HyperbolicCompositePatch3::PatchAttributes::~PatchAttributes(){
    if(patch)delete patch;
    if(img)delete img;
//    if(derivatives_color)delete derivatives_color;
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
      case NE:{
               cpatch->patch->GetData(0,2,p[0][2]);
               cpatch->patch->GetData(0,3,p[0][3]);
               cpatch->patch->GetData(1,2,p[1][2]);
               cpatch->patch->GetData(1,3,p[1][3]);

          for (int i=0;i<4;++i) {
            coords[3*4 + i] = (i+1)*p[0][3]-i*p[0][2];
          }
          for (int i=0;i<4;++i) {
            coords[i*4 ] = (4-i)*p[0][3]-(4-i-1)*p[1][3];
          }
          coords[2*4+1]= 2*p[0][3]-p[1][2];
          coords[1*4+2]= 3*p[0][3]-2*p[1][2];
          coords[0*4+3]= 4*p[0][3]-3*p[1][2];

          coords[0*4+1]= (2/3.0)*coords[0]+(1/3.0)*coords[3];
          coords[0*4+2]= (1/3.0)*coords[0]+(2/3.0)*coords[3];
          coords[1*4+1]=(1/2.0)*(coords[1*4+0]+coords[1*4+2]);
          coords[2*4+2]=(1/2.0)*(coords[1*4+2]+coords[3*4+2]);
          coords[1*4+3]= (2/3.0)*coords[0*4+3]+(1/3.0)*coords[3*4+3];
          coords[2*4+3]= (1/3.0)*coords[0*4+3]+(2/3.0)*coords[3*4+3];
      }break;
      case SW:{
               cpatch->patch->GetData(2,0,p[0][2]);
               cpatch->patch->GetData(3,0,p[0][3]);
               cpatch->patch->GetData(2,1,p[1][2]);
               cpatch->patch->GetData(3,1,p[1][3]);

          for (int i=0;i<4;++i) {
            coords[0*4 + i] = (4-i)*p[0][3]-(4-i-1)*p[1][3];
          }
          for (int i=0;i<4;++i) {
            coords[i*4 +3 ] = (i+1)*p[0][3]-(i)*p[0][2];
          }
          coords[1*4+2]= 2*p[0][3]-p[1][2];
          coords[2*4+1]= 3*p[0][3]-2*p[1][2];
          coords[3*4+0]= 4*p[0][3]-3*p[1][2];

          coords[3*4+1]= (2/3.0)*coords[3*4+0]+(1/3.0)*coords[3*4+3];
          coords[3*4+2]= (1/3.0)*coords[3*4+0]+(2/3.0)*coords[3*4+3];
          coords[2*4+2]=(1/2.0)*(coords[2*4+1]+coords[3*4+2]);
          coords[1*4+1]=(1/2.0)*(coords[0*4+1]+coords[2*4+1]);
          coords[2*4+0]= (1/3.0)*coords[0*4+0]+(2/3.0)*coords[3*4+0];
          coords[1*4+0]= (2/3.0)*coords[0*4+0]+(1/3.0)*coords[3*4+0];
      }break;
      case NW:{
               cpatch->patch->GetData(0,0,p[0][0]);
               cpatch->patch->GetData(0,1,p[0][1]);
               cpatch->patch->GetData(1,0,p[1][0]);
               cpatch->patch->GetData(1,1,p[1][1]);

          for (int i=0;i<4;++i) {
            coords[i*4 + 3] = (4-i)*p[0][0]-(4-i-1)*p[1][0];
          }
          for (int i=0;i<4;++i) {
            coords[3*4 + i ] = (4-i)*p[0][0]-(4-i-1)*p[0][1];
          }
          coords[2*4+2]= 2*p[0][0]-p[1][1];
          coords[1*4+1]= 3*p[0][0]-2*p[1][1];
          coords[0*4+0]= 4*p[0][0]-3*p[1][1];

          coords[0*4+1]= (2/3.0)*coords[0]+(1/3.0)*coords[3];
          coords[0*4+2]= (1/3.0)*coords[0]+(2/3.0)*coords[3];
          coords[1*4+2]=(1/2.0)*(coords[1*4+1]+coords[1*4+3]);
          coords[2*4+1]=(1/2.0)*(coords[1*4+1]+coords[3*4+1]);
          coords[1*4+0]= (2/3.0)*coords[0*4+0]+(1/3.0)*coords[3*4+0];
          coords[2*4+0]= (1/3.0)*coords[0*4+0]+(2/3.0)*coords[3*4+0];
      }break;
      case SE:{
               cpatch->patch->GetData(3,3,p[1][1]);
               cpatch->patch->GetData(3,2,p[1][0]);
               cpatch->patch->GetData(2,3,p[0][1]);
               cpatch->patch->GetData(2,2,p[0][0]);

          for (int i=0;i<4;++i) {
            coords[0*4 + i] = (i+1)*p[1][1]-(i)*p[1][0];
          }
          for (int i=0;i<4;++i) {
            coords[i*4 + 0 ] = (i+1)*p[1][1]-(i)*p[0][1];
          }

          coords[1*4+1]= 2*p[1][1]-p[0][0];
          coords[2*4+2]= 3*p[1][1]-2*p[0][0];
          coords[3*4+3]= 4*p[1][1]-3*p[0][0];

          coords[3*4+1]= (2/3.0)*coords[3*4+0]+(1/3.0)*coords[3*4+3];
          coords[3*4+2]= (1/3.0)*coords[3*4+0]+(2/3.0)*coords[3*4+3];
          coords[1*4+2]=(1/2.0)*(coords[0*4+2]+coords[2*4+2]);
          coords[2*4+1]=(1/2.0)*(coords[2*4+0]+coords[2*4+2]);
          coords[1*4+3]= (2/3.0)*coords[0*4+3]+(1/3.0)*coords[3*4+3];
          coords[2*4+3]= (1/3.0)*coords[0*4+3]+(2/3.0)*coords[3*4+3];
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
      case NE:{
          cpatch->neighbours[1] = _patches[_patch_count-1];
           _patches[_patch_count-1]->neighbours[5]= cpatch;
        }break;
      case SW:{
          cpatch->neighbours[5] = _patches[_patch_count-1];
           _patches[_patch_count-1]->neighbours[1]= cpatch;
        }break;
      case NW:{
          cpatch->neighbours[7] = _patches[_patch_count-1];
           _patches[_patch_count-1]->neighbours[3]= cpatch;
        }break;
      case SE:{
          cpatch->neighbours[3] = _patches[_patch_count-1];
           _patches[_patch_count-1]->neighbours[7]= cpatch;
        }break;
    }
    return GL_TRUE;
  }

  GLuint HyperbolicCompositePatch3::join(GLuint firstId, GLuint secondId,Direction firstDirection,Direction secondDirection){
    if(firstId >= _patch_count || secondId >= _patch_count) {
        std::cerr<<"Invalid ids"<<firstId<<" or "<<secondId<<std::endl;
        return 9;
    }

    HyperbolicPatch3* firstPatch = _patches[firstId]->patch;
    HyperbolicPatch3* secondPatch = _patches[secondId]->patch;

    ColumnMatrix<DCoordinate3> coords(16);
    DCoordinate3 p[2][4];
    DCoordinate3 q[2][4];

    switch (firstDirection) {
      case N:{
          for (int i=0;i<4;++i) {
              firstPatch->GetData(0,i,p[1][i]);
              firstPatch->GetData(1,i,p[0][i]);
          }
        }break;
      case S:{
          for (int i=0;i<4;++i) {
              firstPatch->GetData(2,i,p[0][i]);
              firstPatch->GetData(3,i,p[1][i]);
          }
        }break;
      case E:{
          for (int i=0;i<4;++i) {
              firstPatch->GetData(i,2,p[0][i]);
              firstPatch->GetData(i,3,p[1][i]);
          }
        }break;
      case W:{
          for (int i=0;i<4;++i) {
              firstPatch->GetData(i,0,p[1][i]);
              firstPatch->GetData(i,1,p[0][i]);
          }
        }break;
      }

    switch (secondDirection) {
      case S:{
          for (int i=0;i<4;++i) {
              secondPatch->GetData(2,i,q[0][i]);
              secondPatch->GetData(3,i,q[1][i]);
          }
        }break;
      case N:{
          for (int i=0;i<4;++i) {
              secondPatch->GetData(0,i,q[1][i]);
              secondPatch->GetData(1,i,q[0][i]);
          }
        }break;
      case W:{
          for (int i=0;i<4;++i) {
              secondPatch->GetData(i,0,q[1][i]);
              secondPatch->GetData(i,1,q[0][i]);
          }
        }break;
      case E:{
          for (int i=0;i<4;++i) {
              secondPatch->GetData(i,2,q[0][i]);
              secondPatch->GetData(i,3,q[1][i]);
          }
        }break;
      }

  //Connecting the patches with a new one
    //set boundary coords
  bool up;
  switch(firstDirection){
    case N:{
        switch(secondDirection){
          case N:{
              for (int i=0;i<4;++i) {
                coords[0*4+3-i]=p[1][i];
                coords[1*4+3-i]=2*p[1][i]-p[0][i];

                coords[2*4+3-i]=2*q[1][3-i]-q[0][3-i];
                coords[3*4+3-i]=q[1][3-i];
              }
            }break;
          case S:{
              for (int i=0;i<4;++i) {
                  coords[0*4+i]=q[1][i];
                  coords[1*4+i]=2*q[1][i]-q[0][i];

                  coords[2*4+i]=2*p[1][i]-p[0][i];
                  coords[3*4+i]=p[1][i];
              }

            }break;
          case E:{
            for (int i=0;i<4;++i) {
                    coords[0*4+i]=q[1][i];
                    coords[1*4+i]=2*q[1][i]-q[0][i];

                    coords[2*4+i]=2*p[1][i]-p[0][i];
                    coords[3*4+i]=p[1][i];
                  }
            }break;
          case W:{
            for (int i=0;i<4;++i) {
                    coords[0*4+i]=q[1][i];
                    coords[1*4+i]=2*q[1][i]-q[0][i];

                    coords[2*4+i]=2*p[1][i]-p[0][i];
                    coords[3*4+i]=p[1][i];
                  }
            }break;
          }
      }break;
    case S:{
        switch(secondDirection){
          case N:{
              for (int i=0;i<4;++i) {
                  coords[0*4+3-i]=q[1][i];
                  coords[1*4+3-i]=2*q[1][i]-q[0][i];

                  coords[2*4+3-i]=2*p[1][i]-p[0][i];
                  coords[3*4+3-i]=p[1][i];
              }
            }break;
          case S:{
              for (int i=0;i<4;++i) {
                  coords[0*4+i]=q[1][i];
                  coords[1*4+i]=2*q[1][i]-q[0][i];

                  coords[2*4+i]=2*p[1][3-i]-p[0][3-i];
                  coords[3*4+i]=p[1][3-i];
              }
            }break;
          case E:{
            for (int i=0;i<4;++i) {
                    coords[0*4+3-i]=p[1][3-i];
                    coords[1*4+3-i]=2*p[1][3-i]-p[0][3-i];

                    coords[2*4+3-i]=2*q[1][3-i]-q[0][3-i];
                    coords[3*4+3-i]=q[1][3-i];
                  }
            }break;
          case W:{
            for (int i=0;i<4;++i) {
                    coords[0*4+i]=p[1][i];
                    coords[1*4+i]=2*p[1][i]-p[0][i];

                    coords[2*4+i]=2*q[1][3-i]-q[0][3-i];
                    coords[3*4+i]=q[1][3-i];
                  }
            }break;
          }
      }break;
    case E:{
         switch(secondDirection){
           case W:{
                     for (int i=0;i<4;++i) {
                       coords[0*4+i]=q[1][i];
                       coords[1*4+i]=2*q[1][i]-q[0][i];

                       coords[2*4+i]=2*p[1][i]-p[0][i];
                       coords[3*4+i]=p[1][i];
                     }
             }break;
           case E:{
               for (int i=0;i<4;++i) {
                 coords[0*4+i]=p[1][3-i];
                 coords[1*4+i]=2*p[1][3-i]-p[0][3-i];

                 coords[2*4+i]=2*q[1][i]-q[0][i];
                 coords[3*4+i]=q[1][i];
               }
             }break;
           case N:{
             for (int i=0;i<4;++i) {
                     coords[0*4+i]=q[1][3-i];
                     coords[1*4+i]=2*q[1][3-i]-q[0][3-i];

                     coords[2*4+i]=2*p[1][i]-p[0][i];
                     coords[3*4+i]=p[1][i];
                   }
             }break;
           case S:{
             for (int i=0;i<4;++i) {
                     coords[0*4+i]=p[1][3-i];
                     coords[1*4+i]=2*p[1][3-i]-p[0][3-i];

                     coords[2*4+i]=2*q[1][3-i]-q[0][3-i];
                     coords[3*4+i]=q[1][3-i];
                   }
             }break;
           }
      }break;
    case W:{
         switch(secondDirection){
           case E:{
                     for (int i=0;i<4;++i) {
                       coords[0*4+i]=p[1][i];
                       coords[1*4+i]=2*p[1][i]-p[0][i];

                       coords[2*4+i]=2*q[1][i]-q[0][i];
                       coords[3*4+i]=q[1][i];
                     }
             }break;
           case W:{
               for (int i=0;i<4;++i) {
                   coords[0*4+i]=q[1][i];
                   coords[1*4+i]=2*q[1][i]-q[0][i];

                   coords[2*4+i]=2*p[1][3-i]-p[0][3-i];
                   coords[3*4+i]=p[1][3-i];
               }
             }break;
           case N:{
             for (int i=0;i<4;++i) {
                     coords[0*4+i]=q[1][3-i];
                     coords[1*4+i]=2*q[1][3-i]-q[0][3-i];

                     coords[2*4+i]=2*p[1][3-i]-p[0][3-i];
                     coords[3*4+i]=p[1][3-i];
                   }
             }break;
           case S:{
             for (int i=0;i<4;++i) {
                     coords[0*4+i]=p[1][i];
                     coords[1*4+i]=2*p[1][i]-p[0][i];

                     coords[2*4+i]=2*q[1][3-i]-q[0][3-i];
                     coords[3*4+i]=q[1][3-i];
                   }
             }break;
           }
      }break;
  }

  //if( firstDirection < secondDirection ){
//      for (int i=0;i<4;++i) {
//        coords[0*4+i]=q[1][i];
//        coords[1*4+i]=2*q[1][i]-q[0][i];

//        coords[2*4+i]=2*p[1][i]-p[0][i];
//        coords[3*4+i]=p[1][i];
//      }
    //}else{
//      for (int i=0;i<4;++i) {
//        coords[0*4+i]=p[1][i];
//        coords[1*4+i]=2*p[1][i]-p[0][i];

//        coords[2*4+i]=2*q[1][i]-q[0][i];
//        coords[3*4+i]=q[1][i];
//      }
//    }


  GLdouble alpha_avg =(firstPatch->GetAlpha()+secondPatch->GetAlpha())/2.0;
  if(!insert(alpha_avg,1,coords,_patches[firstId]->material))return 2;
  switch (firstDirection) {
    case N:{
      _patches[firstId]->neighbours[S]=_patches[_patch_count-1];
      }break;
    case S:{
      _patches[firstId]->neighbours[N]=_patches[_patch_count-1];
      }break;
    }
  switch (secondDirection) {
    case S:{
      _patches[secondId]->neighbours[N]=_patches[_patch_count-1];
      }break;
    case N:{
      _patches[secondId]->neighbours[S]=_patches[_patch_count-1];
      }break;
    }
  _patches[_patch_count-1]->neighbours[N]=_patches[secondId];
  _patches[_patch_count-1]->neighbours[S]=_patches[firstId];
  //Set directions
  return 1;
  }

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

