#pragma once
#include "Support/AttributeBox.h"

namespace Fiea::Engine::Tests::Support
{
    using namespace Content;

    class AttributeBoxPlus : public AttributeBox
    {
        RTTI_DECLARATIONS(AttributeBoxPlus, AttributeBox);

    public:
        static const size_t ARRAY_SIZE = 5;

        // Rule-of-5 methods
        AttributeBoxPlus() : AttributeBox(TypeIdInstance(), GetPlusClassDefs()) {};
        AttributeBoxPlus(const AttributeBoxPlus& rhs) = default;
        AttributeBoxPlus(AttributeBoxPlus&& rhs) noexcept = default;
        AttributeBoxPlus& operator=(const AttributeBoxPlus& rhs) = default;
        AttributeBoxPlus& operator=(AttributeBoxPlus&& rhs) noexcept = default;
        virtual ~AttributeBoxPlus() = default;

        // Scope overrides
        [[nodiscard]] AttributeBoxPlus* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        int32_t ExtraData{};
        string ExtraString{};

        //Other functions
    protected:
        //void CopyFrom(const AttributeBoxPlus& other);
        //void Bind();
        ClassDefinition GetPlusClassDefs();
    };
}

// ToString specializations required for Are*Equal / Are*Same
#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    using Fiea::Engine::Tests::Support::AttributeBoxPlus;

    template<>
    inline std::wstring ToString<AttributeBoxPlus>(const AttributeBoxPlus& t)
    {
        RETURN_WIDE_STRING(t.ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBoxPlus>(const AttributeBoxPlus* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBoxPlus>(AttributeBoxPlus* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }
}