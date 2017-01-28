#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include "renderer.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void setRenderTrajectory(bool b)
    {
        renderWithTrajectory = b;
    }
    void setRenderHand(bool b)
    {
        renderWithHand = b;
    }

    Renderer* getRenderer()
    {
        return &renderer;
    }

    Arm* getArm()
    {
        return &leftArm;
    }

    int R;
    float x,y,z;
    bool wired;
    float ang;

private:

    Renderer renderer;
    QTimer timer;

    Arm leftArm;
    Arm rightArm;
    bool renderWithTrajectory = true,
         renderWithHand = true;
    float angleX = 0.f, angleY = 0.f, angleZ = 0.f, angleStep = 1.5f;

};

#endif // GLWIDGET_H
