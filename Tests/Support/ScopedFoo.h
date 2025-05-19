#pragma once
#include "FieaGameEngine/Types.h"
#include "../../FieaGameEngine/include/FieaGameEngine/FactoryService.h"
#include "../Tests.h"
#include "../../FieaGameEngine/src/Content/Scope.h"

namespace Fiea::Engine::Tests::Support
{
    using Scope = Fiea::Engine::Content::Scope;
    class ScopedFoo : public Scope
    {
    public:
        explicit ScopedFoo(int32_t data = 0);
        ScopedFoo(const ScopedFoo& rhs);
        ScopedFoo(ScopedFoo&& rhs);
        ScopedFoo& operator=(const ScopedFoo& rhs);
        ScopedFoo& operator=(ScopedFoo&& rhs);
        virtual ~ScopedFoo();

        bool operator==(const ScopedFoo& rhs) const;
        bool operator!=(const ScopedFoo& rhs) const;

        std::int32_t Data() const;
        void SetData(int32_t data);

        // DA CLONE FUNCTION!!!
        virtual ScopedFoo* Clone() const override; // in ScopedFoo class

        //RTTI virtual functions
        string ToString() const override;
        bool Equals(const RTTI* rhs) const override;

        RTTI_DECLARATIONS(ScopedFoo, Scope);
    private:
        int32_t* _Data;
    };

    FACTORY_DECLARE(ScopedFoo, Scope);

}

// ToString specializations required for Are*Equal / Are*Same
#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    using Fiea::Engine::Tests::Support::ScopedFoo;

    template<>
    inline std::wstring ToString<ScopedFoo>(const ScopedFoo& t)
    {
        RETURN_WIDE_STRING("ScopedFoo:" << t.Data());
    }

    template<>
    inline std::wstring ToString<ScopedFoo>(const ScopedFoo* t)
    {
        RETURN_WIDE_STRING("ScopedFoo:" << t->Data());
    }

    template<>
    inline std::wstring ToString<ScopedFoo>(ScopedFoo* t)
    {
        RETURN_WIDE_STRING("ScopedFoo:" << t->Data());
    }
}