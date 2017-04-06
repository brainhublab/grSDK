#ifndef GR_RENDERER_H
#define GR_RENDERER_H

#include <GL/glu.h>

#include "grHand.h"
#include "cmath"
#include "renderConfig.h"

// Window size
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class GRRenderer
{
public:
        GRRenderer();
        ~GRRenderer(){};

        GRRenderer(const GRRenderer&) = delete;
        GRRenderer& operator=(const GRRenderer&) = delete;
        bool drawScene(float, float, float);
        bool setViewport(int, int);
        bool update();

        bool renderArm(struct GRHandNode*, float, float, float);
	bool renderTrajectory(GRHand *, float, float, float);
        // accessors
        int getWidth();
        int getHeight();
        bool setLinesColor(int r = 255, int g = 0, int b = 0, int a = 200);
        bool setPlaneColor(int r = 255, int g = 255, int b = 255, int a = 200);
private:
        render_data data;
        bool initGL();

        // Geometric
        bool drawFinger( GRHandNode *hand, float fingerDistance, int fingerIndex);
        bool drawFingers(GRHandNode *hand); // draws finders after hand was drawed
        bool drawRightHand(float, float, float);
        bool drawLeftHand(float, float, float);
        // Figures
        bool createColorSphere(float, int, int, GLubyte r, GLubyte g, GLubyte b);
        bool createCube(float, float, float);
        bool createSphere(float, int, int);
        bool createCylinder(float, float);
        bool createFrustum(float, float, float);
};
#endif // GR_RENDERER_H
