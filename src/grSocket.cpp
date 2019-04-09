#include "grSocket.h"

GRSocket::GRSocket()
{
    this->isListening = false;
    this->isSetUp = false;
    this->socketPath = DEFAULT_PATH;
    this->setUpAddress();
    this->setUpFds();
    this->initializeCmds();
    this->initializeDataTypes();
}

GRSocket::GRSocket(const char *path)
{
    this->isListening = false;
    this->isSetUp = false;
    this->socketPath = std::string(path);
    this->setUpAddress();
    this->setUpFds();
    this->initializeCmds();
    this->initializeDataTypes();
}

GRSocket::~GRSocket()
{
    // close all connections left
    for (int i = 0; i < this->maxNfds; i++) {
        if (this->fds[i].fd != -1)
        {
            close(this->fds[i].fd);
        }
    }

    // remove UNIX path name from filesystem
    unlink(this->socketPath.c_str());
}

void GRSocket::initializeCmds()
{
    this->commands["CMD1"] = Command::CMD1;
    this->commands["STREAM_DATA"] = Command::STREAM_DATA;
}

void GRSocket::initializeDataTypes()
{
    this->dataTypes["NOTYPE"] = DataType::NOTYPE;
    this->dataTypes["palm"] = DataType::RAW_PALM;
    this->dataTypes["thumb"] = DataType::RAW_THUMB;
    this->dataTypes["index"] = DataType::RAW_INDEX;
    this->dataTypes["middle"] = DataType::RAW_MIDDLE;
    this->dataTypes["ring"] = DataType::RAW_RING;
    this->dataTypes["pinky"] = DataType::RAW_PINKY;
}

void GRSocket::setUpAddress()
{
    this->address.sun_family = AF_UNIX;
    bcopy(this->socketPath.c_str(), address.sun_path, this->socketPath.size() + 1);
}

void GRSocket::setUpFds()
{
    for(int i = 0; i < this->maxNfds; i++)
    {
        this->fds[i].fd = -1;
        this->fds[i].events = POLLIN;
        this->fds[i].revents = 0;
    }

    this->nfds = 0;
}

bool GRSocket::startListening()
{
    if (!this->isSetUp)
    {
        this->error("socket is not set up");
        return false;
    }

    if (!this->addFdToPoll(this->serverFd))
    {
        this->error("add server to fds");
        return false;
    }

    if (listen(this->serverFd, 3) < 0)
    {
        this->error("listen");
        return false;
    }

    return true;
}

void GRSocket::error(const char *msg)
{
    // log the error somewhere
    perror(msg);
    // printf("Error in GRSocket: %s\n", msg);
}

bool GRSocket::setUp()
{
    if (this->isSetUp)
    {
        return true;
    }

    // Creating socket file descriptor
    if ((this->serverFd = socket(AF_UNIX, SOCK_STREAM, 0)) == 0)
    {
        this->error("socket failed");
        return false;
    }

    // set options to reuse existitng socket address
    if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, (char *) &this->address,
                sizeof(this->address)) < 0)
    {
        this->error("setsockopt");
        return false;
    }

    int flags = fcntl(this->serverFd, F_GETFL);
    if (fcntl(this->serverFd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        this->error("fcntl");
        return false;
    }

    // attach socket to address
    if (bind(this->serverFd, (struct sockaddr *) &this->address, sizeof(this->address)) < 0)
    {
        this->error("bind failed");
        return false;
    }

    this->isSetUp = true;

    return true;
}

bool GRSocket::addFdToPoll(int fd)
{
    for(int i = 0; i < this->maxNfds; i++)
    {
        if (this->fds[i].fd == -1)
        {
            this->fds[i].fd = fd;
            this->fds[i].events = POLLIN | POLLOUT;
            this->fds[i].revents = 0;
            return true;
        }
    }

    return false;;
}

bool GRSocket::removeFdFromPoll(int fd)
{
    bool isRemoved = false;

    for (int i = 0; i < this->maxNfds; i++) {
        if (this->fds[i].fd == fd)
        {
            close(this->fds[i].fd);

            this->fds[i].fd = -1;
            this->fds[i].events = 0;
            this->fds[i].revents = 0;

            isRemoved = true;
        }
    }

    return isRemoved;
}

bool GRSocket::acceptConnection()
{
    int newSocket = -1;
    if((newSocket = accept(this->serverFd, NULL, NULL)) < 0)
    {
        this->error("accept");
        return false;
    }

    if (!this->addFdToPoll(newSocket))
    {
        this->error("add new socket to fds");
        return false;
    }

    printf("new socket: %d\n", newSocket);
    return true;
}

bool GRSocket::pollConnections()
{
    int ret = 0;
    if ((ret = poll(this->fds, this->maxNfds, 0)) < 0)
    {
        this->error("poll");
    }
    else if (ret > 0)
    {
        for (int i = 0; i < this->maxNfds; i++)
        {
            if (this->fds[i].revents & POLLIN)
            {
                if (this->fds[i].fd == this->serverFd)
                {
                    this->acceptConnection();
                    printf("acceptedConnection\n");
                }
                else
                {
                    this->readFromSocket(&(this->fds[i]));
                    printf("readFromSocket\n");
                }
            }

            if (this->fds[i].revents & POLLOUT)
            {
                if (this->fds[i].fd != this->serverFd)
                {
                    this->writeToSocket(&this->fds[i]);
                    // printf("wrote to socket\n");
                }
            }
        }
    }

    return true;
}

bool GRSocket::readFromSocket(pollfd *in)
{
    int valread = 0;
    char buffer[BUFFER_LEN];

    // check if connection is closed by the client
    if (recv(in->fd, buffer, BUFFER_LEN, MSG_PEEK | MSG_DONTWAIT) == 0)
    {
        printf("closing socket\n");

        this->removeFdFromPoll(in->fd);

        return true;
    }

    // read actual data
    if((valread = read( in->fd , buffer, BUFFER_LEN)) < 0)
    {
        this->error("read");
        return false;
    }

    if (!handleCmd(in, std::string(buffer, valread)))
    {
        this->error("handleCommand");
        return false;
    }

    return true;
}

bool GRSocket::writeToSocket(pollfd *out)
{
    if (this->toWrite[out->fd].size() > 0)
    {
        std::string msg = this->toWrite[out->fd].front();

        if (write(out->fd, msg.c_str(), msg.size()) < 0)
        {
            this->error("writing to socket");
            return false;
        }

        this->toWrite[out->fd].pop();
    }

    return true;
}

bool GRSocket::handleCmd(pollfd *in, std::string cmd)
{
    std::vector<std::string> cmdComponents;
    this->splitCmd(cmd, std::back_inserter(cmdComponents));

    if (cmdComponents.size() == 0)
    {
        this->error("empty command");
        return false;
    }

    // handle cmd here in a switch
    switch(this->commands[cmdComponents[0]])
    {
        case Command::CMD1:
            printf("CMD1 %s\n", cmdComponents[1].c_str());
            this->toWrite[in->fd].push(cmdComponents[1]);
            break;
        case Command::STREAM_DATA:
            printf("STREAM_DATA\n");
            if (cmdComponents.size() == 2)
            {
                if (this->dataTypes.find(cmdComponents[1]) == this->dataTypes.end())
                {
                    this->outSockets[in->fd] = DataType::NOTYPE;
                    std::queue<std::string>().swap(this->toWrite[in->fd]);  // empty previous data in queue
                    this->toWrite[in->fd].push(std::string("KO Wrong data type"));
                    break;
                }

                this->outSockets[in->fd] = this->dataTypes[cmdComponents[1]];  // change type
                std::queue<std::string>().swap(this->toWrite[in->fd]);  // empty previous data in queue
                this->toWrite[in->fd].push(std::string("OK"));
            }
            else
            {
                this->error("missing argument of command CMD2");
                this->toWrite[in->fd].push(std::string("KO Missing argument"));
            }
            break;
        default:
            printf("NOCMD\n");
            break;
    }

    return true;
}

    template<typename T>
void GRSocket::splitCmd(std::string cmd, T out)
{
    std::stringstream ss(cmd);
    std::string item;
    while (std::getline(ss, item, ' '))
    {
        if (!item.empty())
        {
            *(out++) = item;
        }
    }
}

    template <typename T>
std::string numToString(T num)
{
    std::ostringstream ss;
    ss << num;
    return ss.str();
}

bool GRSocket::addDataToWrite(DataType type, std::vector<double> data)
{
    std::string sData;
    int socket = 0;

    sData += numToString(int(type));
    sData += ' ';

    // if NOTYPE return right away
    if (type == DataType::NOTYPE)
    {
        return false;
    }


    // find which socket expects data of type `type`
    for (const auto& x : this->outSockets)
    {
        if (x.second == type)
        {
            socket = x.first;
            break;
        }
    }

    // if no socket expects the data return
    if (socket == 0)
    {
        return false;
    }

    // represent the data as a string
    for (unsigned int i = 0; i < data.size(); i++)
    {
        std::ostringstream ss;
        ss << data[i];
        sData += ss.str();
        sData += ',';
    }

    sData.pop_back();  // clean trailing comma

    // add data to socket's writing queue
    this->toWrite[socket].push(sData);

    return true;
}

bool GRSocket::addRawData(std::string key, GRImu *imu)
{
    DataType type = this->dataTypes[key];
    int socket = 0;
    std::string sData;

    // if NOTYPE return right away
    if (type == DataType::NOTYPE)
    {
        return false;
    }

    // find which socket expects data of type `type`
    for (const auto& x : this->outSockets)
    {
        if (x.second == type)
        {
            socket = x.first;
            break;
        }
    }

    // if no socket expects the data return
    if (socket == 0)
    {
        return false;
    }

    sData += key;
    sData += ' ';
 //   std::cout<<imu->acc[0]<<"  PRINT IN SOCK"<<std::endl;
    for (auto& x: imu->acc) {
        sData += numToString(x);
        sData += ',';
    }
    sData.pop_back();
    sData += ' ';
    for (auto& x: imu->gyro) {
        sData += numToString(x);
        sData += ',';
    }

    sData.pop_back();
    sData += ' ';

    for (auto& x: imu->mag) {
        sData += numToString(x);
        sData += ',';
    }

    sData.pop_back();

    // add data to socket's writing queue
    this->toWrite[socket].push(sData);
   // std::cout<<sData<<"SDATA===================="<<std::endl;
    return true;
}
