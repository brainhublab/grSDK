#include "grVisualization.h"
#include "ui_grVisualisation.h"



GRVisualization::GRVisualization(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::GRVisualization)
{
	ui->setupUi(this);
	initUiProps();

	rightArmApplier.setArm(&ui->GLwidget->rightArm);

	plotter_acc = new GRDataPlotter(ui->only_accelerometer);
	plotter_gyro = new GRDataPlotter(ui->only_gyroscope);
	plotter_mag = new GRDataPlotter(ui->only_magnetometer);

	plotter_all_acc = new GRDataPlotter(ui->accelerometer);
	plotter_all_gyro = new GRDataPlotter(ui->gyroscope);
	plotter_all_mag = new GRDataPlotter(ui->magnetometer);

}

GRVisualization::~GRVisualization()
{
	delete plotter_acc;
	delete plotter_gyro;
	delete plotter_mag;

	delete plotter_all_acc;
	delete plotter_all_gyro;
	delete plotter_all_mag;
	delete ui;
}

bool GRVisualization::initUiProps()
{
	// splitter init
	QList<int> sizes;
	sizes.push_back(300);
	sizes.push_back(100);
	ui->splitter->setSizes(sizes);
	// checkboxes style init
	ui->hackerModeCheckBox->setStyleSheet("background-color: #008081;");
	ui->leftHandCheckBox->setStyleSheet("background-color: #008081;");
	ui->rightHandCheckBox->setStyleSheet("background-color: #008081;");
	ui->trajectoryCheckBox->setStyleSheet("background-color: #008081;");
	ui->loggingCheckBox->setStyleSheet("background-color: #008081;");
	ui->settings->setStyleSheet("background-color: #008081;");
	ui->devicesTree->setStyleSheet("background-color: #008081;");
	// QColor backgroundColor = ui->settings->palette().light().color();
	// backgroundColor.setAlpha(100);
	// QPainter customPainter(this);
	// customPainter.fillRect(rect(),backgroundColor);

	// disable push random data button
	ui->randomData->setVisible(false);


	return true;
}

bool GRVisualization::runDataReading()
{
	rightArmApplier.run();
	return true;
}

//
// Qt Slots
//

void GRVisualization::on_trajectoryCheckBox_toggled(bool checked)
{
	ui->GLwidget->renderTrajectory(checked);
}

void GRVisualization::on_leftHandCheckBox_toggled(bool checked)
{
	ui->GLwidget->renderLeftHand(checked);
}

void GRVisualization::on_rightHandCheckBox_toggled(bool checked)
{
	ui->GLwidget->renderRightHand(checked);
}

void GRVisualization::on_hackerModeCheckBox_toggled(bool checked)
{
	if (checked)
	{
		ui->GLwidget->getRenderer()->setPlaneColor(0, 255, 0, 0);
		// ui->GLwidget->getRenderer()->setLinesColor(255,193,7, 255);
		ui->GLwidget->getRenderer()->setLinesColor(0, 255, 0, 255);
		ui->hackerModeCheckBox->setStyleSheet("color: white;"
				                                      "background: #e67e22;"
				                                      "selection-color: #34495e;"
				                                      "selection-background-color: white;");
	} else
	{
		// go to default
		ui->GLwidget->getRenderer()->setLinesColor(255, 0, 0, 255);
		ui->GLwidget->getRenderer()->setPlaneColor(255, 255, 255, 255);
		ui->hackerModeCheckBox->setStyleSheet("color: white;"
				                                      "background: #008081;"
				                                      "selection-color: white;"
				                                      "selection-background-color: white;");
	}

}

void GRVisualization::on_loggingCheckBox_toggled(bool checked)
{
	ui->randomData->setVisible(checked);

	rightArmApplier.isLoggingEnabled = checked;
}

void GRVisualization::on_randomData_clicked()
{
	printf("\nGRVisualization: The function Random data is disabled now\n");
//    int max = 256, min = -256;
//    std::vector<float> s;
//    s.push_back(qrand() % ((max + 1) - min) + min);
//    //s.append(" ");
//    s.push_back(0);//qrand() % ((max + 1) - min) + min));
//    //s.append(" ");
//    s.push_back(0);//qrand() % ((max + 1) - min) + min));
//    //rightArmApplier.sourceBuffer->push_back(s);

}

void GRVisualization::on_pausePlotCheckBox_toggled(bool checked)
{
	plotter_acc->pause = checked;
	plotter_mag->pause = checked;
	plotter_gyro->pause = checked;

	plotter_all_acc->pause = checked;
	plotter_all_mag->pause = checked;
	plotter_all_gyro->pause = checked;
}

