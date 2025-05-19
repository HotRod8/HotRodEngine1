#include "ScopedFoo.h"

namespace Fiea::Engine::Tests::Support
{
    RTTI_DEFINITIONS(Fiea::Engine::Tests::Support::ScopedFoo);
    FACTORY_DEFINE(Fiea::Engine::Tests::Support::ScopedFoo);

    //Construction data gets build first, then the body gets built
    ScopedFoo::ScopedFoo(const ScopedFoo& rhs) : _Data(new int32_t(*rhs._Data)) {}
    /// @brief 
    /// @param rhs 
    ScopedFoo::ScopedFoo(ScopedFoo&& rhs) : _Data(new int32_t(*rhs._Data))
    {
        rhs._Data = nullptr;
    }
    /// @brief 
    /// @param rhs 
    /// @return 
    ScopedFoo& ScopedFoo::operator=(const ScopedFoo& rhs)
    {
        if (this != &rhs)
        {
            *_Data = *rhs._Data;
        }
        return *this;
    }
    /// @brief 
    /// @param rhs 
    /// @return 
    ScopedFoo& ScopedFoo::operator=(ScopedFoo&& rhs)
    {
        if (this != &rhs)
        {
            delete _Data;

            _Data = rhs._Data;
            rhs._Data = nullptr;
        }
        return *this;
    }
    /// @brief 
    ScopedFoo::~ScopedFoo()
    {
        delete _Data;
        _Data = nullptr;
    }
    /// @brief 
    /// @param data 
    ScopedFoo::ScopedFoo(int32_t data) : _Data(new int32_t(data))
    {
    }
    /// @brief 
    /// @param rhs 
    /// @return 
    bool ScopedFoo::operator==(const ScopedFoo& rhs) const
    {
        return *_Data == *rhs._Data;
    }
    /// @brief 
    /// @param rhs 
    /// @return 
    bool ScopedFoo::operator!=(const ScopedFoo& rhs) const
    {
        return !operator==(rhs);
    }

    /// @brief Retrieves a copy of the data held by this ScopedFoo
    /// @return A copy of the data
    int32_t ScopedFoo::Data() const
    {
        return *_Data;
    }

    /// @brief Sets the data within this ScopedFoo
    /// @param data The data to set
    void ScopedFoo::SetData(int32_t data)
    {
        *_Data = data;
    }

    /// @brief 
    /// @return 
    ScopedFoo* ScopedFoo::Clone() const
    {
        return new ScopedFoo(*this);
    }

    /// @brief Converts a ScopedFoo's data to a string
    /// @return ScopedFoo string
    string ScopedFoo::ToString() const
    {
        std::ostringstream ostr;
        ostr << TypeName() << "(" << Data() << ")" << std::endl;
        return ostr.str();
    }
    /// @brief Tests if 2 ScopedFoos have == data
    /// @param rhs - other ScopedFoo
    /// @return  returns T or F on ScopedFoo equivalency
    bool ScopedFoo::Equals(const RTTI* rhs) const
    {
        const ScopedFoo* other = rhs->As<ScopedFoo>();
        if (other == nullptr) return false;
        return *_Data == *other->_Data;
    }
}