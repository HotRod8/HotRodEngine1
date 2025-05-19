#include "FieaGameEngine/ClockService.h"

namespace Fiea::Engine
{
    using clock = std::chrono::system_clock;
    class ClockService : public ITimeProvider, public ITimeUpdater
    {
    public:
        /// @brief 
        /// @return 
        uint32_t Time() override
        {
            clock::time_point now = clock::now();
            clock::duration time_since_epoch = now.time_since_epoch();
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch);
            return static_cast<uint32_t>(seconds.count());
        }
        /// @brief 
        /// @param newTime 
        /// @return 
        uint32_t Update(uint32_t newTime = 0) override
        {
            if(newTime == 0)
            {
                uint32_t prev = _currTime;
                _currTime = Time();
                return prev;
            }
            else
            {
                _currTime = Time() + newTime;
                return _currTime;
            }
        }
    private:
        uint32_t _currTime = Time();
        FIEA_SERVICE_BIND2(ClockService, ITimeProvider, ITimeUpdater);
    };
}