#include "iot.h"
#include "string"

struct WifiInfo : public iot::Message
{
    //data fields
    std::string ssid;
    std::string password;

    static const unsigned int id = 1;
    unsigned int getId() { return WifiInfo::id; }
    WifiInfo(std::string ssid, std::string password) : ssid(ssid), password(password) {}
};