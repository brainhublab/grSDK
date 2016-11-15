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
		void drawScene(float, float, float);
		// renders arm using last boolean argument "isLeft"
		void renderArm(struct Node*, float, float, float, bool);
		void renderTrajectory(Arm*, float, float, float, bool);

public:
		int getWidth();
		int getHeight();
		void setViewport(int, int);
		void update();

private:
		render_data data;

private:
		void initGL();
		void drawFinger( Node *hand, float fingerDistance, int fingerIndex);

		// Geometric
		void createRightHand(float, float, float);
		void createLeftHand(float, float, float);

		void createColorSphere(float, int, int, GLubyte r, GLubyte g, GLubyte b);
		void createCube(float, float, float);
		void createSphere(float, int, int);
		void createCylinder(float, float);
		void createFrustum(float, float, float);
};
#endif
