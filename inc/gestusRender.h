#ifndef GESTUS_RENDERER_H
#define GESTUS_RENDERER_H

#include <GL/glu.h>

#include "gestusHand.h"
#include "cmath"
#include "dimensions.h"

// Window size
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class GestusRenderer
{
public:
        GestusRenderer();
        ~GestusRenderer(){};

        GestusRenderer(const GestusRenderer&) = delete;
        GestusRenderer& operator=(const GestusRenderer&) = delete;
        bool drawScene(float, float, float);
        bool setViewport(int, int);
        bool update();

		    bool renderArm(struct GestusHandNode*, float, float, float);
		    bool renderTrajectory(GestusHand*, float, float, float);
        // accessors
        int getWidth();
        int getHeight();
        bool setLinesColor(int r = 255, int g = 0, int b = 0, int a = 127);
        bool setPlaneColor(int r = 255, int g = 255, int b = 255, int a = 127);
private:
        render_data data;
        bool initGL();

        // Geometric
        bool drawFinger( GestusHandNode *hand, float fingerDistance, int fingerIndex);
        bool drawFingers(GestusHandNode *hand); // draws finders after hand was drawed
        bool drawRightHand(float, float, float);
        bool drawLeftHand(float, float, float);
        // Figures
        bool createColorSphere(float, int, int, GLubyte r, GLubyte g, GLubyte b);
        bool createCube(float, float, float);
        bool createSphere(float, int, int);
        bool createCylinder(float, float);
        bool createFrustum(float, float, float);
};
#endif // GESTUS_RENDERER_H
