//
// Created by vtimofeev on 06/11/16.
//

#ifndef VISUALIZATION_GUIMETHODS_H
#define VISUALIZATION_GUIMETHODS_H

#include "arm.h"
#include "ui/imgui_impl_sdl.h"
#include <deque>

// todo : move all of this hell to an class
class VisualGui
{
public:
	VisualGui();
	~VisualGui();
private:
		bool drawChart(const float* data, size_t size, float min, float max);
		bool drawTrajectiiryByAngleName(char name ,const std::vector<std::map<char, float>>& trajectory, float min, float max);
		bool drawTrajectoryChart(const std::vector<std::map<char, float>>& trajectory, Arm* arm);
		bool renderChartFromBuffer(std::string name, const std::deque< std::string > &buffer, size_t axis);
public:
		bool plotData(std::map< std::string, std::deque<std::string>*>& buffers);
		bool addPlotData(double[3]); // custom data plotting
		bool drawDataChart(const std::vector<std::map<char, float>>& data);
		bool drawMenu(Arm* leftArm, Arm* rightArm ,bool* renderWithHand, bool* renderWithTrajectory, float * angleX, float * angleY, float * angleZ);

private:
	std::vector<std::map<char, float>> dataForPlot;
	bool showChartWindow = false;
  bool showDataChartWindow = false;
	float chartArray[600];
};

#endif
