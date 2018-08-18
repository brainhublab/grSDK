#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <map>
#include <vector>
#include <sstream>
#include <iterator>
#include <queue>

#include "grDevice.h"


#define DEFAULT_PATH "/tmp/grsock";
#define BUFFER_LEN 2048

enum class DataType {NOTYPE, RAW_PALM, RAW_THUMB, RAW_INDEX, RAW_MIDDLE,
                     RAW_RING, RAW_PINKY, ROTATIONS};
enum class Command {NOCMD, CMD1, STREAM_DATA, STREAM_ROTATIONS_DATA};



class GRSocket {
    public:
        GRSocket();
        GRSocket(const char *path);
        ~GRSocket();

        bool setUp();
        bool startListening();
        bool pollConnections();
        bool addDataToWrite(DataType x, std::vector<double> data);
        bool addRawData(std::string key, GRImu *imu);
        bool addRotationsData(std::unordered_map<std::string, std::vector<double>> data);

    private:
        bool isListening, isSetUp;
        int serverFd;
        struct sockaddr_un address;
        std::string socketPath;
        std::map<int, DataType> outSockets;
        std::map<int, std::queue<std::string>> toWrite;

        std::map<std::string, Command> commands;
        std::map<std::string, DataType> dataTypes;

        struct pollfd fds[20];
        int nfds, maxNfds = 20;

        void initializeCmds();
        void initializeDataTypes();
        void setUpAddress();
        void setUpFds();
        bool acceptConnection();
        bool readFromSocket(pollfd *in);
        bool writeToSocket(pollfd *out);
        bool handleCmd(pollfd *in, std::string cmd);
        template <typename T>
        void splitCmd(std::string cmd, T out);
        void error(const char *msg);
        bool addFdToPoll(int fd);
        bool removeFdFromPoll(int fd);
};
