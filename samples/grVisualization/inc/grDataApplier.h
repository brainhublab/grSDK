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

    bool addHistoryData(std::vector<double>);
    bool writeQuanternionHistory(std::deque<std::vector<float>>*);
	bool isLoggingEnabled = false; // bool for logging to a file

    const std::map<int, device_t>& getActiveDevices() const;


public slots:
	bool fetchSignal();
//	signals:
//	void fetchSignal();

private:
    bool processMsg(std::string);
    bool fetchData(); // gets data from algdev and applies it for hand
	QTimer *fetchTimer; // a timer for fetchdata
    GRHand* arm; // for moving arm


    GRConnection conn;
    device_t* device;
    gr_message msg;
    gr_alg_message alg_msg;
    std::map<int, device_t> activeDevices;


	int fetchFrequency = 20;
	// algorithms for each finger and hand
    std::unordered_map<std::string, GRAlgorithm> algorithms;
    bool setAlgorithms();

    GRAlgorithm algPinky;
    GRAlgorithm algRing;
    GRAlgorithm algMiddle;
    GRAlgorithm algIndex;
    GRAlgorithm algThumb;
    GRAlgorithm algPalm;

	// for applying data
    bool applyToFinger(std::vector<double>&, int);
    bool applyToHand(std::vector<double>&);
    std::vector<float>* nodeQuanternion;
    std::vector<float> prevQuants[6];
    bool fetchRunning = false;

    std::deque<std::vector<float>>* targetQuanternionHistory = nullptr;
};

#endif // GR_BUFFER_MANAGER_H
