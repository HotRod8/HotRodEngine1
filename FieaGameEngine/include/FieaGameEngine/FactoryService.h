#pragma once
#include "FieaGameEngine/Types.h"
#include <memory>
#include <map>
#include "../RTTI.h"

namespace Fiea::Engine
{
    struct IFactoryBase : RTTI
    {
        virtual string GetProductName() = 0;              // the string name for this factory
        RTTI_DECLARATIONS(IFactoryBase, RTTI);
    };
    // Factories of any type will all have an associated string
    template <typename BaseType> 
    struct IAbstractFactory : IFactoryBase
    {
        [[nodiscard]] virtual BaseType* Create() = 0;     // a create function for the abstract factory that produces a default-constructed sub-class
        RTTI_DECLARATIONS(IAbstractFactory, IFactoryBase);
    };
    // All subclasses of a particular BaseType have a unique string and creation method, completing the abstract factory pattern

    template <typename T>
    RTTI_DEFINITIONS(IAbstractFactory<T>);


    class IFactoryService
    {
        //using UniqPtr = std::unique_ptr<RTTI>;

    public:
        // thin wrapper for RegisterFactory
        template <typename BaseType> void Register(std::unique_ptr<IAbstractFactory<BaseType>>&& concreteFactory)
        {
            RegisterFactory(BaseType::TypeIdClass(), std::move(concreteFactory));
        }
        // thin wrapper for GetFactory, which calls Create if a factory is found
        template <typename BaseType> BaseType* CreateProduct(const string& productName)
        {
            IFactoryBase* ptr = GetFactory(BaseType::TypeIdClass(), productName);
            if(ptr != nullptr)
            {
                //Creates a new product type
                IAbstractFactory<BaseType>* fact = ptr->As<IAbstractFactory<BaseType>>();
                if(fact == nullptr)
                {
                    FIEA_ASSERT("Creating an abstract factory failed its conversions in IFactoryService.");
                    return nullptr;
                }
                return fact->Create();
            }
            FIEA_ASSERT("Factory does not exist in IFactoryService.");
            return nullptr;
        }
    protected:
        // associate this factory with an abstract factory for the class w/ the given id
        virtual void RegisterFactory(RTTI::IdType baseId, std::unique_ptr<IFactoryBase>&& factory) = 0;
        // retrieve a factory associated with abstract factory for the class w/ the given id, matching the provided productName
        virtual IFactoryBase* GetFactory(RTTI::IdType baseId, const string& productName) const = 0;

        using FactoryMap = std::map< std::pair<RTTI::IdType, string>, std::unique_ptr<IFactoryBase> >;
        //Make a storage conatiner to contain a reference to each factory - still determining which key to use though (using string fn)
        FactoryMap _FactMap;
    };
    /*
    * IFactoryService <-> SpellManager
    * IAbstractFactory<T> <-> ISpellMaker<T>
    * Scope <-> ISpell
    * ScopedFoo::Scope <-> Fireball::ISpell
    */

    class FactoryService : Singleton<FactoryService>, public IFactoryService
    {
    protected:
        virtual void RegisterFactory(RTTI::IdType baseId, std::unique_ptr<IFactoryBase>&& factory) override
        {
            _FactMap.emplace(std::pair(baseId, factory.get()->GetProductName()), std::move(factory));
        }
        virtual IFactoryBase* GetFactory(RTTI::IdType baseId, const string& productName) const override
        {
            FactoryMap::const_iterator itr = _FactMap.find(std::pair(baseId, productName));
            FIEA_ASSERT(itr != _FactMap.end());
            if (itr != _FactMap.end())
            {
                FIEA_ASSERT(itr->first.first == baseId && itr->first.second == productName);
                return itr->second.get();
            }
            return nullptr;
        }
    };
}

#define FACTORY_DECLARE(ProductType, BaseType)                                             \
    class ProductType##Factory : public Fiea::Engine::IAbstractFactory<BaseType>           \
    {                                                                                      \
    public:                                                                                \
        virtual string GetProductName() override { return ProductType::TypeName(); }       \
        [[nodiscard]] virtual BaseType* Create() override { return new ProductType(); }    \
        RTTI_DECLARATIONS(ProductType##Factory, Fiea::Engine::IAbstractFactory<BaseType>); \
    };

#define FACTORY_DEFINE(ProductType)          \
    RTTI_DEFINITIONS(ProductType##Factory);