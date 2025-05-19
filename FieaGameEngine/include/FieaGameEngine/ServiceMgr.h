#pragma once
#include "FieaGameEngine/Types.h"
#include <unordered_map>
#include <stdexcept>


using InterfaceId = size_t;
#define FIEA_SERVICE_INTERFACE                                    \
        static inline InterfaceId GetInterfaceId()                \
        {                                                         \
            static char _unused{};                                \
            return (InterfaceId)&_unused;                         \
        }

#define FIEA_SERVICE_BIND1(_impl, _intf)                                                                                    \
        static bool _Register()                                                                                             \
        {                                                                                                                   \
            ServiceMgr* mgr = ServiceMgr::Instance();                                                                       \
            auto create = []() -> void* { return static_cast<void*>(new _impl()); };                                        \
            auto destroy = [](void* service) -> void { delete static_cast<_impl*>(service); };                              \
            auto provide = [](void* ptr) -> void* { return static_cast<_intf*>(reinterpret_cast<_impl*>(ptr)); };           \
            mgr->RegisterInterfaceProvider<_intf>(create, destroy, provide);                                                \
            return true;                                                                                                    \
        }                                                                                                                   \
        static inline bool _registered = _Register();


#define FIEA_SERVICE_BIND2(_impl, _intf1, _intf2)                                                                           \
        static bool _Register()                                                                                             \
        {                                                                                                                   \
            ServiceMgr* mgr = ServiceMgr::Instance();                                                                       \
            auto create = []() -> void* { return static_cast<void*>(new _impl()); };                                        \
            auto destroy = [](void* service) -> void { delete static_cast<_impl*>(service); };                              \
            auto provideM = [](void* ptr) -> void* { return static_cast<_intf1*>(reinterpret_cast<_impl*>(ptr)); };         \
            auto provideT = [](void* ptr) -> void* { return static_cast<_intf2*>(reinterpret_cast<_impl*>(ptr)); };         \
            mgr->RegisterInterfaceProvider<_intf1>(create, destroy, provideM);                                              \
            mgr->RegisterInterfaceProvider<_intf2>(create, destroy, provideT);                                              \
            return true;                                                                                                    \
        }                                                                                                                   \
        static inline bool _registered = _Register();

#define FIEA_SERVICE_BIND4(_impl, _intf1, _intf2, _intf3, _intf4)                                                           \
        static bool _Register()                                                                                             \
        {                                                                                                                   \
            ServiceMgr* mgr = ServiceMgr::Instance();                                                                       \
            auto create = []() -> void* { return static_cast<void*>(new _impl()); };                                        \
            auto destroy = [](void* service) -> void { delete static_cast<_impl*>(service); };                              \
            auto provideM = [](void* ptr) -> void* { return static_cast<_intf1*>(reinterpret_cast<_impl*>(ptr)); };         \
            auto provideT = [](void* ptr) -> void* { return static_cast<_intf2*>(reinterpret_cast<_impl*>(ptr)); };         \
            mgr->RegisterInterfaceProvider<_intf1>(create, destroy, provideM);                                              \
            mgr->RegisterInterfaceProvider<_intf2>(create, destroy, provideT);                                              \
            return true;                                                                                                    \
        }                                                                                                                   \
        static inline bool _registered = _Register();


namespace Fiea::Engine
{
    //typedef void* (*CreateFunc)(void); //takes no parameters, returns a void*
    //typedef void (*DestroyFunc)(void*); //take a void* parameter, returns nothing
    //typedef void* (*ProvideFunc)(void*); //take a void* and return a void* -WHAT ?

    using CreateFunc = void* (*) (void);
    using DestroyFunc = void (*) (void*);
    using ProvideFunc = void* (*) (void*);

    struct ServiceProvider {
        CreateFunc Create;
        DestroyFunc Destroy;
        ProvideFunc Provide;
    };

    struct Implementors
    {
        void* Data;
        DestroyFunc Destroy;
    };

    using umap = std::unordered_map<size_t, ServiceProvider>;
    using existMap = std::unordered_map<CreateFunc, Implementors>;

    class ServiceMgr : public Singleton<ServiceMgr>
    {
    private:
        // a mapping from InterfaceId to ServiceProvider - what container type to use? (My Ans: mapping + Id = map<K, V>)
        umap _idMap;
        // storage for created implementations of managers (Ex: FooManager/MemoryService)... what container type to use?
        existMap _ptrMap;

        const ServiceProvider* GetProvider(InterfaceId id);
        static void* GetImplementation(const ServiceProvider& provider);

    public:
        // we'll use this as a way to uniquely refer to a particular interface 

        /// @brief - Creates a new map variable
        /// @tparam I - Our Interface typename
        /// @param create - The create function pointer passed in to here
        /// @param destroy - The destroy function pointer passed in to here
        /// @param provide - The provide function pointer passed in to here
        template <typename I>
        static void RegisterInterfaceProvider(CreateFunc create, DestroyFunc destroy, ProvideFunc provide)
        {
            // Typename I will allow us to call the Interface's ID macro and use that for the map ID
            // I have lazy initallization, so this will handle creation if the ServiceMgr doesn't exist
            
            ServiceProvider pvdr = ServiceProvider
            {
                create,
                destroy,
                provide
            };
            umap& temp1 = Instance()->_idMap;
            InterfaceId id = I::GetInterfaceId();

            temp1.emplace(id, pvdr);
        };

        /// @brief - Creates an implementation, if necessary; otherwise re-uses a previously created implementation
        /// @tparam I - Interface typename
        /// @return - Returns an Interface
        template<typename I>
        static I* ProvideInterface()
        {
            umap temp1 = Instance()->_idMap;
            existMap temp2 = Instance()->_ptrMap;

            InterfaceId id = I::GetInterfaceId();
            auto it = temp1.find(id);

            if (it != temp1.end())
            {
                auto itr = temp1.find(id);
                if (temp2.find(itr->second.Create) != temp2.end())
                {
                    auto res = temp2.find(itr->second.Create);
                    return static_cast<I*>(itr->second.Provide(res->second.Data));
                }

                void* res = GetImplementation(itr->second);
                return static_cast<I*>(itr->second.Provide(res));
            }
            throw std::out_of_range("Interface not found");
        };

        void Reset();
    };
}