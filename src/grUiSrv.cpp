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

    this->_dataType["pinky"] = 0;// DataType::PINKY;
    this->_dataType["ring"] = 1;//DataType::RING;
    this->_dataType["middle"] = 2;//DataType::MIDDLE;
    this->_dataType["index"] = 3;//DataType::INDEX;
    this->_dataType["thumb"] = 4;//DataType::THUMB;
    this->_dataType["palm"] = 5;//DataType::PALM;

}

GRUiSrv::GRUiSrv(const std::string sockPath)
{
    memset(this->_ans, 0, sizeof(this->_ans));
    this->_defaultSockPath = sockPath;
}

GRUiSrv::~GRUiSrv()
{
    for(auto& client: _clients)
    {
        if(client.second != nullptr) 
        {
            client.second->destroy();
        }
    }
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
            try{
                std::unique_ptr<libsocket::unix_stream_client> tmpCli;
                tmpCli = srv.accept2();
                *tmpCli>>res;
                //std::cout<<res<<std::endl;
                std::vector<std::string> cmd= _splitBySpace(&res);
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
                        res.resize(6);
                       res = "OK";
                        std::cout<<"ON NEW CLIENT"<<cmd[1]<<std::endl;
                        *tmpCli<<res;
               //         _clients[cmd[1]] = nullptr;
                        _clients[cmd[1]] = std::move(tmpCli);
                    }
                }
                res.resize(128);
            }
            catch (const libsocket::socket_exception& inexc)
            {
                std::cerr<<inexc.mesg;
                return false;
            }

            std::cout<<"iteration --------"<<std::endl;
        }

        std::cout<<"afterWhile"<<std::endl;
         if(srv.getfd() > 0)
           {
           srv.destroy();
           }
    }
    catch (const libsocket::socket_exception& exc)
    {
        return false;
    }
    return true;
    }
    void GRUiSrv::writeData(GRMessage* msg)
    {
        std::cout<<"IN CALLBACK UI"<<std::endl;
        for(auto& client : this->_clients)
        {
            if(client.second != nullptr && client.second->getfd()>0)
            {
                try{
                    int asd = (_dataType[client.first]);
                    std::string asdasd  = std::to_string(asd);//= reinterpret_cast<std::string>(asd);
                     *client.second<<std::to_string(_dataType[client.first]) + 
                         msg->imus[client.first]->imuByteData;//getAsStr();

                }catch(const libsocket::socket_exception& exc)
                {
                    std::cerr<<exc.mesg;
                }
            }
        }
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

