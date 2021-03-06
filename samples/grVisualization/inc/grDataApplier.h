#ifndef GR_BUFFER_MANAGER_H
#define GR_BUFFER_MANAGER_H

#include <QObject>
#include <QTimer>

#include <iostream>
#include <deque>
#include <string>
#include "plog/Log.h" //Lib for logging in csv format

#include "GRGLWidget.h"
#include "grConnection.h"
#include "grAlgorithm.h"

class GRDataApplier : public QObject
{
	Q_OBJECT
public:
	GRDataApplier();
	~GRDataApplier();

    bool setArm(GRHand *arm); // sets OpenGL model of arm for data applying
    bool assignDeviceName(std::string);
    bool setDeviceId(int);
    bool setConnection(GRConnection*);
    bool run(); // connect device and run fetchdata every {fetchFrequency)

    bool writeQuanternionHistory(std::deque<std::vector<float>>*); // start writing history to vector pointer
    bool addHistoryData(std::vector<double>); // adds new quaternion to history

    bool isLoggingEnabled = false; // for logging to a file
    std::string deviceName = "";

public slots:
    bool fetchSignal(); // this signal is called every 20 ms
private:
    int deviceId = -1;
    bool fetchRunning = false; // mutex analog
    int fetchFrequency = 20; // ms
    QTimer *fetchTimer; // a timer for fetchdata

    GRHand *arm; // OpenGL model pointer

    GRConnection* conn;
    device_t* device;
    gr_message msg; // raw data
    GRAlgorithm alg; // madgwick algorithm for alg_msg producing
    gr_alg_message alg_msg; // madgwick updated data

    bool processMsg(std::string); // gets data from msg variable, updates it with madgwick and writes to alg_msg
    bool fetchData(); // gets data from algdev, writes it to msg variable and apply msg for each arm node

    bool applyToFinger(std::vector<double>&, int); // apply quaternion to [int] finger
    bool applyToHand(std::vector<double>&); // apply quaternion to hand
    std::vector<float>* nodeQuanternion; // current temporary quaternion
    std::vector<float> prevQuants[6]; // previous quaternions of msg

    std::unordered_map<std::string, int> fingers; // nodeName-indexe
    std::deque<std::vector<float>>* targetQuanternionHistory = nullptr; // buffer pointer for writing a history of quaternions
};

#endif // GR_BUFFER_MANAGER_H
