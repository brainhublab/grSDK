#include <iostream>
//#include </usr/include/dbus-1.0/dbus/dbus.h>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>
#include "grConnection.h"
#include <thread>
#include <time.h>
#include <unistd.h>
#include <QApplication>
#include <GL/glut.h>

#include "grVisualization.h"
#include "grConnection.h"

int main(int argc, char *argv[])
{

	std::deque<std::string> acc;
	std::deque<std::string> gyro;
	std::deque<std::string> mag;

	GRConnection connection;
	// connection.setAvalibleDevices();
	// connection.getData(0, "acc", &acc);
	// connection.getData(0, "gyro", &gyro);
	// connection.getData(0, "magnet", &mag);

	// acc.push_back("123 120 100");
	// acc.push_back("134 400 23");
	// gyro.push_back("13 1220 1030");
  // mag.push_back("1132 10 0");


	glutInit(&argc, argv);
	QApplication a(argc, argv);
	GRVisualization w;

	w.setupPlotters(&acc, &gyro, &mag);
	w.show();
	//w.realTimeDrowData(&buffer);

	return a.exec();
}
