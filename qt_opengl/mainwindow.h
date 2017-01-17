#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QKeyEvent>

#include "dataplotter.h"

#include <thread>
#include <deque>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);
private slots:


private:
    Ui::MainWindow *ui;
    DataPlotter* plotter_x;
    DataPlotter* plotter_y;
    DataPlotter* plotter_z;

    std::deque<std::string> buffer;
};

#endif // MAINWINDOW_H
