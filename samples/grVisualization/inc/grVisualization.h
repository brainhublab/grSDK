#ifndef GR_VISUALISATION_H
#define GR_VISUALISATION_H


//#define GR_VISUALIZATION_LOGGING_ENABLED

#include <QMainWindow>
#include <QKeyEvent>
#include <QObject>

#include <thread>
#include <deque>

#include "GRGLWidget.h"
#include "grDataPlotter.h"
#include "grDataApplier.h"
#include <iostream>

namespace Ui
{
	class GRVisualization;
}

class GRVisualization : public QMainWindow
{
Q_OBJECT

public:
	explicit GRVisualization(QWidget *parent = 0);
	~GRVisualization();

    bool runDataReading(); // runs dataAppliers for hands
private slots:
	void on_trajectoryCheckBox_toggled(bool checked);
	void on_leftHandCheckBox_toggled(bool checked);
	void on_rightHandCheckBox_toggled(bool checked);
	void on_hackerModeCheckBox_toggled(bool checked);
	void on_loggingCheckBox_toggled(bool checked);
	void on_randomData_clicked();
	void on_pausePlotCheckBox_toggled(bool checked);
    void on_rotationsCheckBox_toggled(bool checked);

private:
	bool initUiProps();

    	//GRConnection conn; // connection for data getting
	std::map<int, device_t> activeDevices;	
	
	Ui::GRVisualization *ui; // pointer to GUI
	bool withTrajectory = false;
	bool withRotations = true;
    GRDataApplier rightArmApplier; // applier for right hand
    GRDataApplier leftArmApplier; // applier for left hand
    std::deque<std::vector<float>>* buffer; // pointer to buffer of data
	// TODO : Maybe move it to GRDataApplier, because it manages all data, not grVisualization
	GRDataPlotter *plotter_acc;
	GRDataPlotter *plotter_gyro;
	GRDataPlotter *plotter_mag;
	GRDataPlotter *plotter_all_acc;
	GRDataPlotter *plotter_all_gyro;
	GRDataPlotter *plotter_all_mag;
};

#endif // MAINWINDOW_H
