//
// Created by vtimofeev on 06/11/16.
//

#ifndef VISUALIZATION_GUIMETHODS_H
#define VISUALIZATION_GUIMETHODS_H

#include "Arm.h"
#include "ui/imgui_impl_sdl.h"

// todo : move all of this hell to an class
class VGui
{
public:
	VGui();
	~VGui();
private:
		bool drawChart(const float* data, size_t size, float min, float max);
		bool drawTrajectiiryByAngleName(char name ,const std::vector<std::map<char, float>>& trajectory, float min, float max);
		bool drawTrajectoryChart(const std::vector<std::map<char, float>>& trajectory, Arm* arm);
public:
		bool addPlotData(double[3]); // custom data plotting
		bool drawDataChart(const std::vector<std::map<char, float>>& data);
		bool drawMenu(Arm* leftArm, Arm* rightArm ,bool* renderWithHand, bool* renderWithTrajectory, float * angleX, float * angleY, float * angleZ);

private:
	std::vector<std::map<char, float>> dataForPlot;
	bool showChartWindow = false;
	float chartArray[600];
};

#endif
