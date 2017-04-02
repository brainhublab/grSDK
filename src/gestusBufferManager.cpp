#include "gestusBufferManager.h"

//
// BufferManager definitions
//


// BufferManager::BufferManager()
// {
//   firstBuffer = new std::deque<std::string>();
//   secondBuffer = new std::deque<std::string>();
//
//
//   plog::init(plog::debug, "log.csv");
// }
//
// BufferManager::~BufferManager()
// {
//   delete fetchTimer;
//   delete firstBuffer;
//   delete secondBuffer;
// }

bool BufferManager::setupSource(std::deque<std::string>* buf)
{
    sourceBuffer = buf;

    // start timer for fetching data from source to separate copies
    fetchTimer = new QTimer();
    QObject::connect(fetchTimer, SIGNAL(timeout()), this, SLOT(fetchData()));
    fetchTimer->start(50);

    return true;
}

void BufferManager::fetchData()
{
    if(sourceBuffer != nullptr && !sourceBuffer->empty())
    {

        double grad2rad = 3.141592/180.0;

        double d[3] = {0, 0, 0};

        splitSensorData(sourceBuffer->front(), d);

        // bend hands
        grad2rad = 1;
        widget->leftArm.bendHand(190-d[1]*grad2rad, d[0], d[2]*grad2rad);
        widget->rightArm.bendHand(190-d[1]*grad2rad, d[0], d[2]*grad2rad);

        // move front data to different buffers
        firstBuffer->push_back(sourceBuffer->front());
        secondBuffer->push_back(sourceBuffer->front());

        sourceBuffer->pop_front();

        if(isLoggingEnabled) // TODO: move it to proper place
        {
            // LOG(plog::info) << firstBuffer->back() << i;
        }
    }
}
