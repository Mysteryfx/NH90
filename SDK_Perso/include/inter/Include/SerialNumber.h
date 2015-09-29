#ifndef _ed_SerialNumber_h_
#define _ed_SerialNumber_h_

#include "IWorldNet.h"

class RegisterStarForceSerial {
    uint64_t _handle;
public:
    RegisterStarForceSerial(const ed::string& starforce_id)
        : _handle(0)
    {
        if (globalWorldNet)
            _handle = globalWorldNet->registerSerial(starforce_id);
    }
    ~RegisterStarForceSerial()
    {
        if (globalWorldNet)
            globalWorldNet->unregisterSerial(_handle);
    }
};

#endif /* _ed_SerialNumber_h_ */
