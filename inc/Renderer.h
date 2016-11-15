//
// Created by vtimofeev on 30/10/16.
//

#ifndef VISUALIZATION_RENDERER_H
#define VISUALIZATION_RENDERER_H

#include "openGL.h"
#include "Arm.h"

#include "Dimensions.h"

// Window size
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class Renderer
{
public:
		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		~Renderer(){};

public:
		bool drawScene(float, float, float);
		// renders arm using last boolean argument "isLeft"
		bool renderArm(struct Node*, float, float, float, bool);
		bool renderTrajectory(Arm*, float, float, float, bool);

public:
		int getWidth();
		int getHeight();
		bool setViewport(int, int);
		bool update();

private:
		render_data data;

private:
		bool initGL();
		bool drawFinger( Node *hand, float fingerDistance, int fingerIndex);

		// Geometric
		bool createRightHand(float, float, float);
		bool createLeftHand(float, float, float);

		bool createColorSphere(float, int, int, GLubyte r, GLubyte g, GLubyte b);
		bool createCube(float, float, float);
		bool createSphere(float, int, int);
		bool createCylinder(float, float);
		bool createFrustum(float, float, float);
};
#endif
