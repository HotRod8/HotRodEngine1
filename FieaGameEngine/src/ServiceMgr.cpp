#include "FieaGameEngine/MemoryService.h"
#include "FieaGameEngine/ServiceMgr.h"

namespace Fiea::Engine
{
    using InterfaceId = size_t;
    /// @brief - Gets the Provider for the passed ID if it exists
    /// @param id - key
    /// @return - Returns the ServiceProvider from the ID
    const ServiceProvider* ServiceMgr::GetProvider(InterfaceId id)
    {
        auto itr = _idMap.find(id);
        if(itr != _idMap.end())
        {
            return &(itr->second);
        }
        throw std::out_of_range("Provider not found");
    }
    /// @brief - Returns a manager implementation as a void*
    /// @param provider - ServiceProvider param
    /// @return - void* for manager implementation
    void* ServiceMgr::GetImplementation(const ServiceProvider& provider)
    {
        existMap& temp2 = Instance()->_ptrMap;

        if(temp2.find(provider.Create) == temp2.end())
        {
            //Create new implementation ptr 
            void* res = provider.Create();
            Implementors imp = Implementors
            {
                res,
                provider.Destroy
            };
            temp2.emplace(provider.Create, imp);
        }
        auto mapItr = temp2.find(provider.Create);
        return mapItr->second.Data;
    }

    /// @brief - Resets all implementation data to 0. Provider data is dealt w/ automatically
    void ServiceMgr::Reset()
    {
        ServiceMgr* mgr = Instance();
        existMap clearMap;

        for (existMap::iterator idItr = mgr->_ptrMap.begin(); idItr != mgr->_ptrMap.end(); ++idItr)
        {
            idItr->second.Destroy(idItr->second.Data);
        }
        clearMap.swap(mgr->_ptrMap);
    }
}