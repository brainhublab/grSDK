#include "mainwindow.h"
#include <QApplication>
#include <GL/glut.h>
#include "gestusConnection.h"

int main(int argc, char *argv[])
{

    std::deque<std::string> acc;
    std::deque<std::string> gyro;
    std::deque<std::string> mag;

    GestusConnection connection;
    connection.setAvalibleDevices();
    connection.getData(0, "acc", &acc);
    connection.getData(0, "gyro", &gyro);
    connection.getData(0, "magnet", &mag);

    //acc.push_back("123 120 100");
    //gyro.push_back("123 120 100");
    //mag.push_back("123 120 100");


    glutInit(&argc, argv);
    QApplication a(argc, argv);
    MainWindow w;

    w.setupPlotters(&acc, &gyro, &mag);
    w.show();
    //w.realTimeDrowData(&buffer);

    return a.exec();
}
