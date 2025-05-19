#include "Support/SupportClass.h"

namespace Fiea::Engine::Tests::Support
{
    RTTI_DEFINITIONS(EmptyChild);
    RTTI_DEFINITIONS(Empty);

    int32_t Empty::GetValue() const
    {
        return 0;
    }

    EmptyChild::EmptyChild(int32_t i) : _Val(i) {}

    int32_t EmptyChild::GetValue() const
    {
        return _Val;
    }

    string EmptyChild::ToString() const
    {
        std::ostringstream ostr;
        ostr << TypeName() << "(" << GetValue() << ")" << std::endl;
        return ostr.str();
    }

    bool EmptyChild::Equals(const RTTI* rhs) const
    {
        const EmptyChild* other = rhs->As<EmptyChild>();
        if (other == nullptr) return false;
        return _Val == other->_Val;
    }
}