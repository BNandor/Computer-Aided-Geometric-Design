#pragma once

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLFormat>
#include <QTimer>
#include "SideWidget.h"

#include "Parametric/ParametricCurves3.h"
#include "../Core/TriangulatedMeshes3.h"
#include "../Hyperbolic/HyperbolicArc3.h"
#include "../Hyperbolic/HyperbolicPatch3.h"
#include "../Cyclic/CyclicCurves3.h"
#include "../Hyperbolic/HyperbolicCompositeCurves3.h"
#include "../Hyperbolic/HyperbolicCompositePatch3.h"
#include "../Core/ShaderPrograms.h"

namespace cagd
{
    class GLWidget: public QGLWidget
    {
        Q_OBJECT

    private:

        // variables defining the projection matrix
        float       _aspect;            // aspect ratio of the rendering window
        float       _fovy;              // field of view in direction y
        float       _z_near, _z_far;    // distance of near and far clipping planes

        // variables defining the model-view matrix
        float       _eye[3], _center[3], _up[3];

        // variables needed by transformations
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;

        // your other declarations
        enum HomeWork{ParametricCurve,ParametricSurface,DynamicVBO,HyperbolicArc3,TensorProductSurface3,CyclicCurve,CompositeCurve,CompositePatch};
      ParametricCurve3* _pc;      
      GenericCurve3* _image_of_pc;

      //Testing dynamic vertex buffer objects
      QTimer* _timer;
      GLfloat _angle;
      //Parametric surfaces
      RowMatrix<TriangulatedMesh3*> surfaces;
      TriangulatedMesh3 _mouse;
      TriangulatedMesh3 _surface;
      //mine
      //ParametricCurves
      RowMatrix<GenericCurve3*> curves;
      float derivative_scale;
      //CyclicCurve3

      GenericCurve3* _cyclicCurve3Image;
      GenericCurve3* _cyclicCurve3InterpolationImage;
      CyclicCurve3* cyclicCurve3;
      //Hyperbolic arcs
      GenericCurve3*  _hyperbolicArc3Image;
      class HyperbolicArc3* hyperbolicArc3;
      //TensorProductSurfaces
      class TensorProductSurface3* tensorSurface3DataGrid;
      TriangulatedMesh3* hyperbolicPatch3Image;
      TriangulatedMesh3* hyperbolicPatch3InterpolationImage;
      RowMatrix<GenericCurve3*>* UISOLINES;
      RowMatrix<GenericCurve3*>* VISOLINES;
      HomeWork currentHomework=CompositePatch;

      // shaders
      int                  _shader_to_show = 1;
      bool                 _show_shader = true;
      ShaderProgram*       _shader[4];

      //Project
        //CompositeCurves
         Material patchMaterials[3];
         HyperbolicCompositeCurve3 * compositeCurve;
        //CompositePatches
         HyperbolicCompositePatch3 * compositePatch;
      //eof mine
    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0, const QGLFormat& format = QGL::Rgba | QGL::DepthBuffer | QGL::DoubleBuffer);

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);
        virtual ~GLWidget();

        //mine
        void initialize_curves();
        void initialize_surfaces();
        void updateXYZ(int, int);
        void updatePatchXYZ(int, int, int);
        SideWidget * _sideWidget;

        // shaders
        void installShaders();
    public slots:
        // public event handling methods/slots
        void set_angle_x(int value);
        void set_angle_y(int value);
        void set_angle_z(int value);

        void set_zoom_factor(double value);

        void set_trans_x(double value);
        void set_trans_y(double value);
        void set_trans_z(double value);
        //mine
        void change_curve(int index);
        void change_scale(int scale);
        void change_surface(int index);
        void change_homework(int index);
        void change_ArcAlpha(double);

        void insertArc();
        void continueArc();        
        void joinArcs();
        void changeTransformPointIndex(int);
        void changeTransformX(double);
        void changeTransformY(double);
        void changeTransformZ(double);                
        void mergeArcs();
          // sphere stuff
          void updateSpheresOnArcSelected(int);
        //Patches
        void insertPatch();
        void insertPatchX();
        void insertPatchY();
        void clearPatch();
        void continuePatch();
        void joinPatch();
        void transformPatchX();
        void transformPatchY();
        void transformPatchZ();
        void changeTransformPointsIndex();
        void mergePatches();
        void updateSphereOnPatchSelected();
        void updateSelectionCurveOnPatchSelected(GLuint patchIndex,HyperbolicCompositePatch3::Direction selectedDirection);
        void updateSelectionCurveOnContinue();
        void updateSelectionCurveOnJoinFirst();
        void updateSelectionCurveOnJoinSecond();
        void updateSelectionCurveOnMergeFirst();
        void updateSelectionCurveOnMergeSecond();
   private slots:
        void _animate();//Testing dynamic vertex buffer objects
    };
}
