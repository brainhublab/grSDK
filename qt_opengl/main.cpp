#include "mainwindow.h"
#include <QApplication>
#include <GL/glut.h>

int main(int argc, char *argv[])
{
    std::deque<std::string> buffer;
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.realTimeDrowData(&buffer);

    return a.exec();
}
