#pragma once
#include "../Tests.h"
#include "../../FieaGameEngine/include/RTTI.h"

namespace Fiea::Engine::Tests::Support
{
    class Empty : public RTTI
    {
    public:
        virtual int32_t GetValue() const;
        RTTI_DECLARATIONS(Empty, RTTI);
    };

    class EmptyChild : public Empty
    {
    public:
        EmptyChild(int32_t i);
        int32_t GetValue() const override;
        string ToString() const override;
        bool Equals(const RTTI* rhs) const override;

        RTTI_DECLARATIONS(EmptyChild, Empty);
    private:
        int32_t _Val;
    };
}