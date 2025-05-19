#include "Support/AttributeBox.h"

namespace Fiea::Engine::Tests::Support
{
    using namespace std::string_literals;

    RTTI_DEFINITIONS(AttributeBox);

#pragma region
    //--- OLD RO5 METHODS ---
    /// @brief 
    //AttributeBox::AttributeBox() : Attributed::Attributed(TypeIdInstance(), GetClassDefs()){}

    /// @brief 
    /// @param rhs 
    //AttributeBox::AttributeBox(const AttributeBox& rhs) : Attributed::Attributed(TypeIdInstance(), GetClassDefs())
    //{
    //    CopyFrom(rhs);
    //}

    /// @brief 
    /// @param rhs 
    //AttributeBox::AttributeBox(AttributeBox&& rhs) noexcept : Attributed::Attributed(TypeIdInstance(), GetClassDefs())
    //{
    //    CopyFrom(rhs);
    //    rhs.Clear();
    //}

    /// @brief 
    /// @param rhs 
    /// @return 
    //AttributeBox& AttributeBox::operator=(const AttributeBox& rhs)
    //{
    //    if(this != &rhs)
    //    {
    //        CopyFrom(rhs);
    //    }
    //    return *this;
    //}

    /// @brief 
    /// @param rhs 
    /// @return 
    //AttributeBox& AttributeBox::operator=(AttributeBox&& rhs) noexcept
    //{
    //    if (this != &rhs)
    //    {
    //        CopyFrom(rhs);
    //        rhs.Clear();
    //    }
    //    return *this;
    //}
#pragma endregion Old RO5 Methods

    // Scope overrides

    /// @brief 
    /// @return 
    [[nodiscard]] AttributeBox* AttributeBox::Clone() const
    {
        //return new AttributeBox(*this);
        return nullptr;
    }

    // RTTI overrides

    /// @brief 
    /// @return 
    std::string AttributeBox::ToString() const
    {
        return "AttributeBox"s;
    }

    /// @brief 
    /// @param rhs 
    /// @return 
    bool AttributeBox::Equals(const RTTI* rhs) const
    {
        // this would happen without the override, but included for clarity
        return Attributed::Equals(rhs);

        // if AttributeBox had fields that are NOT tracked by Scope, then there would be additional checks here
    }

#pragma region
    /// @brief 
    /// @param other 
    //void AttributeBox::CopyFrom(const AttributeBox& other)
    //{
    //    ExternalInteger = other.ExternalInteger;
    //    ExternalFloat = other.ExternalFloat;
    //    ExternalString = other.ExternalString;
    //    ExternalPointer = other.ExternalPointer;

    //    for (size_t i = 0; i < ARRAY_SIZE; ++i)
    //    {
    //        ExternalIntegerArray[i] = other.ExternalIntegerArray[i];
    //        ExternalFloatArray[i] = other.ExternalFloatArray[i];
    //        ExternalPointerArray[i] = other.ExternalPointerArray[i];
    //        ExternalStringArray[i] = other.ExternalStringArray[i];
    //    }
    //}
    ///// @brief 
    //void AttributeBox::Clear()
    //{
    //    ExternalString.clear();
    //    ExternalPointer = nullptr;
    //    for (size_t i = 0; i < ARRAY_SIZE; ++i)
    //    {
    //        ExternalPointerArray[i] = nullptr;
    //        ExternalStringArray[i].clear();
    //    }
    //}
    /// @brief 
    //void AttributeBox::Bind()
    //{
    //    Append("int").SetStorage(&ExternalInteger);
    //    Append("float").SetStorage(&ExternalFloat);
    //    Append("string").SetStorage(&ExternalString);
    //    Append("Object*").SetStorage(reinterpret_cast<RTTI**>(&ExternalPointer));

    //    Append("intArray").SetStorage(ExternalIntegerArray, ARRAY_SIZE);
    //    Append("floatArray").SetStorage(ExternalFloatArray, ARRAY_SIZE);
    //    Append("stringArray").SetStorage(ExternalStringArray, ARRAY_SIZE);
    //    Append("Object*Array").SetStorage(reinterpret_cast<RTTI**>(ExternalPointerArray), ARRAY_SIZE);
    //}
#pragma endregion Old 2-way Binding Methods

    Attributed::ClassDefinition AttributeBox::GetClassDefs() 
    {
        ClassDefinition fieldlist;
        fieldlist.Sigs = {
            { Signature("int", offsetof(AttributeBox, ExternalInteger), Datum::Type::Int, 1) },
            { Signature("float", offsetof(AttributeBox, ExternalFloat), Datum::Type::Float, 1) },
            { Signature("string", offsetof(AttributeBox, ExternalString), Datum::Type::String, 1) },
            { Signature("Object*", offsetof(AttributeBox, ExternalPointer), Datum::Type::Pointer, 1) },
            { Signature("intArray", offsetof(AttributeBox, ExternalIntegerArray), Datum::Type::Int, ARRAY_SIZE) },
            { Signature("floatArray", offsetof(AttributeBox, ExternalFloatArray), Datum::Type::Float, ARRAY_SIZE) },
            { Signature("stringArray", offsetof(AttributeBox, ExternalStringArray), Datum::Type::String, ARRAY_SIZE) },
            { Signature("Object*Array", offsetof(AttributeBox, ExternalPointerArray), Datum::Type::Pointer, ARRAY_SIZE) }
        };

        return fieldlist;
    }
}