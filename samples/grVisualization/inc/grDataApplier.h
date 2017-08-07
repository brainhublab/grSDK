#ifndef GR_BUFFER_MANAGER_H
#define GR_BUFFER_MANAGER_H

#include <QObject>
#include <QTimer>

#include <deque>
#include <string>

#include "plog/Log.h" //Lib for logging in csv format

#include "GRGLWidget.h"

#include <iostream>

#include "grConnection.h"
#include "grAlgorithm.h"

class GRDataApplier : public QObject
{
	Q_OBJECT
public:
	GRDataApplier();

	~GRDataApplier();

	bool setArm(GRHand *arm); // set the arm for applying data
	bool run(); // runs fetchdata every {fetchFrequency)

	bool isLoggingEnabled = false; // bool for logging to a file
public slots:
	bool fetchSignal();
//	signals:
//	void fetchSignal();

private:

	bool fetchData(); // gets data from algdev and applies it for hand


	QTimer *fetchTimer; // a timer for fetchdata


    GRHand* arm; // for moving arm


    GRConnection conn;
    device_t* device;
    gr_message msg;
    gr_alg_message alg_msg;
    std::map<int, device_t> devices;


	int fetchFrequency = 20;
	// algorithms for each finger and hand
	GRAlgorithm alg;
	GRAlgorithm alg1;
	GRAlgorithm alg2;
	GRAlgorithm alg3;
	GRAlgorithm alg4;
	GRAlgorithm alg5;

	// for applying data
	bool applyToFinger(std::deque<std::vector<float>>&, int);
    bool applyToHand(std::vector<double>&);
    std::vector<float>* nodeQuanternion;
    std::vector<float> prevQuants[6];
	bool fetchRunning = false;
};

#endif // GR_BUFFER_MANAGER_H
