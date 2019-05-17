#include "MainWindow.h"

namespace cagd
{
    MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    {
        setupUi(this);

    /*

      the structure of the main window's central widget

     *---------------------------------------------------*
     |                 central widget                    |
     |                                                   |
     |  *---------------------------*-----------------*  |
     |  |     rendering context     |   scroll area   |  |
     |  |       OpenGL widget       | *-------------* |  |
     |  |                           | | side widget | |  |
     |  |                           | |             | |  |
     |  |                           | |             | |  |
     |  |                           | *-------------* |  |
     |  *---------------------------*-----------------*  |
     |                                                   |
     *---------------------------------------------------*

    */
        _side_widget = new SideWidget(this);

        _scroll_area = new QScrollArea(this);
        _scroll_area->setWidget(_side_widget);
        _scroll_area->setSizePolicy(_side_widget->sizePolicy());
        _scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        _gl_widget = new GLWidget(this);
        _gl_widget->_sideWidget = _side_widget;

        centralWidget()->setLayout(new QHBoxLayout());
        centralWidget()->layout()->addWidget(_gl_widget);
        centralWidget()->layout()->addWidget(_scroll_area);

        // creating a signal slot mechanism between the rendering context and the side widget
        connect(_side_widget->rotate_x_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_x(int)));
        connect(_side_widget->rotate_y_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_y(int)));
        connect(_side_widget->rotate_z_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_z(int)));

        connect(_side_widget->zoom_factor_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_zoom_factor(double)));

        connect(_side_widget->trans_x_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_x(double)));
        connect(_side_widget->trans_y_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_y(double)));
        connect(_side_widget->trans_z_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_z(double)));
        connect(_side_widget->comboBox,SIGNAL(currentIndexChanged(int)),_gl_widget,SLOT(change_curve(int)));
        connect(_side_widget->scaleSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(change_scale(int)));        
        connect(_side_widget->surfaceCombo,SIGNAL(currentIndexChanged(int)),_gl_widget,SLOT(change_surface(int)));
        connect(_side_widget->homeworkChooser,SIGNAL(currentIndexChanged(int)),_gl_widget,SLOT(change_homework(int)));
        connect(_side_widget->ArcAlphaSpinBox,SIGNAL(valueChanged(double)),_gl_widget,SLOT(change_ArcAlpha(double)));

        connect(_side_widget->ArcInsertButton, SIGNAL(clicked()),_gl_widget,SLOT(insertArc()));
        connect(_side_widget->ArcContinueButton, SIGNAL(clicked()),_gl_widget,SLOT(continueArc()));
        connect(_side_widget->ArcJoinButton, SIGNAL(clicked()),_gl_widget,SLOT(joinArcs()));

        connect(_side_widget->ArcTransformPointIndex, SIGNAL(valueChanged(int)),_gl_widget,SLOT(changeTransformPointIndex(int)));
        connect(_side_widget->ArcTransformX,SIGNAL(valueChanged(double)),_gl_widget,SLOT(changeTransformX(double)));
        connect(_side_widget->ArcTransformY,SIGNAL(valueChanged(double)),_gl_widget,SLOT(changeTransformY(double)));
        connect(_side_widget->ArcTransformZ,SIGNAL(valueChanged(double)),_gl_widget,SLOT(changeTransformZ(double)));

        connect(_side_widget->ArcMergeButton, SIGNAL(clicked()),_gl_widget,SLOT(mergeArcs()));
        //Patch
        connect(_side_widget->PatchInsertButton, SIGNAL(clicked()),_gl_widget,SLOT(insertPatch()));
        connect(_side_widget->PatchContinueButton, SIGNAL(clicked()),_gl_widget,SLOT(continuePatch()));

    }

    //--------------------------------
    // implementation of private slots
    //--------------------------------
    void MainWindow::on_action_Quit_triggered()
    {
        qApp->exit(0);
    }
}
