#include "HyperbolicCompositePatch3.h"
#include <vector>

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
    if(!patchattr->generateImage())return GL_FALSE;
    if(!patchattr->updateVBO())return GL_FALSE;    
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

    //Clear selectionCurve
    clearSelectionCurve();
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

  GLdouble alpha_avg =(firstPatch->GetAlpha()+secondPatch->GetAlpha())/2.0;
  if(!insert(alpha_avg,1,coords,_patches[firstId]->material))return 2;
  switch (firstDirection) {
    case N:{
      _patches[firstId]->neighbours[N]=_patches[_patch_count-1];
      }break;
    case S:{
      _patches[firstId]->neighbours[S]=_patches[_patch_count-1];
      }break;
    case E:{
      _patches[firstId]->neighbours[E]=_patches[_patch_count-1];
      }break;
    case W:{
      _patches[firstId]->neighbours[W]=_patches[_patch_count-1];
      }break;
    }
  switch (secondDirection) {
    case S:{
      _patches[secondId]->neighbours[N]=_patches[_patch_count-1];
      }break;
    case N:{
      _patches[secondId]->neighbours[S]=_patches[_patch_count-1];
      }break;
    case E:{
      _patches[secondId]->neighbours[E]=_patches[_patch_count-1];
      }break;
    case W:{
      _patches[secondId]->neighbours[W]=_patches[_patch_count-1];
      }break;
    }
  _patches[_patch_count-1]->neighbours[N]=_patches[firstId];
  _patches[_patch_count-1]->neighbours[S]=_patches[secondId];
  //Set directions
  return 1;
  }

  GLboolean HyperbolicCompositePatch3::updatePatchForRendering( PatchAttributes* attr){
    attr->patch->DeleteVertexBufferObjectsOfData();
    if(!attr->patch->UpdateVertexBufferObjectsOfData())return GL_FALSE;
    if(!attr->generateImage())return GL_FALSE;
    if(!attr->updateVBO())return GL_FALSE;
    return GL_TRUE;
  }
  int kind(int i, int j){
    bool edgei,edgej;

    edgei = i==0 || i == 3;
    edgej = j==0 || j == 3;

    if(edgei && edgej){
      return 0;
     }
    if(!edgei && !edgej){
        return 2;
    }
   if (edgei)return -1;
   if (edgej)return -2;
  }

  struct corresponding{
    HyperbolicCompositePatch3::PatchAttributes* patchAttr;
    int i;
    int j;
  };

  vector<corresponding> getCorresponding( HyperbolicCompositePatch3::PatchAttributes* attr,DCoordinate3 toMove){
      vector<corresponding > result;
      for (int n = 0; n < 8; ++n) {
          if(attr->neighbours[n]) {
              for (int i=0;i<4;i++) {
                  for (int j=0;j<4;j++) {
                      DCoordinate3 coord;
                      (attr->neighbours[n])->patch->GetData(i,j,coord);

                      if(coord == toMove){
                        corresponding c;
                        c.i=i;
                        c.j=j;
                        c.patchAttr = (attr->neighbours[n]);
                        result.push_back(c);
                        }
                    }
                }
          }
      }
      return result;
  }

  vector<pair<int,int>> getPointNeighbours(int i, int j){
    vector<pair<int,int>> result;
    switch(i){
      case 0:{
          switch(j){
            case 0:{
                result.push_back(make_pair(0,1));
                result.push_back(make_pair(1,0));
                result.push_back(make_pair(1,1));
              };break;
            case 1:{
                result.push_back(make_pair(0,0));
                result.push_back(make_pair(1,1));
              };break;
            case 2:{
                result.push_back(make_pair(0,3));
                result.push_back(make_pair(1,2));
              };break;
            case 3:{
                result.push_back(make_pair(0,2));
                result.push_back(make_pair(1,2));
                result.push_back(make_pair(1,3));
              };break;
            }
        };break;
      case 1:{
          switch(j){
            case 0:{
                result.push_back(make_pair(0,0));
                result.push_back(make_pair(1,1));
              };break;
            case 1:{
                result.push_back(make_pair(0,1));
                result.push_back(make_pair(1,0));
              };break;
            case 2:{
                result.push_back(make_pair(0,2));
                result.push_back(make_pair(1,3));
              };break;
            case 3:{
                result.push_back(make_pair(1,2));
                result.push_back(make_pair(0,3));
              };break;
            }
        };break;
      case 2:{
          switch(j){
            case 0:{
              result.push_back(make_pair(2,1));
              result.push_back(make_pair(3,0));
              };break;
            case 1:{
              result.push_back(make_pair(2,0));
              result.push_back(make_pair(3,1));
              };break;
            case 2:{
              result.push_back(make_pair(2,3));
              result.push_back(make_pair(3,2));
              };break;
            case 3:{
              result.push_back(make_pair(2,2));
              result.push_back(make_pair(3,3));

              };break;
            }
        };break;
      case 3:{
          switch(j){
            case 0:{
                result.push_back(make_pair(2,0));
                result.push_back(make_pair(2,1));
                result.push_back(make_pair(3,1));
              };break;
            case 1:{
                result.push_back(make_pair(2,1));
                result.push_back(make_pair(3,0));
              };break;
            case 2:{
                result.push_back(make_pair(2,2));
                result.push_back(make_pair(3,3));
              };break;
            case 3:{
                result.push_back(make_pair(2,2));
                result.push_back(make_pair(3,2));
                result.push_back(make_pair(2,3));
              };break;
            }
        };break;
      }
    return result;
  }
  pair<int,int> getRowwiseNeighbour(int i, int j){
    vector<pair<int,int>> c = getPointNeighbours(i,j);

    for (int t=0;t<c.size();++t) {
        if(c[t].first == i) return c[t];
    }
  }
  pair<int,int> getColumnwiseNeighbour(int i, int j){
    vector<pair<int,int>> c = getPointNeighbours(i,j);
    for (int t=0;t<c.size();++t) {
        if(c[t].second == j) return c[t];
    }
  }
  GLboolean HyperbolicCompositePatch3::update(int i,int j, int patchIndex,DCoordinate3 newCoord){

    if(patchIndex < 0 || patchIndex >= _patch_count)return GL_FALSE;
    if(i < 0 || i > 3 || j < 0 || j>3)return GL_FALSE;
    // Clear selection curve
    clearSelectionCurve();

    DCoordinate3 currentCoord;
    (*(_patches[patchIndex]->patch)).GetData(i,j,currentCoord);
    DCoordinate3 diff = newCoord - currentCoord;    
    switch(kind(i,j)){
      case 0:{
        vector<corresponding> correspondence = getCorresponding(_patches[patchIndex],currentCoord);
        vector<pair<int,int>> tomodify;
        DCoordinate3 currentCoord;
        for (int c=0;c<correspondence.size();c++) {
            correspondence[c].patchAttr->patch->GetData(correspondence[c].i,correspondence[c].j,currentCoord);
            correspondence[c].patchAttr->patch->SetData(correspondence[c].i,correspondence[c].j,currentCoord+diff);
            tomodify=getPointNeighbours(correspondence[c].i,correspondence[c].j);
            for(int t=0;t<tomodify.size();++t){
                correspondence[c].patchAttr->patch->GetData(tomodify[t].first,tomodify[t].second,currentCoord);
                correspondence[c].patchAttr->patch->SetData(tomodify[t].first,tomodify[t].second,currentCoord+diff);
              }
            updatePatchForRendering(correspondence[c].patchAttr);
        }
        tomodify=getPointNeighbours(i,j);
        for(int t=0;t<tomodify.size();++t){
            _patches[patchIndex]->patch->GetData(tomodify[t].first,tomodify[t].second,currentCoord);
            _patches[patchIndex]->patch->SetData(tomodify[t].first,tomodify[t].second,currentCoord+diff);
          }
      }break;
      case -1:{
          vector<corresponding> correspondence = getCorresponding(_patches[patchIndex],currentCoord);
           pair<int,int> colwise;
          for (int c=0;c<correspondence.size();c++) {
              correspondence[c].patchAttr->patch->GetData(correspondence[c].i,correspondence[c].j,currentCoord);
              correspondence[c].patchAttr->patch->SetData(correspondence[c].i,correspondence[c].j,currentCoord+diff);
              colwise = getColumnwiseNeighbour(correspondence[c].i,correspondence[c].j);
              correspondence[c].patchAttr->patch->GetData(colwise.first,colwise.second,currentCoord);
              correspondence[c].patchAttr->patch->SetData(colwise.first,colwise.second,currentCoord+diff);

//              pair<int,int> rowwise=getRowwiseNeighbour(colwise.first,colwise.second);
//              correspondence[c].patchAttr->patch->GetData(rowwise.first,rowwise.second,currentCoord);
//              vector<corresponding> correspondenceofcor = getCorresponding(correspondence[c].patchAttr,currentCoord);
//              for (int c2=0;c2<correspondenceofcor.size();c2++) {
//                    pair<int, int> rowwisecor = getRowwiseNeighbour(correspondenceofcor[c2].i,correspondenceofcor[c2].j);
//                    correspondenceofcor[c2].patchAttr->patch->GetData(rowwisecor.first,rowwisecor.second,currentCoord);
//                    correspondenceofcor[c2].patchAttr->patch->SetData(rowwisecor.first,rowwisecor.second,currentCoord-diff);
//                    updatePatchForRendering(correspondenceofcor[c2].patchAttr);
//              }
              updatePatchForRendering(correspondence[c].patchAttr);
          }

//          pair<int,int> rowwise=getRowwiseNeighbour(i,j);
//          _patches[patchIndex]->patch->GetData(rowwise.first,rowwise.second,currentCoord);
//          vector<corresponding> correspondenceofcor = getCorresponding(_patches[patchIndex],currentCoord);
//          for (int c2=0;c2<correspondenceofcor.size();c2++) {
//                pair<int, int> rowwisecor = getRowwiseNeighbour(correspondenceofcor[c2].i,correspondenceofcor[c2].j);
//                correspondenceofcor[c2].patchAttr->patch->GetData(rowwisecor.first,rowwisecor.second,currentCoord);
//                correspondenceofcor[c2].patchAttr->patch->SetData(rowwisecor.first,rowwisecor.second,currentCoord-diff);
//                updatePatchForRendering(correspondenceofcor[c2].patchAttr);
//          }

//          colwise = getColumnwiseNeighbour(i,j);
//          rowwise=getRowwiseNeighbour(colwise.first,colwise.second);
//          _patches[patchIndex]->patch->GetData(rowwise.first,rowwise.second,currentCoord);
//          correspondenceofcor = getCorresponding(_patches[patchIndex],currentCoord);
//          for (int c2=0;c2<correspondenceofcor.size();c2++) {
//                pair<int, int> rowwisecor = getRowwiseNeighbour(correspondenceofcor[c2].i,correspondenceofcor[c2].j);
//                correspondenceofcor[c2].patchAttr->patch->GetData(rowwisecor.first,rowwisecor.second,currentCoord);
//                correspondenceofcor[c2].patchAttr->patch->SetData(rowwisecor.first,rowwisecor.second,currentCoord-diff);
//                updatePatchForRendering(correspondenceofcor[c2].patchAttr);
//          }
          colwise = getColumnwiseNeighbour(i,j);
          _patches[patchIndex]->patch->GetData(colwise.first,colwise.second,currentCoord);
          _patches[patchIndex]->patch->SetData(colwise.first,colwise.second,currentCoord+diff);
        }break;
      case -2:{
          (*(_patches[patchIndex]->patch)).GetData(i,j,currentCoord);
          vector<corresponding> correspondence = getCorresponding(_patches[patchIndex],currentCoord);
           pair<int,int> rowwise;
          for (int c=0;c<correspondence.size();c++) {
              correspondence[c].patchAttr->patch->GetData(correspondence[c].i,correspondence[c].j,currentCoord);
              correspondence[c].patchAttr->patch->SetData(correspondence[c].i,correspondence[c].j,currentCoord+diff);
//              rowwise = getRowwiseNeighbour(correspondence[c].i,correspondence[c].j);
//              correspondence[c].patchAttr->patch->GetData(rowwise.first,rowwise.second,currentCoord);
//              correspondence[c].patchAttr->patch->SetData(rowwise.first,rowwise.second,currentCoord+diff);

//              pair<int,int> rowwise=getRowwiseNeighbour(rowwise.first,rowwise.second);
//              correspondence[c].patchAttr->patch->GetData(rowwise.first,rowwise.second,currentCoord);
//              vector<corresponding> correspondenceofcor = getCorresponding(correspondence[c].patchAttr,currentCoord);
//              for (int c2=0;c2<correspondenceofcor.size();c2++) {
//                    pair<int, int> rowwisecor = getRowwiseNeighbour(correspondenceofcor[c2].i,correspondenceofcor[c2].j);
//                    correspondenceofcor[c2].patchAttr->patch->GetData(rowwisecor.first,rowwisecor.second,currentCoord);
//                    correspondenceofcor[c2].patchAttr->patch->SetData(rowwisecor.first,rowwisecor.second,currentCoord-diff);
//                    updatePatchForRendering(correspondenceofcor[c2].patchAttr);
//              }
              updatePatchForRendering(correspondence[c].patchAttr);
          }

//          pair<int,int> rowwise=getRowwiseNeighbour(i,j);
//          _patches[patchIndex]->patch->GetData(rowwise.first,rowwise.second,currentCoord);
//          vector<corresponding> correspondenceofcor = getCorresponding(_patches[patchIndex],currentCoord);
//          for (int c2=0;c2<correspondenceofcor.size();c2++) {
//                pair<int, int> rowwisecor = getRowwiseNeighbour(correspondenceofcor[c2].i,correspondenceofcor[c2].j);
//                correspondenceofcor[c2].patchAttr->patch->GetData(rowwisecor.first,rowwisecor.second,currentCoord);
//                correspondenceofcor[c2].patchAttr->patch->SetData(rowwisecor.first,rowwisecor.second,currentCoord-diff);
//                updatePatchForRendering(correspondenceofcor[c2].patchAttr);
//          }

//          rowwise = getColumnwiseNeighbour(i,j);
//          rowwise=getRowwiseNeighbour(rowwise.first,rowwise.second);
//          _patches[patchIndex]->patch->GetData(rowwise.first,rowwise.second,currentCoord);
//          correspondenceofcor = getCorresponding(_patches[patchIndex],currentCoord);
//          for (int c2=0;c2<correspondenceofcor.size();c2++) {
//                pair<int, int> rowwisecor = getRowwiseNeighbour(correspondenceofcor[c2].i,correspondenceofcor[c2].j);
//                correspondenceofcor[c2].patchAttr->patch->GetData(rowwisecor.first,rowwisecor.second,currentCoord);
//                correspondenceofcor[c2].patchAttr->patch->SetData(rowwisecor.first,rowwisecor.second,currentCoord-diff);
//                updatePatchForRendering(correspondenceofcor[c2].patchAttr);
//          }
          cout<<i<<" ij "<<j<<endl;
          rowwise = getRowwiseNeighbour(i,j);
          _patches[patchIndex]->patch->GetData(rowwise.first,rowwise.second,currentCoord);
          _patches[patchIndex]->patch->SetData(rowwise.first,rowwise.second,currentCoord+diff);
        }break;
    }

    (*(_patches[patchIndex]->patch)).GetData(i,j,currentCoord);
    _patches[patchIndex]->patch->SetData(i,j,currentCoord+diff);
    updatePatchForRendering(_patches[patchIndex]);
    return GL_TRUE;
  }

  GLuint HyperbolicCompositePatch3::merge(GLuint firstId, GLuint secondId,Direction firstDirection,Direction secondDirection){
    if(firstId < 0 || firstId >= _patch_count  )return GL_FALSE;
    if(secondId < 0 || secondId >= _patch_count  )return GL_FALSE;

    DCoordinate3 p[4];
    DCoordinate3 q[4];
    // Clear Selection curve
    clearSelectionCurve();
    switch (firstDirection) {
      case N:{
          switch (secondDirection) {
            case N:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(1,i,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(1,3-i,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(0,i,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(0,i,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case E:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(1,i,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(3-i,2,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(0,i,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(3-i,3,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case S:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(1,i,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(2,i,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(0,i,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(3,i,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case W:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(1,i,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(i,1,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(0,i,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(i,0,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            }
        }break;
      case E:{
          switch (secondDirection) {
            case N:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(i,2,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(1,3-i,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(i,3,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(0,3-i,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case E:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(i,2,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(i,2,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(i,3,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(i,3,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case S:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(i,2,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(2,i,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(i,3,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(3,i,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case W:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(i,2,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(i,1,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(i,3,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(i,0,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            }
        }break;
      case S:{
          switch (secondDirection) {
            case N:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(2,i,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(1,i,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(3,i,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(0,i,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case E:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(2,i,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(i,2,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(3,i,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(i,3,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case S:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(2,i,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(2,3-i,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(3,i,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(3,3-i,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case W:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(2,i,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(3-i,1,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(3,i,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(3-i,0,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            }
        }break;
      case W:{
          switch (secondDirection) {
            case N:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(i,1,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(1,i,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(i,0,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(0,i,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case E:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(i,1,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(i,2,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(i,0,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(i,3,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case S:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(i,1,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(2,3-i,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(i,0,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(3,3-i,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            case W:{
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).GetData(i,1,p[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[secondId]->patch)).GetData(3-i,1,q[i]);
                }
                for (int i=0;i<4;++i) {
                    (*(_patches[firstId]->patch)).SetData(i,0,(1/2.0)*(p[i]+q[i]));
                    (*(_patches[secondId]->patch)).SetData(3-i,0,(1/2.0)*(p[i]+q[i]));
                }
              }break;
            }
        }break;
      }
    updatePatchForRendering(_patches[firstId]);
    updatePatchForRendering(_patches[secondId]);
  }

void HyperbolicCompositePatch3::renderAll(){
  if(sphere->_image){
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glEnable(GL_NORMALIZE);
      MatFBTurquoise.Apply();
      sphere->_image->Render();
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
      glDisable(GL_NORMALIZE);
    }

  if(selectedPatchBorderCurve){
    glColor3f(0.2f,0.6f,0.6f);
    glLineWidth(5.0);
    selectedPatchBorderCurve->RenderDerivatives(0,GL_LINE_STRIP);
    glLineWidth(1.0);
    }

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
          //render u,v isoparametric lines
           glColor3f(0.2f,0.6f,0.6f);
          if(_patches[i]->ulines){
              for (int t=0;t<_patches[i]->ulines->GetColumnCount();++t) {
                (*_patches[i]->ulines)[t]->RenderDerivatives(0,GL_LINE_STRIP);
                (*_patches[i]->ulines)[t]->RenderDerivatives(1,GL_LINES);
                }
            }
          glColor3f(0.1f,0.7f,0.6f);
          if(_patches[i]->vlines){
              for (int t=0;t<_patches[i]->vlines->GetColumnCount();++t) {
                (*_patches[i]->vlines)[t]->RenderDerivatives(0,GL_LINE_STRIP);
                (*_patches[i]->vlines)[t]->RenderDerivatives(1,GL_LINES);
                }
            }
    }
  }
}
}



