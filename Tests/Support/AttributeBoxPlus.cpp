#include "Support/AttributeBoxPlus.h"

namespace Fiea::Engine::Tests::Support
{
    using namespace std::string_literals;

    RTTI_DEFINITIONS(AttributeBoxPlus);

#pragma region Old RO5 Methods
    //AttributeBoxPlus::AttributeBoxPlus(const AttributeBoxPlus& rhs)
    //{
    //    Bind();
    //    CopyFrom(rhs);
    //}
    //AttributeBoxPlus::AttributeBoxPlus(AttributeBoxPlus&& rhs) noexcept
    //{
    //    Bind();
    //    CopyFrom(rhs);
    //    // int gets cleared automatically
    //}
    //AttributeBoxPlus& AttributeBoxPlus::operator=(const AttributeBoxPlus& rhs)
    //{
    //    if (this != &rhs)
    //    {
    //        Bind();
    //        CopyFrom(rhs);
    //    }
    //    return *this;
    //}
    //AttributeBoxPlus& AttributeBoxPlus::operator=(AttributeBoxPlus&& rhs) noexcept
    //{
    //    if (this != &rhs)
    //    {
    //        Bind();
    //        CopyFrom(rhs);
    //        // int gets cleared automatically
    //    }
    //    return *this;
    //}
#pragma endregion 

    AttributeBoxPlus* AttributeBoxPlus::Clone() const
    {
        return new AttributeBoxPlus(*this);
    }
    std::string AttributeBoxPlus::ToString() const
    {
        return "AttributeBoxPlus"s;
    }
    bool AttributeBoxPlus::Equals(const RTTI* rhs) const
    {
        return AttributeBox::Equals(rhs);
    }

#pragma region Old 2-way Binds
    //void AttributeBoxPlus::CopyFrom(const AttributeBoxPlus& other)
    //{
    //    ExtraData = other.ExtraData;
    //}
    //void AttributeBoxPlus::Bind()
    //{
    //    Append("Extra Data").SetStorage(&ExtraData);
    //}
#pragma endregion

    Attributed::ClassDefinition AttributeBoxPlus::GetPlusClassDefs()
    {
        ClassDefinition allParentFields = AttributeBox::GetClassDefs();
        ClassDefinition fieldlist;
        fieldlist.Sigs = {
            { Signature("Extra Data", offsetof(AttributeBoxPlus, ExtraData), Datum::Type::Int, 1) },
            { Signature("Extra String", offsetof(AttributeBoxPlus, ExtraString), Datum::Type::String, 1) }
        };
        for(size_t i = 0; i < allParentFields.Sigs.size(); ++i)
        {
            fieldlist.Sigs.push_back(allParentFields.Sigs[i]);
        }

        return fieldlist;
    }
}