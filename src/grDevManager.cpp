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

            _allManagedDevicesPaths.push_back(objPath);
        }
    }
    return true;
}

GVariant* GRDevManager::_getProperty(std::string propName, std::string oPath, GDBusProxy *propProxy)
{
    GVariant *res;
    res = g_dbus_proxy_call_sync(propProxy,
            "Get",
            g_variant_new("(ss)",
                deviceIface.c_str(),
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
    g_print( "\n The NAMEEE :   %s\n", g_variant_get_data(res) );
    //  g_variant_get(variantRes, "(v)", strRes);
    //  out = strRes;
    return res;


}

std::string GRDevManager::_getStringProp(std::string oPath, GDBusProxy* propProxy, std::string prop)
{
    const gchar *res;
    GVariant* gVar; 
    g_variant_get(_getProperty(prop, oPath, propProxy), "(v)", &gVar);
    g_variant_get(gVar, "s", &res);
    g_variant_unref (gVar);

    return std::string(res);
}

GDBusProxy* GRDevManager::_createPropProxy(std::string oPath)
{
   return  g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
            G_DBUS_PROXY_FLAGS_NONE,
            NULL,
            bluezBus.c_str(),
            oPath.c_str(),
            propIface.c_str(),
            NULL,
            &_err);

}
/* Return map of avalible for connecting GR devices */
std::vector<GRDevice> GRDevManager::getAvalibleDevices()
{
    GRDevice device;
    std::string result;
   // GVariant *gVar; 
    GDBusProxy *propProxy;
    for(auto &path: _allManagedDevicesPaths)
    {
        propProxy = _createPropProxy(path);
        result = _getStringProp(path, propProxy, "Name"); 
        if(result == lName || result == rName)
        {
            device.propProxy = propProxy;
            device.dbusObjecPath = path;
            device.name = result;
            device.address = _getStringProp(path, device.propProxy, "Address");
            std::cout<<"IT WORKING"<<device.address<<std::endl;
        }
        result.clear();
       // g_object_unref(propProxy); TODO fix with adding link in CMakeLists for pkgconfig of all objects of glib
    }

    return _avalibleDevices;
}

/* Set GR device from avalible to active and make it ready for connection */
GRConnection* GRDevManager::setActiveDevice(int devId)
{
    std::unordered_map<int, GRConnection>::iterator it = this->_activeDevices.begin();
    while(it != this->_activeDevices.end())
    {
        if(devId == it->first)
        {
            std::cout<<"ERROR: could not ad device with id: "<<devId<<" because is exists"<<std::endl;
            return &it->second;
        }
        it++;
    }

    this->_activeDevices[devId] = GRConnection(this->_avalibleDevices[devId]);
    std::cout<<"Set device with address in active: "<<_avalibleDevices[devId].get_address()<<std::endl;
    return &this->_activeDevices[devId];
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
    int i=1;

    while(i <= _avalibleDevices.size())
    {
        if(addr != _avalibleDevices[i].get_address())
        {
            i++;
        }
        else if(addr == _avalibleDevices[i].get_address())
        {
            return true;
        }
    }

    return false;
}


