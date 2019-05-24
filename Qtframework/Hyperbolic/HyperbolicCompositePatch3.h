#ifndef HYPERBOLICCOMPOSITEPATCH3_H
#define HYPERBOLICCOMPOSITEPATCH3_H


#include "HyperbolicPatch3.h"
#include "../Core/TensorProductSurfaces3.h"
#include "../Core/Colors4.h"
#include "../Core/Materials.h"
#include "./IndicatingSphere.h"
#include <string.h>
#include <vector>

using namespace std;

namespace cagd {
  class HyperbolicCompositePatch3 {
  public:
    enum Direction{N=0,NE=1,E=2,SE=3,S=4,SW=5,W=6,NW=7};
    static const GLuint div_point_count = 20;
    constexpr static const GLdouble derivative_scale = 0.3;
    Color4 default_derivatives_colour;
    IndicatingSphere * sphere;
    GenericCurve3* selectedPatchBorderCurve;

    class PatchAttributes{
    public:
        HyperbolicPatch3 * patch;
        TriangulatedMesh3 * img;
        Material  material;
        Color4 * derivatives_color;
        PatchAttributes* neighbours[8];
        PatchAttributes():patch(0),img(0),material(MatFBEmerald){
          memset(neighbours,0,8*sizeof(PatchAttributes*));
        }
        PatchAttributes(const PatchAttributes & other);
        PatchAttributes& operator=(const PatchAttributes & other);
        ~PatchAttributes();

        GLboolean generateImage(){
          img = patch->GenerateImage(div_point_count,div_point_count);
          return img != 0;
        }
        GLboolean updateVBO(){
           if(!img)return GL_FALSE;
           return img->UpdateVertexBufferObjects();
        }
    };

  protected:
    vector<PatchAttributes*> _patches;
    GLuint _patch_count;
  public:
    void clear(){
      for (int i=0;i<_patch_count;++i) {
          delete _patches[i];
          _patches[i]=0;
        }
      _patch_count=0;
      if(selectedPatchBorderCurve){
          delete selectedPatchBorderCurve;
        selectedPatchBorderCurve=0;
      }
      if(sphere->_image){
          delete sphere->_image;
          sphere->_image=0;
        }
    }    
    DCoordinate3 getCoord(int i, int j, int index){
      DCoordinate3 coord;
      _patches[index]->patch->GetData(i,j,coord);
      return coord;
    }
    int getSize(){return _patch_count;}
    PatchAttributes* getPatch(int index){return _patches[index];}
    void updateSpherePosition(GLuint row,GLuint column,GLuint patchIndex){
      if(patchIndex >= _patch_count){
          cerr<<"Error: invalid patch index selected"<<endl;
          return;
        }
      if(row>=4|| column >=4){
          cerr<<"Error: invalid row, or column selected"<<endl;
          return;
        }
      DCoordinate3 pointCurrentPosition;
      (_patches[patchIndex]->patch)->GetData(row,column,pointCurrentPosition);
      sphere->updateImage(pointCurrentPosition);
    }
    void updateSelectionCurve(GLuint patchIndex, Direction selectedDirection){
      if(patchIndex>=_patch_count || selectedDirection == NE || selectedDirection == NW ||selectedDirection == SE ||selectedDirection == SW){
          cerr<<"Error: patch index out of bounds, or wrong direction"<<endl;
          return;
        }

      if(selectedPatchBorderCurve)delete  selectedPatchBorderCurve;
      selectedPatchBorderCurve = 0;
      switch (selectedDirection) {
        case N:{
          RowMatrix<GenericCurve3*>* isocurves = _patches[patchIndex]->patch->GenerateUIsoparametricLines(2,0,100);
          selectedPatchBorderCurve = (*isocurves)[0];
          if(!selectedPatchBorderCurve){
              cerr<<"Error generating isoparametric border line "<<endl;
              return;
          }
          if(!selectedPatchBorderCurve->UpdateVertexBufferObjects()){
              cerr<<"Error updating vbo of selectedPatchBorderCurve"<<endl;
              return;
            }
          delete (*isocurves)[1];
          }break;
        case S:{
          RowMatrix<GenericCurve3*>* isocurves = _patches[patchIndex]->patch->GenerateUIsoparametricLines(2,0,100);
          selectedPatchBorderCurve = (*isocurves)[1];
          if(!selectedPatchBorderCurve){
              cerr<<"Error generating isoparametric border line "<<endl;
              return;
          }
          if(!selectedPatchBorderCurve->UpdateVertexBufferObjects()){
              cerr<<"Error updating vbo of selectedPatchBorderCurve"<<endl;
              return;
            }
          delete (*isocurves)[0];
          }break;
        case W:{
          RowMatrix<GenericCurve3*>* isocurves = _patches[patchIndex]->patch->GenerateVIsoparametricLines(2,0,100);
          selectedPatchBorderCurve = (*isocurves)[0];
          if(!selectedPatchBorderCurve){
              cerr<<"Error generating isoparametric border line "<<endl;
              return;
          }
          if(!selectedPatchBorderCurve->UpdateVertexBufferObjects()){
              cerr<<"Error updating vbo of selectedPatchBorderCurve"<<endl;
              return;
            }
          delete (*isocurves)[1];
          }break;
        case E:{
          RowMatrix<GenericCurve3*>* isocurves = _patches[patchIndex]->patch->GenerateVIsoparametricLines(2,0,100);
          selectedPatchBorderCurve = (*isocurves)[1];
          if(!selectedPatchBorderCurve){
              cerr<<"Error generating isoparametric border line "<<endl;
              return;
          }
          if(!selectedPatchBorderCurve->UpdateVertexBufferObjects()){
              cerr<<"Error updating vbo of selectedPatchBorderCurve"<<endl;
              return;
            }
          delete (*isocurves)[0];
          }break;
        }
    }
    void clearSelectionCurve(){
      if(selectedPatchBorderCurve)delete selectedPatchBorderCurve;
      selectedPatchBorderCurve = 0;
    }
    HyperbolicCompositePatch3(GLuint max_curve_count):_patches(max_curve_count),_patch_count(0),selectedPatchBorderCurve(0){
      default_derivatives_colour=Color4(0,0.5,0);
      sphere = new IndicatingSphere(0.02);
    }
    GLboolean insert(GLdouble alpha,GLuint max_order_of_derivatives,const ColumnMatrix<DCoordinate3>& _data,Material material=MatFBEmerald);
    GLboolean continueExisting(GLuint id,Direction direction,GLdouble alpha,Material material);
    GLuint join(GLuint firstId, GLuint SecondID,Direction firstDirection,Direction secondDirection);
    GLuint merge(GLuint firstId, GLuint SecondID,Direction firstDirection,Direction secondDirection);
    GLboolean update(int i,int j,int patchindex,DCoordinate3 newcoord);
    GLboolean updatePatchForRendering( PatchAttributes*);
    void renderAll();
    ~HyperbolicCompositePatch3();
  };
}

#endif // HYPERBOLICCOMPOSITEPATCH3_H
