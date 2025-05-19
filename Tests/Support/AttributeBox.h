#pragma once
#include "../../FieaGameEngine/src/Content/Attributed.h"
#include "Foo.h"
#include "../Tests.h"

namespace Fiea::Engine::Tests::Support
{
    using namespace Content;

    class AttributeBox : public Attributed
    {
        RTTI_DECLARATIONS(AttributeBox, Attributed);
        using ClassDef = const ClassDefinition(*) (void);

    public:
        static const size_t ARRAY_SIZE = 5;

        // Rule-of-5 methods
        AttributeBox() : Attributed(TypeIdInstance(), GetClassDefs()) {};
        AttributeBox(const AttributeBox& rhs) = default;
        AttributeBox(AttributeBox&& rhs) noexcept = default;
        AttributeBox& operator=(const AttributeBox& rhs) = default;
        AttributeBox& operator=(AttributeBox&& rhs) noexcept = default;
        virtual ~AttributeBox() = default;

        // Scope overrides
        [[nodiscard]] AttributeBox* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        int32_t ExternalInteger{};
        float ExternalFloat{};
        string ExternalString{};
        Foo* ExternalPointer{};

        int32_t ExternalIntegerArray[ARRAY_SIZE]{};
        float ExternalFloatArray[ARRAY_SIZE]{};
        string ExternalStringArray[ARRAY_SIZE]{};
        Foo* ExternalPointerArray[ARRAY_SIZE]{};

    private:
        //Other functions
        //void CopyFrom(const AttributeBox& other);
        //void Clear();
       // void Bind();
    protected:
        ClassDefinition GetClassDefs();
        AttributeBox(RTTI::IdType classId, ClassDefinition classDefs) : Attributed(classId, classDefs) {};
    };
}

// ToString specializations required for Are*Equal / Are*Same
#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    using Fiea::Engine::Tests::Support::AttributeBox;

    template<>
    inline std::wstring ToString<AttributeBox>(const AttributeBox& t)
    {
        RETURN_WIDE_STRING(t.ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBox>(const AttributeBox* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBox>(AttributeBox* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }
}