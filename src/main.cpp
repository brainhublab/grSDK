/*#include <iostream>
//#include </usr/include/dbus-1.0/dbus/dbus.h>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>
#include "gestusConnection.h"
#include <thread>
#include <time.h>
#include <unistd.h>
using namespace std;
//void threadFunction(GestusConnection)
int main()
{
    string characteristic = "acc";

    deque<string> buffer;

    GestusConnection connection;
    connection.setAvalibleDevices();
    connection.getData(0, characteristic, &buffer);
    //thread thr(&GestusConnection::connectAndRead, &connection, 0, characteristic, &buffer);
    //thr.detach();
    do
    {
        if(!buffer.empty())
        {
            cout<<buffer.front()<<endl;
            buffer.pop_front();
        }
        //else
            //cout<<"is empty"<<endl;
    }while(TRUE);

      return 0;
}
*/
#include "gestusVisualization.h"
#include <QApplication>
#include <GL/glut.h>
//#include "gestusConnection.h"

int main(int argc, char *argv[])
{

	std::deque<std::string> acc;
	std::deque<std::string> gyro;
	std::deque<std::string> mag;

//	GestusConnection connection;
//	connection.setAvalibleDevices();
//	connection.getData(0, "acc", &acc);
//	connection.getData(0, "gyro", &gyro);
//	connection.getData(0, "magnet", &mag);

acc.push_back("123 120 100");
acc.push_back("134 400 23");
acc.push_back("123 20 100");
acc.push_back("134 40 23");
acc.push_back("123 10 0");
acc.push_back("134 0 3");
acc.push_back("123 1 100");
acc.push_back("134 4 2");
acc.push_back("123 1 100");
acc.push_back("134 40 23");
acc.push_back("123 0 100");
acc.push_back("34 400 23");
acc.push_back("23 120 100");
acc.push_back("134 400 23");
acc.push_back("13 120 100");
acc.push_back("1 400 23");
acc.push_back("13 120 100");
acc.push_back("34 400 23");
gyro.push_back("123 120 100");
    mag.push_back("123 120 100");


	glutInit(&argc, argv);
	QApplication a(argc, argv);
	GestusVisualization w;

	w.setupPlotters(&acc, &gyro, &mag);
	w.show();
	//w.realTimeDrowData(&buffer);

	return a.exec();
}
