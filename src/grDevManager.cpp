#include "grDevManager.h"
#include <sys/socket.h>
#include <gio/gio.h>
//constructor
GRDevManager::GRDevManager()
{

    _err = NULL;
    _rootProxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
            G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
            NULL,
            bluezBus.c_str(),
            "/",
            "org.freedesktop.DBus.ObjectManager",
            NULL,
            &_err);

}

//destructor
GRDevManager::~GRDevManager()
{
}

//copy constructor
GRDevManager::GRDevManager(const GRDevManager& t)
{
}

//operator =
GRDevManager& GRDevManager::operator=(const GRDevManager& t)
{
}
bool GRDevManager::_getAllManagedDevicesPaths()
{
    GVariant *res;

    res = g_dbus_proxy_call_sync(_rootProxy,
            "GetManagedObjects",
            NULL,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            NULL,
            &_err);
    if(!res)
    {
        g_dbus_error_strip_remote_error(_err);
        g_print("ERROR: getting of list of managed objects failed: %s\n", _err->message);
        g_error_free(_err);
        return false;
    }

    GVariantIter *objPathIter, *ifaceIter, *propIter;
    gchar  *objPath;

    g_variant_get(res, "(a{oa{sa{sv}}})", &objPathIter);

    while(g_variant_iter_loop(objPathIter, "{oa{sa{sv}}}", &objPath, &ifaceIter))
    {
        //g_print("%s\n", objPath);
        if(std::strstr(objPath, defaultAdapterPath.c_str()) && strlen(objPath) == 37)    
        {

            this->_allManagedDevicesPaths.push_back(objPath);
        }
    }
    return true;
}

GVariant* GRDevManager::_getProperty(std::string propName, std::string oPath, GDBusProxy *propProxy, std::string iFace)
{
    GVariant *res;
    res = g_dbus_proxy_call_sync(propProxy,
            "Get",
            g_variant_new("(ss)",
                iFace.c_str(),
                propName.c_str()),
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            NULL,
            &_err);
    if(!res)
    {
        g_dbus_error_strip_remote_error(_err);
        g_warning("ERROR: failed to get property: %s\n", _err->message);
        g_error_free(_err);
        //return ;

    }
    //g_print( "\n The NAMEEE :   %s\n", g_variant_get_data(res) );
    //  g_variant_get(variantRes, "(v)", strRes);
    //  out = strRes;
    return res;


}
bool GRDevManager::_callDevMethod(std::string methodName, GDBusProxy* methodProxy)
{
    g_dbus_proxy_call_sync(methodProxy,
            methodName.c_str(),
            NULL,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            NULL,
            &_err);
    return true; 
}

std::string GRDevManager::_getStringProp(std::string oPath, GDBusProxy* propProxy, std::string prop)
{
    const gchar *res;
    GVariant *gVar; 
    g_variant_get(_getProperty(prop, oPath, propProxy, deviceIface), "(v)", &gVar);
    g_variant_get(gVar, "s", &res);
    g_variant_unref (gVar);

    return std::string(res);
}

bool GRDevManager::_getBoolProp(std::string oPath, GDBusProxy* propProxy, std::string prop)
{
    gboolean res;
    GVariant *gVar;
    g_variant_get(_getProperty(prop, oPath, propProxy, deviceIface), "(v)", &gVar);
    g_variant_get(gVar, "b", &res);
    g_variant_unref(gVar);
    return res;

}

bool GRDevManager::_getDataset(std::string oPath, GDBusProxy* gattCharProxy,  std::string prop)
{
    std::cout<<"in getDataset"<<oPath<<"   "<<std::endl;
    unsigned char bytes[20];
    int len=0;
    guchar res;
    GVariant *gVar;
    GVariantIter *iter;
    g_variant_get(_getProperty(prop, oPath, gattCharProxy, gattCharIface), "(v)", &gVar);
    g_variant_get(gVar, "ay", &iter);
    while(g_variant_iter_loop(iter, "y", &res))
    {
        bytes[len++] = res;

    }

    std::cout<<"This is LEN-> :"<<len<<std::endl;
    //g_print("%s\n", res)
    int i=0;
    while(i<20)
    {
        if(i == 0)
        {
            std::cout<<"fuck it: "<<char( bytes[i])<<"  ";
            i++;
        }
        else
        {int16_t d = bytes[i] << 8 | bytes[i+1];
            i+=2;
            std::cout<<d<<" ";

        }
    }

    std::cout<<std::endl;
    g_variant_iter_free(iter);
    //gfree(res);
    // std::cout<<std::string(res)<<"HERE IS THE RES"<<std::endl;
    return true;
}

GDBusProxy* GRDevManager::_createProxy(std::string oPath, std::string iFace)
{
    return  g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
            G_DBUS_PROXY_FLAGS_NONE,
            NULL,
            bluezBus.c_str(),
            oPath.c_str(),
            iFace.c_str(),
            NULL,
            &_err);

}

bool GRDevManager::_connected(GRDevice *dev)
{

    return  this->_getBoolProp(dev->dbusObjecPath, dev->propProxy, "Connected");
}

bool GRDevManager::getData(GRDevice *dev)
{
    this->_getDataset((dev->dbusObjecPath+dataGattCharPath), dev->gattPropProxy, "Value"); 
}
/* Return map of avalible for connecting GR devices */
std::vector<GRDevice> GRDevManager::getAvalibleDevices()
{
    GRDevice device;
    std::string devName, devAddr;
    // GVariant *gVar; 
    GDBusProxy *propProxy;
    for(auto &path: _allManagedDevicesPaths)
    {
        propProxy = this->_createProxy(path, propIface);
        std::cout<<path<<"<-------this is path"<<std::endl;
        devName = this->_getStringProp(path, propProxy, "Name"); 
        devAddr = this->_getStringProp(path, propProxy, "Address");
        if((devName == lName || devName == rName) && !this->_deviceIsIn(devAddr))
        {
            device.name = devName;
            device.propProxy = propProxy;
            device.devMethodProxy = this->_createProxy(path, deviceIface);
            device.gattCharProxy = this->_createProxy((path+dataGattCharPath), gattCharIface);
            device.gattPropProxy = this->_createProxy((path+dataGattCharPath), propIface);
            device.dbusObjecPath = path;
            device.address = devAddr;
            device.id = this->_id++;
            //std::cout<<"IT WORKING"<<device.address<<std::endl;
            if(this->_connected(&device))
            {
                std::cout<<"IS Connected!! ->"<<device.address<<std::endl;
                device.connected = true;
            }
            this->_avalibleDevices.push_back(device);
        }
        devName.clear();
        //device.clear(); //TODO add clear method
        // g_object_unref(propProxy); TODO fix with adding link in CMakeLists for pkgconfig of all objects of glib
    }

    return this->_avalibleDevices;
}

/* Set GR device from avalible to active and make it ready for connection */
bool GRDevManager::connect(GRDevice* dev)
{
    if(!this->_connected(dev))
    {
        this->_callDevMethod("Connect", dev->devMethodProxy); 
    }
    else
    {
        std::cout<<"Device is already connected"<<std::endl;
        return false;
    }
}

bool GRDevManager::disconnect(GRDevice* dev)
{
    if(!this->_connected(dev))
    {
        std::cout<<"Device is already disconncted"<<std::endl;
        return false;
    }
    else
    {
        this->_callDevMethod("Disconnect", dev->devMethodProxy);
    }
}

bool GRDevManager::prepareDataReading(GRDevice* dev)
{
    this->_callDevMethod("StartNotify", dev->gattCharProxy); 
}
bool GRDevManager::finishDataReading(GRDevice *dev)
{
    this->_callDevMethod("StopNotify", dev->gattCharProxy);
}
//TODO needs to be implemented later
/* Return device by ID */
GRConnection* GRDevManager::getActiveDeviceById(int id)
{
    if(this->_activeDevices.find(id)->first != 0)
    {
        return &this->_activeDevices[id];
    }
    else
    {
        std::cout<<"ERROR: No such device with id: "<<id<<std::endl;
        return NULL;
    }
}

/* Check id device in avalible device */
bool GRDevManager::_deviceIsIn(std::string addr)
{
    int i=0;

    while(i < _avalibleDevices.size())
    {
        if(addr != _avalibleDevices[i].address)
        {
            i++;
        }
        else if(addr == _avalibleDevices[i].address)
        {
            return true;
        }
    }

    return false;
}


