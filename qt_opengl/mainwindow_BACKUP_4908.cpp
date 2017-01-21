#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    buffer.push_back("123 23 94");
    buffer.push_back("154 54 124");
    buffer.push_back("173 74 144");
    buffer.push_back("193 93 194");
    buffer.push_back("123 23 34");
    buffer.push_back("3 0 144");
    buffer.push_back("173 74 144");
    buffer.push_back("154 54 124");
    buffer.push_back("123 23 34");
    buffer.push_back("193 93 194");
    buffer.push_back("123 23 94");

    std::deque<std::string> buffer1;
    buffer1.push_back("3 0 144");

    buffer1.push_back("173 74 144");
    buffer1.push_back("154 54 124");
    buffer1.push_back("123 23 34");
    buffer1.push_back("193 93 194");
    buffer1.push_back("123 23 94");
    buffer1.push_back("193 3 87");
    buffer1.push_back("13 200 17");
    buffer1.push_back("123 23 94");


    std::deque<std::string> buffer2;
    buffer2.push_back("0 9 14");
    buffer2.push_back("3 7 4");
    buffer2.push_back("15 54 24");
    buffer2.push_back("112 24 342");
    buffer2.push_back("193 3 87");
    buffer2.push_back("13 200 17");
    buffer2.push_back("123 23 94");
    buffer2.push_back("154 54 124");
    buffer2.push_back("173 74 144");
    buffer2.push_back("193 93 194");
    buffer2.push_back("123 23 34");


    plotter_x = new DataPlotter(ui->PlotData_X);
    //plotter_x->drawPlotByAxis(&buffer, 0, "buffer 1");
    //plotter_x->drawPlotByAxis(&buffer1, 0, "buffer 2");
    //plotter_x->drawPlotByAxis(&buffer2, 0, "buffer 3");
    //plotter_x->showPlot();

    plotter_y = new DataPlotter(ui->PlotData_Y);
    //plotter_y->drawPlotByAxis(&buffer, 1, "buffer1");
    //plotter_y->drawPlotByAxis(&buffer1, 1, "buffer2");
    //plotter_y->drawPlotByAxis(&buffer2, 1, "buffer3");
    //plotter_y->showPlot();

    plotter_z = new DataPlotter(ui->PlotData_Z);
    //plotter_z->drawPlotByAxis(&buffer, 2, "buffer1");
    //plotter_z->drawPlotByAxis(&buffer1, 2, "buffer 2");
    //plotter_z->drawPlotByAxis(&buffer2, 2, "buffer3");
    //plotter_z->showPlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // move it to GVLwidget
    if(event->key() == Qt::Key_7)
    {
        this->ui->GLwidget->getArm()->bendArm( 5.f, 0.f, 0.f );
        buffer.clear();
        plotter_x->drawPlotByAxis(&buffer, 0, "buffer 1e");
        plotter_x->showPlot();
    }
    if(event->key() == Qt::Key_8)
    {
        this->ui->GLwidget->getArm()->bendArm( 0.f, 5.f, 0.f );

        buffer.push_back("23 24 45");
    }
    if(event->key() == Qt::Key_9)
    {
        this->ui->GLwidget->getArm()->bendArm( 0.f, 0.f, 5.f );
    }

}

<<<<<<< HEAD

=======
>>>>>>> 42c377f7ad8a7228823820673707792e0599cb88


