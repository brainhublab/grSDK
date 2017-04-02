#ifndef GR_GL_WIDGET_H
#define GR_GL_WIDGET_H

#include <QKeyEvent>
#include <QGLWidget>
#include <QTimer>
#include "grRender.h"

class GRGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GRGLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void renderTrajectory(bool b);
    void renderLeftHand(bool b);
    void renderRightHand(bool b);

    GRRenderer* getRenderer();

    int R;
    float x,y,z;
    bool wired;
    float ang;


    	GRHand leftArm = GRHand(true);
    	GRHand rightArm = GRHand(false);
private:

	GRRenderer renderer;
  QTimer timer;
  bool renderWithTrajectory = false,
       renderWithLeftHand = true,
       renderWithRightHand = true;
  float angleX = 0.f, angleY = 0.f, angleZ = 0.f;

};

#endif // GRGLWidget_H
