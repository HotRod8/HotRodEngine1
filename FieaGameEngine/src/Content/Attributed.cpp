#include "Attributed.h"

using namespace std::string_literals;

namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(Attributed);

    //RO5 methods
    Attributed::Attributed(const Attributed& other) : Scope(other), _Defs(other._Defs)
    {
        BindAtts(_Defs);
    }
    Attributed::Attributed(Attributed&& rhs) noexcept : Scope(rhs), _Defs(rhs._Defs)
    {
        BindAtts(_Defs);
    }
    Attributed& Attributed::operator=(const Attributed& other)
    {
        if (this != &other)
        {
            BindAtts(_Defs);
        }
        return *this;
    }
    Attributed& Attributed::operator=(Attributed&& rhs) noexcept
    {
        if(this != &rhs)
        {
            BindAtts(_Defs);
        }
        return *this;
    }

    Attributed::Attributed(RTTI::IdType classId, ClassDefinition fieldDefFunc)
    {
        BindAtts(fieldDefFunc);
        _Defs = fieldDefFunc;
    }

    Attributed* Attributed::Clone() const
    {
        return new Attributed(*this);
    }
    
    std::string Attributed::ToString() const
    {
        return "Attributed"s;
    }

    bool Attributed::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }
    void Attributed::BindAtts(ClassDefinition classDef)
    {
        //binding logic
        for (const auto& signature : classDef.Sigs)
        {
            //Get address of the field from each signature
            //Use 1B on reinterpretation for an address. 
            //Otherwise, the following data (aka our desired field addresses) 
            //beyond the offset gets weeded out as garbage.
            void* extFieldAddr = reinterpret_cast<char*>(this) + signature.Offset;

            Datum& datAddress = Append(signature.AttributeName);
            datAddress.SetStorage(extFieldAddr, signature.Tag, signature.Count);
        }
    }
}