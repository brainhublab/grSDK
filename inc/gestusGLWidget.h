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

    void setRenderTrajectory(bool b)
    {
        renderWithTrajectory = b;
    }
    void setRenderHand(bool b)
    {
        renderWithHand = b;
    }

	GestusRenderer* getRenderer()
    {
        return &renderer;
    }

	GestusHand* getArm()
    {
        return &leftArm;
    }

    int R;
    float x,y,z;
    bool wired;
    float ang;

    void keyPressEvent(QKeyEvent* event) {
        switch(event->key()) {
        case Qt::Key_Escape:
            leftArm.bendArm(10, 10 , 10);
            rightArm.bendArm(-10, -10 , -10);
            break;
        default:
            event->ignore();
            break;
        }
    };
private:

	GestusRenderer renderer;
    QTimer timer;

	GestusHand leftArm;
	GestusHand rightArm;
    bool renderWithTrajectory = true,
         renderWithHand = true;
    float angleX = 0.f, angleY = 0.f, angleZ = 0.f, angleStep = 1.5f;

};

#endif // GestusGLWidget_H
