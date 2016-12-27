#ifndef VISUALIZATION
#define VISUALIZATION

#include "VGui.h"
#include "Renderer.h"

#include <string>
#include <deque>

class Visualization
{
public:
		Visualization();
		~Visualization();
		Visualization(const Visualization&) = delete;
		Visualization&operator=(const Visualization&) = delete;
		bool init(const int w, const int h);
		bool run();
		bool stop();
		bool addPlotData(double[3]);
		bool visualizeData(std::map< std::string, std::deque<std::string>*>&);
private:
		VGui gui;
		Arm leftArm;
		Arm rightArm;
		//
		bool setOpenGLVersion();
		bool quit = false;
		bool close();
		// SDL stuff
		SDL_Window *window;
		SDL_GLContext glContext;
		// states
		bool renderWithTrajectory = false,
						renderWithHand = true;
		float angleX = 0.f, angleY = 0.f, angleZ = 0.f, angleStep = 1.5f;
};

#endif
