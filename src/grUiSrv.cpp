#include "grUiSrv.h"

GRUiSrv::GRUiSrv()
{
    memset(this->_ans, 0, sizeof(this->_ans));
    this->_serverFd = 0;
    this->_clientFd = 0;
    this->_bytesRecieved = 0;
    this->_ret = 0;
    this->_defaultSockPath = DEFAULT_PATH;

    this->_clients["pinky"] = nullptr;
    this->_clients["ring"] = nullptr;
    this->_clients["middle"]= nullptr;
    this->_clients["index"] = nullptr;
    this->_clients["thumb"] = nullptr;
    this->_clients["palm"] = nullptr;
    this->_clients["rotations"] = nullptr;
}

GRUiSrv::GRUiSrv(const std::string sockPath)
{
    memset(this->_ans, 0, sizeof(this->_ans));
    this->_defaultSockPath = sockPath;
}

GRUiSrv::~GRUiSrv()
{

}

bool GRUiSrv::run()
{
    std::thread rn(&GRUiSrv::_runInThread, this);
    rn.detach();
}
bool GRUiSrv::_runInThread()
{
    try{
        libsocket::unix_stream_server srv(this->_defaultSockPath);

        std::cout<<"beforeWHile"<<std::endl;
        std::string res;
        res.resize(128);
        //        srv.destroy();
        std::cout<<"THE FDD"<<srv.getfd()<<std::endl;
        //   while(true)
        // {
        // cli = srv.accept2();
        while(srv.getfd() > 0)
        {
            //            std::string asd;
            //            asd.resize(128);
            try{
                std::unique_ptr<libsocket::unix_stream_client> tmpCli;
                tmpCli = srv.accept2();
                *tmpCli>>res;
                std::cout<<res<<std::endl;
                std::vector<std::string> cmd= _splitBySpace(&res);
              //  std::cout<<cmd[1]<<" "<<cmd[0]<<std::endl;
                if(cmd.size() == 2)
                {
                    if(!_clients.count(cmd[1]))
                    {

                        res.resize(5);
                        res = "KO";
                        *tmpCli<<res;
                        std::cout<<"there is no sensor with name: "<<cmd[1]<<std::endl;
                    }
                    else
                    {
                        res.resize(5);
                        res = "OK";
                        *tmpCli<<res;
                        _clients[cmd[1]] = std::move(tmpCli);
                    }
                }
                // if()
                res.resize(128);

            }catch (const libsocket::socket_exception& inexc)
            {
                std::cerr<<inexc.mesg;
                return false;
            }
        }

        std::cout<<"afterWhile"<<std::endl;
    }catch (const libsocket::socket_exception& exc)
    {
        std::cerr <<exc.mesg;
        return false;
    }
    return true;
    }
    void GRUiSrv::writeData(GRMessage* msg)
    {
        std::cout<<"IN CALLBACK UI"<<std::endl;
        // if(key=="palm")
        // {
        std::string asd = "palm 123,1234,12356 432,53424,3434 8484,233,3435";
        // *_cli<<asd;
        for(auto& client : this->_clients)
        {
            if(client.second != nullptr)
            {
                *client.second<<client.first + " " + msg->imus[client.first]->getAsStr();
            }
        }
        /*
        if(this->_clients["palm"] != nullptr)
        {
            *this->_clients["palm"]<<"palm " + msg->imus["palm"]->getAsStr();//imuByteData;
        }
        */
        //  }
    }
    std::vector<std::string> GRUiSrv::_splitBySpace(std::string* inp)
    {
        //TODO need to fix vector size its put the terminating zero as an element of vector
        std::regex reg("\\s+");
        return std::vector<std::string>{
            std::regex_token_iterator<std::string::iterator>(inp->begin(),
                    inp->end(),
                    reg,
                    -1),{}
        };
    }

