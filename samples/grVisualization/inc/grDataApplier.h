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

	bool setArm(GRHand *arm);
	bool run();

	QTimer *fetchTimer;

	bool isLoggingEnabled = false; // bool for logging to a file
public slots:
	void fetchData(); // gets data from algdev and applies it for hand

private:

	GRHand* arm; // for moving arm
	device_t dev;
	alg_device_t algDev; // filtered data
	GRConnection conn;

	int fetchFrequency = 20;
	// algorithms for each finger and hand
	GRAlgorithm alg;
	GRAlgorithm alg1;
	GRAlgorithm alg2;
	GRAlgorithm alg3;
	GRAlgorithm alg4;
	GRAlgorithm alg5;
};

#endif // GR_BUFFER_MANAGER_H
