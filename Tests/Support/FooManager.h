#pragma once
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ServiceMgr.h"

namespace Fiea::Engine
{
    struct IFooMaker
    {
        FIEA_SERVICE_INTERFACE;        
        virtual void Make(const char* name, int32_t value) = 0;
    };

    struct IFooTaker
    {
        FIEA_SERVICE_INTERFACE;
        virtual int32_t Take(const char* name) = 0;
    };
}