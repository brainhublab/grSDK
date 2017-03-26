#ifndef GESTUS_VISUALISATION_H
#define GESTUS_VISUALISATION_H


#include <QMainWindow>
#include <QKeyEvent>
#include <QObject>

#include <thread>
#include <deque>

#include "gestusDataPlotter.h"
#include "gestusGLWidget.h"
#include <iostream>
#include "externAssets/plog/Log.h" //Lib for logging in csv format
#include <fstream>
// serial
//
// ===
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

class SerialData
{
    public:
        SerialData()
        {
            setup();
        };

        ~SerialData()
        {
            close(fileDescriptor);
        };

        bool setup(std::string src = "/dev/rfcomm0")
        {
            fileDescriptor = openPort(src);
            setTerm();
            return fileDescriptor != -1;
        };
        void setTerm()
        {
          // fcntl(fileDescriptor, F_SETFL, 0);
            struct termios toptions;

            // baud
            
             cfsetispeed(&toptions, B115200);
              cfsetospeed(&toptions, B115200);
               /* 8 bits, no parity, no stop bits */
              toptions.c_cflag &= ~PARENB;
 //               toptions.c_cflag &= ~CSTOPB;
              toptions.c_cflag &= ~CSIZE;
    //             toptions.c_cflag |= CS8;
                   /* no hardware flow control */
       //            toptions.c_cflag &= ~CRTSCTS;
                    /* enable receiver, ignore status lines */
         //           toptions.c_cflag |= CREAD | CLOCAL;
                     /* disable input/output flow control, disable restart chars */
           //          toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
                      /* disable canonical input, disable echo,
                       *  disable visually erase chars,
                       *   disable terminal-generated signals */
             //         toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
                       /* disable output processing */
               //        toptions.c_oflag &= ~OPOST;
                        


/* wait for 12 characters to come in before read returns */
/* WARNING! THIS CAUSES THE read() TO BLOCK UNTIL ALL */
/* CHARACTERS HAVE COME IN! */
//toptions.c_cc[VMIN] = 4*3+2;
 /* no minimum time to wait before read returns */
                toptions.c_cc[VTIME] = 200;
            toptions.c_cc[VSTOP] = '\n';
/* commit the options */
             tcsetattr(fileDescriptor, TCSANOW, &toptions);
 // usleep(1000*1000);
          tcflush(fileDescriptor, TCIFLUSH);
        }



        std::string getNext()
        { 
  //int n = 1;
            char buf[2];
             std::string result;
             std::cout << "\nReading from device...";
             while(true)
             {

                read(fileDescriptor, buf, 1);
    //std::cout << buf[0];
                if(buf[0] != '\n')
                {
                     result += buf[0];
                }
                else
                {
                    std::cout << "\nGot data!";
                    return result;
                }
 }
    /* print what's in th:e buffer */
   // printf("Buffer contains: >%s<\n", result.c_str());
 }

    
    private:
        char *portname="/dev/rfcomm0";
        char buf[256];
        int fileDescriptor;

    int openPort(std::string path = "/dev/rfcomm0")
    {
       int fd; // file descriptor

       fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
       if (fd == -1)
       {
            printf("Error opening %s!\n", path.c_str());
            perror("open_port: Unable to open /dev/rfcomm0 - ");
       }
       else
       {
           fcntl(fd, F_SETFL, 0);
       }

       return (fd);
    };


};


namespace Ui{
class GestusVisualization;
}
// TODO:: DataPlotter -> DataManager(in fetchData() to bend hand)

    struct BufferManager : public QObject
    {
      Q_OBJECT
    public:
        BufferManager();
        ~BufferManager();
        bool setGLWidget(GestusGLWidget* w)
        {
            widget = w;
            return true;
        };

        void splitSensorData(std::string str, double arr[3])
        {
                int i = 0;
                double n;
                std::stringstream ss(str);
                while(ss >> n)
                {
        			arr[i] = n;
                        i++;
                }
        };
        std::ifstream ifs;
        //
        bool setupSource(std::deque<std::string>* buf);
        QTimer* fetchTimer;

        std::deque<std::string>* sourceBuffer = nullptr;

        std::deque<std::string>* firstBuffer;
        std::deque<std::string>* secondBuffer;


            bool isLoggingEnabled = false;
    public slots:
        void fetchData();
    private:
          SerialData serialD;
          GestusGLWidget* widget;
    };

class GestusVisualization : public QMainWindow
{
    Q_OBJECT

public:
	explicit GestusVisualization(QWidget *parent = 0);
	~GestusVisualization();
    bool setupPlotters(std::deque<std::string> *, std::deque<std::string> *, std::deque<std::string> *);

private slots:
    void on_trajectoryCheckBox_toggled(bool checked);
    void on_leftHandCheckBox_toggled(bool checked);
    void on_rightHandCheckBox_toggled(bool checked);
    void on_hackerModeCheckBox_toggled(bool checked);
    void on_loggingCheckBox_toggled(bool checked);
    void on_randomData_clicked();
    void on_pausePlotCheckBox_toggled(bool checked);

private:
    bool initUiProps();
  	Ui::GestusVisualization *ui;

    BufferManager acc;
    BufferManager gyro;
    BufferManager mag;

    DataPlotter* plotter_acc;
    DataPlotter* plotter_gyro;
    DataPlotter* plotter_mag;
    DataPlotter* plotter_all_acc;
    DataPlotter* plotter_all_gyro;
    DataPlotter* plotter_all_mag;
};

#endif // MAINWINDOW_H
