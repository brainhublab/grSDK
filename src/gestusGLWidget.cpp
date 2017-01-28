#include "gestusGLWidget.h"
#include <GL/glut.h>

GestusGLWidget::GestusGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    this->R = 1;
    this->wired = false;
    this->ang = 0.5;

    connect( &timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
    timer.start(16);
}

void GestusGLWidget::initializeGL()
{

    glClearColor( 0.0f, 0.5f, 0.5f, 0.0f );
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_COLOR_MATERIAL);
}

void GestusGLWidget::paintGL()
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    renderer.drawScene( angleX, angleY, angleZ );
    if ( renderWithHand )
    {
            renderer.renderArm( &rightArm, angleX, angleY, angleZ, false );
            renderer.renderArm( &leftArm, angleX, angleY, angleZ, true );
    }
    if ( renderWithTrajectory )
    {
            renderer.renderTrajectory( &rightArm, angleX, angleY, angleZ, false );
            renderer.renderTrajectory( &leftArm, angleX, angleY, angleZ, true );
    }
}

void GestusGLWidget::resizeGL(int w, int h)
{

    renderer.setViewport(w,h);/*
    glViewport(0,0, w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0, (float)w/h, 0.01, 100.0 );*/
    updateGL();
}
