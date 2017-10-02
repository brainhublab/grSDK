//#include <iostream>
//#include <sstream>
//#include <dbus/dbus.h>
//#include <deque>
//#include <stdlib.h>

//#include <thread>
//#include <time.h>
//#include <unistd.h>

#include <QApplication>
#include <GL/glut.h>
#include "grVisualization.h"

#include "grConnection.h"
#include "grAlgorithm.h"

using namespace std;
int main(int argc, char *argv[])
{
    sleep(5);
    glutInit(&argc, argv);
    QApplication a(argc, argv); // make an instance of Qt Application
    GRVisualization w; // Visualization initialize
    w.runDataReading();
    w.show(); // QMainWindow :: show*(), show window
    return a.exec();
}
