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

    void initializeGL(); // init properties as background color
    void paintGL(); // set which items renderer needs to paint
    void resizeGL(int w, int h); // resize and optimize image in renderer

    void renderTrajectory(bool b); // set new value to renderWithTrajectory variable
    void renderLeftHand(bool b); // set new value to renderLeftHand variable
    void renderRightHand(bool b); // set new value to renderRightHand variable

    GRRenderer *getRenderer(); // returns pointer to renderer

    // Like data keepers for each arm
    GRHand leftArm = GRHand(true);
    GRHand rightArm = GRHand(false);

private:

    GRRenderer renderer; // paints scenes, hands, fingers etc.
    QTimer updateTimer;
	bool renderWithTrajectory = false,
			renderWithLeftHand = true,
            renderWithRightHand = true;
    float angleX = 0.f, angleY = 0.f, angleZ = 0.f;

};

#endif // GRGLWidget_H
