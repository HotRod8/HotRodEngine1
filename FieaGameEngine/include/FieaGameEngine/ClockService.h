#pragma once
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ServiceMgr.h"
#include <chrono>

namespace Fiea::Engine
{
    struct ITimeProvider
    {
        FIEA_SERVICE_INTERFACE;
        virtual uint32_t Time() = 0; 
        // in seconds, since "epoch"
    };

    struct ITimeUpdater
    {
        FIEA_SERVICE_INTERFACE;
        virtual uint32_t Update(uint32_t newTime = 0) = 0; 
        // if newTime is zero, just use the system clock, return previous time
    };
}