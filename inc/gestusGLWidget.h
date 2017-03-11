#ifndef GESTUS_GL_WIDGET_H
#define GESTUS_GL_WIDGET_H

#include <QKeyEvent>
#include <QGLWidget>
#include <QTimer>
#include "gestusRender.h"

class GestusGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GestusGLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void renderTrajectory(bool b);
    void renderLeftHand(bool b);
    void renderRightHand(bool b);

    GestusRenderer* getRenderer();

    int R;
    float x,y,z;
    bool wired;
    float ang;
private:

	GestusRenderer renderer;
  QTimer timer;

	GestusHand leftArm = GestusHand(true);
	GestusHand rightArm = GestusHand(false);
  bool renderWithTrajectory = false,
       renderWithLeftHand = true,
       renderWithRightHand = true;
  float angleX = 0.f, angleY = 0.f, angleZ = 0.f;

};

#endif // GestusGLWidget_H
