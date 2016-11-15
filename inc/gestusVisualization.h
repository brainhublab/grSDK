#ifndef VISUALIZATION
#define VISUALIZATION

#include <string>
#include "VGui.h"
#include "Renderer.h"

class Visualization
{
public:
		Visualization(std::string& firstId, std::string& firstBuffer, std::string& secondId, std::string& secondBuffer);
		~Visualization();
		Visualization(const Visualization&) = delete;
		Visualization&operator=(const Visualization&) = delete;
		bool run();
		bool stop();

private:
		VGui gui;
		// todo: move it to Hand class?
		std::string& _firstHandId;
		std::string& _firstBuffer;
		std::string& _secondHandId;
		std::string& _secondBuffer;
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
