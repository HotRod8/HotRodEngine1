#include "Datum.h"
#include <cstdlib>
#include "Scope.h"

namespace Fiea::Engine::Content
{
    /// @brief Default constructor(ctor)
    Datum::Datum() : _Tag(None), _Size(0), _Capacity(0), _External(false) { _Data._Void = nullptr; }
    /// @brief Copy ctor
    /// @param other - pre-made Datum object to be copied
    Datum::Datum(const Datum& other) : _Tag(other._Tag), _Size(other._Size), _Capacity(other._Capacity), _External(other._External)
    {
        //initiallize values w/ one other element @ a time
        //don't use memcpy - doesn't work well w/ pointers
        if (_Tag == Int)
        {
            _Data._Digits = static_cast<int32_t*>(::operator new(sizeof(int32_t) * _Capacity));
            FIEA_ASSERT(_Size <= _Capacity);
            if (_Data._Digits == nullptr)
            {
                FIEA_ABORT_STR("Malloc failed on Datum constructor.");
            }
            for (size_t i = 0; i < other._Size; ++i)
            {
                _Data._Digits[i] = other._Data._Digits[i];
            }
        }
        else if (_Tag == Float)
        {
            _Data._Rationals = static_cast<float*>(::operator new(sizeof(float) * _Capacity));
            FIEA_ASSERT(_Size <= _Capacity);
            if (_Data._Rationals == nullptr)
            {
                FIEA_ABORT_STR("Malloc failed on Datum constructor.");
            }
            for (size_t i = 0; i < other._Size; ++i)
            {
                _Data._Rationals[i] = other._Data._Rationals[i];
            }
        }
        else if (_Tag == String)
        {
            _Data._Words = static_cast<string*>(::operator new(sizeof(string) * _Size));
            for (size_t i = 0; i < other._Size; ++i)
            {
                FIEA_ASSERT(_Size <= _Capacity);
                new(&_Data._Words[i]) string(other._Data._Words[i]);
            }
        }
        else if (_Tag == Pointer)
        {
            _Data._Inheriter = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * _Size));
            for (size_t i = 0; i < other._Size; ++i)
            {
                _Data._Inheriter[i] = other._Data._Inheriter[i];
            }
        }
        else if (_Tag == ScopeTable)
        {
            _Data._Scopes = static_cast<Scope**>(::operator new(sizeof(Scope*) * _Size));
            for (size_t i = 0; i < other._Size; ++i)
            {
                _Data._Scopes[i] = other._Data._Scopes[i];
            }
        }
    }
    /// @brief Move ctor
    /// @param rhs - Datum to be moved
    Datum::Datum(Datum&& rhs) noexcept : _Tag(rhs._Tag), _Size(rhs._Size), _Capacity(rhs._Capacity), _External(rhs._External)
    {
        if (_Tag == Int)
        {
            _Data._Digits = rhs._Data._Digits;
            rhs._Data._Digits = nullptr;
        }
        else if (_Tag == Float)
        {
            _Data._Rationals = rhs._Data._Rationals;
            rhs._Data._Rationals = nullptr;
        }
        else if (_Tag == String)
        {
            _Data._Words = rhs._Data._Words;
            rhs._Data._Words = nullptr;
        }
        else if (_Tag == Pointer)
        {
            _Data._Inheriter = rhs._Data._Inheriter;
            rhs._Data._Inheriter = nullptr;
        }
        else if (_Tag == ScopeTable)
        {
            _Data._Scopes = rhs._Data._Scopes;
            rhs._Data._Scopes = nullptr;
        }
        rhs._Size = rhs._Capacity = 0;
        rhs._Tag = None;
    }
    /// @brief - Copy Assignment operator
    /// @param rhs - copies Datum content into a premade Datum
    /// @return Newly formed Datum w/ the content from rhs
    Datum& Datum::operator=(const Datum& rhs)
    {
        //if rhs is external data, abort and notify the player of this invalid op
        if (rhs._External == true && _External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        //if same, skip init and return *this
        if(this != &rhs)
        {
            if (_Tag != None && _Tag != rhs._Tag)
            {
                FIEA_ERROR(_Tag == rhs._Tag);
            }

            switch (_Tag)
            {
            case Fiea::Engine::Content::Datum::None:
                break;
            case Fiea::Engine::Content::Datum::Int:
                break;
            case Fiea::Engine::Content::Datum::Float:
                break;
            case Fiea::Engine::Content::Datum::String:
                ClearString();
                break;
            case Fiea::Engine::Content::Datum::Pointer:
                break;
            case Fiea::Engine::Content::Datum::ScopeTable:
                break;
            default:
                break;
            }

            _Tag = rhs._Tag;
            _Size = rhs._Size;
            _Capacity = rhs._Capacity;
            _External = rhs._External;
            //initiallize values w/ one other element @ a time
            //don't use memcpy - doesn't work well w/ pointers
            if (_Tag == Int)
            {
                _Data._Digits = static_cast<int32_t*>(::operator new(sizeof(int32_t) * _Capacity));
                FIEA_ASSERT(_Size <= _Capacity);
                if (_Data._Digits == nullptr)
                {
                    FIEA_ABORT_STR("Malloc failed on Datum operator=.");
                }
                for (size_t i = 0; i < rhs._Size; ++i)
                {
                    _Data._Digits[i] = rhs._Data._Digits[i];
                }
            }
            else if (_Tag == Float)
            {
                _Data._Rationals = static_cast<float*>(::operator new(sizeof(float) * _Capacity));
                FIEA_ASSERT(_Size <= _Capacity);
                if (_Data._Rationals == nullptr)
                {
                    FIEA_ABORT_STR("Malloc failed on Datum operator=.");
                }
                for (size_t i = 0; i < rhs._Size; ++i)
                {
                    _Data._Rationals[i] = rhs._Data._Rationals[i];
                }
            }
            else if (_Tag == String)
            {
                _Data._Words = static_cast<string*>(::operator new(sizeof(string) * _Size));
                for (size_t i = 0; i < rhs._Size; ++i)
                {
                    FIEA_ASSERT(_Size <= _Capacity);
                    new(&_Data._Words[i]) string(rhs._Data._Words[i]);
                }
            }
            else if (_Tag == Pointer)
            {
                _Data._Inheriter = rhs._Data._Inheriter;
            }
            else if (_Tag == ScopeTable)
            {
                _Data._Scopes = rhs._Data._Scopes;
            }
        }
        return *this;
    }
    /// @brief - Move assignment operator
    /// @param rhs - Datum to move into the Datum caller
    /// @return Modified Datum references
    Datum& Datum::operator=(Datum&& rhs) noexcept
    {
        //if rhs is external data, abort and notify the player of this invalid op
        if(rhs._External == true && _External == true)
        {
            FIEA_ASSERT(_External != true);
        }

        //if same, skip init and return *this
        if (this != &rhs && _External == false)
        {
            switch (_Tag)
            {
            case Fiea::Engine::Content::Datum::None:
                break;
            case Fiea::Engine::Content::Datum::Int:
                break;
            case Fiea::Engine::Content::Datum::Float:
                break;
            case Fiea::Engine::Content::Datum::String:
                ClearString();
                break;
            case Fiea::Engine::Content::Datum::Pointer:
                break;
            case Fiea::Engine::Content::Datum::ScopeTable:
                break;
            default:
                break;
            }

            _Tag = rhs._Tag;
            _Size = rhs._Size;
            _Capacity = rhs._Capacity;
            _External = rhs._External;
            //initiallize values w/ one other element @ a time
            //don't use memcpy - doesn't work well w/ pointers
            if (_Tag == Int)
            {
                _Data._Digits = rhs._Data._Digits;
                rhs._Data._Digits = nullptr;
            }
            else if (_Tag == Float)
            {
                _Data._Rationals = rhs._Data._Rationals;
                rhs._Data._Rationals = nullptr;
            }
            else if (_Tag == String)
            {
                _Data._Words = rhs._Data._Words;
                rhs._Data._Words = nullptr;
            }
            else if (_Tag == Pointer)
            {
                _Data._Inheriter = rhs._Data._Inheriter;
                rhs._Data._Inheriter = nullptr;
            }
            else if (_Tag == ScopeTable)
            {
                _Data._Scopes = rhs._Data._Scopes;
                rhs._Data._Scopes = nullptr;
            }
            rhs._Size = rhs._Capacity = 0;
            rhs._Tag = None;
        }
        return *this;
    }
    /// @brief Destructor
    Datum::~Datum()
    {
        if(_External == false)
        {
            switch (_Tag)
            {
            case Fiea::Engine::Content::Datum::None:
                break;
            case Fiea::Engine::Content::Datum::Int:
                if(_Size > 0 && _Capacity > 0)
                {
                    delete _Data._Digits;
                }
                break;
            case Fiea::Engine::Content::Datum::Float:
                if (_Size > 0 && _Capacity > 0)
                {
                    delete _Data._Rationals;
                }
                break;
            case Fiea::Engine::Content::Datum::String:
                if (_Size > 0 && _Capacity > 0)
                {
                    ClearString();
                    delete _Data._Words;
                }
                break;
            case Fiea::Engine::Content::Datum::Pointer:
                if (_Size > 0 && _Capacity > 0)
                {
                    delete _Data._Inheriter;
                }
                break;
            case Fiea::Engine::Content::Datum::ScopeTable:
                if (_Size > 0 && _Capacity > 0)
                {
                    delete _Data._Scopes;
                }
                break;
            default:
                break;
            }
        }
    }

    /// @brief Int32_t Datum ctor
    /// @param - int32_t value to be stored
    Datum::Datum(const int32_t& numi) : _Tag(Int), _Size(1), _Capacity(1), _External(false)
    {
        _Data._Digits = static_cast<int32_t*>(::operator new(sizeof(int32_t) * _Size));
        if (_Data._Digits == nullptr)
        {
            FIEA_ABORT_STR("Malloc failed on int32 constructor.");
        }
        _Data._Digits[0] = numi;
    }
    /// @brief Float Datum ctor
    /// @param - float value to be stored
    Datum::Datum(const float& numf) : _Tag(Float), _Size(1), _Capacity(1), _External(false)
    {
        _Data._Rationals = static_cast<float*>(::operator new(sizeof(float) * _Size));
        if (_Data._Rationals == nullptr)
        {
            FIEA_ABORT_STR("Malloc failed on float constructor.");
        }
        _Data._Rationals[0] = numf;
    }
    /// @brief String Datum ctor
    /// @param - string value to be stored
    Datum::Datum(const string& word) : _Tag(String), _Size(1), _Capacity(1), _External(false)
    {
        _Data._Words = static_cast<string*>(::operator new(sizeof(string) * _Size));
        new(&_Data._Words[0]) string(word);
    }

    /// @brief RTTI Datum ctor
    /// @param rtti - RTTI value to be stored
    Datum::Datum(RTTI* const rtti) : _Tag(Pointer), _Size(1), _Capacity(1), _External(false)
    {
        _Data._Inheriter = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * _Size));
        while(_Data._Inheriter == nullptr)
        {
            _Data._Inheriter = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * _Size));
        }
        _Data._Inheriter[0] = rtti;
    }
    /// @brief Scope Datum ctor
    /// @param scp - Scope value to be stored
    Datum::Datum(Scope* const scp) : _Tag(ScopeTable), _Size(1), _Capacity(1), _External(false)
    {
        _Data._Scopes = static_cast<Scope**>(::operator new(sizeof(Scope*) * _Size));
        while (_Data._Scopes == nullptr)
        {
            _Data._Scopes = static_cast<Scope**>(::operator new(sizeof(Scope*) * _Size));
        }
        _Data._Scopes[0] = scp;
    }

    /// @brief IntInitList Datum ctor
    /// @param il - Int32 init list to pass in as values
    Datum::Datum(intInitList il) : _Tag(Int), _Size(il.size()), _Capacity(il.size()), _External(false)
    {
        _Data._Digits = static_cast<int32_t*>(::operator new(sizeof(int32_t) * _Size));
        if (_Data._Digits == nullptr)
        {
            FIEA_ABORT_STR("Malloc failed on int32 init list constructor.");
        }
        size_t i = 0;
        for(intInitList::iterator itr = il.begin(); itr != il.end(); ++itr, ++i)
        {
            _Data._Digits[i] = *itr;
        }
    }
    /// @brief FloatInitList Datum ctor
    /// @param il - Float init list to pass in as values
    Datum::Datum(floatInitList il) : _Tag(Float), _Size(il.size()), _Capacity(il.size()), _External(false)
    {
        _Data._Rationals = static_cast<float*>(::operator new(sizeof(float) * _Size));
        if (_Data._Rationals == nullptr)
        {
            FIEA_ABORT_STR("Malloc failed on float init list constructor.");
        }
        size_t i = 0;
        for (floatInitList::iterator itr = il.begin(); itr != il.end(); ++itr, ++i)
        {
            _Data._Rationals[i] = *itr;
        }
    }
    /// @brief StringInitList Datum ctor
    /// @param il - string init list to pass in as values
    Datum::Datum(strInitList il) : _Tag(String), _Size(il.size()), _Capacity(il.size()), _External(false)
    {
        _Data._Words = static_cast<string*>(::operator new(sizeof(string) * _Size));
        size_t i = 0;
        for (strInitList::iterator itr = il.begin(); itr != il.end(); ++itr, ++i)
        {
            new(&_Data._Words[i]) string(*itr);
        }
    }
    /// @brief rttiInitList Datum ctor
    /// @param il - rtti init list to pass in as values
    Datum::Datum(rttiInitList il) : _Tag(Pointer), _Size(il.size()), _Capacity(il.size()), _External(false)
    {
        _Data._Inheriter = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * _Size));
        if (_Data._Inheriter == nullptr)
        {
            FIEA_ABORT_STR("Malloc failed on RTTI init list constructor.");
        }
        size_t i = 0;
        for (rttiInitList::iterator itr = il.begin(); itr != il.end(); ++itr, ++i)
        {
            _Data._Inheriter[i] = *itr;
        }
    }
    /// @brief scopeInitList Datum ctor
    /// @param il - scope init list to pass in as values
    Datum::Datum(scopeInitList il) : _Tag(ScopeTable), _Size(il.size()), _Capacity(il.size()), _External(false)
    {
        _Data._Scopes = static_cast<Scope**>(::operator new(sizeof(Scope*) * _Size));
        if (_Data._Scopes == nullptr)
        {
            FIEA_ABORT_STR("Malloc failed on scope init list constructor.");
        }
        size_t i = 0;
        for (scopeInitList::iterator itr = il.begin(); itr != il.end(); ++itr, ++i)
        {
            _Data._Scopes[i] = *itr;
        }
    }

    /// @brief Int32 assign op
    /// @param - the new int32_t to be assigned as the first element in this Datum
    /// @return - the new int32_t
    int32_t& Datum::operator=(const int32_t& numi)
    {
        if(_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an int to outside data.");
            int32_t* i = new int32_t(-1);
            return *i;
        }

        if(_Size > 0 && _Capacity >= _Size && _Tag == Int)
        {
            for(size_t i = _Size; i > 0; --i)
            {
                PopInt();
            }
        }
        else if(_Capacity == 0 && _Tag == Int)
        {
            _Capacity = 1;
            _Data._Digits = static_cast<int32_t*>(::operator new(sizeof(int32_t) * _Capacity));
            if(_Data._Digits == nullptr)
            {
                FIEA_ABORT_STR("Malloc failed on int32 operator=.");
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an int to a data type of non-int.");
            int32_t* i = new int32_t(-1);
            return *i;
        }
        _Size = 1;
        _Data._Digits[0] = numi;
        return *_Data._Digits;
    }
    /// @brief float assign op
    /// @param - the new float to be assigned as the first element in this Datum
    /// @return - the new float
    float& Datum::operator=(const float& numf)
    {
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an float to outside data.");
            float* i = new float(-1.0);
            return *i;
        }

        if (_Size > 0 && _Capacity >= _Size && _Tag == Float)
        {
            for (size_t i = _Size; i > 0; --i)
            {
                PopFloat();
            }
        }
        else if (_Capacity == 0 && _Tag == Float)
        {
            _Capacity = 1;
            _Data._Rationals = static_cast<float*>(::operator new(sizeof(float) * _Capacity));
            if (_Data._Rationals == nullptr)
            {
                FIEA_ABORT_STR("Malloc failed on Datum constructor.");
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an int to a data type of non-int.");
            float* i = new float(-1.0);
            return *i;
        }
        _Size = 1;
        _Data._Rationals[0] = numf;
        return *_Data._Rationals;
    }
    /// @brief string assign op
    /// @param - the new string to be assigned as the first element in this Datum
    /// @return - the new string
    string& Datum::operator=(const string& str)
    {
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an float to outside data.");
            string i = "";
            return i;
        }

        if (_Size > 0 && _Capacity >= _Size && _Tag == String)
        {
            for (size_t i = _Size; i > 0; --i)
            {
                PopString();
            }
        }
        else if (_Capacity == 0 && _Tag == String)
        {
            _Capacity = 1;
            _Data._Words = static_cast<string*>(::operator new(sizeof(string) * _Size));
            if (_Data._Words == nullptr)
            {
                FIEA_ABORT_STR("Malloc failed on Datum constructor.");
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an float to outside data.");
            string i = "";
            return i;
        }
        _Size = 1;
        new(&_Data._Words[0]) string(str);
        return *_Data._Words;
    }
    /// @brief RTTI assign op
    /// @param - the new RTTI to be assigned as the first element in this Datum
    /// @return - the new RTTI
    RTTI* Datum::operator=(RTTI* const rtti)
    {
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an float to outside data.");
            RTTI* i = nullptr;
            return i;
        }

        if (_Size > 0 && _Capacity >= _Size && _Tag == Pointer)
        {
            for (size_t i = _Size; i > 0; --i)
            {
                PopRTTI();
            }
        }
        else if (_Capacity == 0 && _Tag == Pointer)
        {
                _Capacity = 1;
                _Data._Inheriter = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * _Capacity));
                if (_Data._Inheriter == nullptr)
                {
                    FIEA_ABORT_STR("Malloc failed on Datum constructor.");
                }
        }
        else
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an float to outside data.");
            RTTI* i = nullptr;
            return i;
        }
        _Size = 1;
        _Data._Inheriter[0] = rtti;
        return *_Data._Inheriter;
    }
    /// @brief Scope assign op
    /// @param - the new Scope to be assigned as the first element in this Datum
    /// @return - the new Scope
    Scope* Datum::operator=(Scope* const scp)
    {
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an float to outside data.");
            Scope* i = nullptr;
            return i;
        }

        if (_Size > 0 && _Capacity >= _Size && _Tag == ScopeTable)
        {
            for (size_t i = _Size; i > 0; --i)
            {
                PopScope();
            }
        }
        else if (_Capacity == 0 && _Tag == ScopeTable)
        {
            _Capacity = 1;
            _Data._Scopes = static_cast<Scope**>(::operator new(sizeof(Scope*) * _Capacity));
            if (_Data._Rationals == nullptr)
            {
                FIEA_ABORT_STR("Malloc failed on Datum constructor.");
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid op: Tried to set an float to outside data.");
            Scope* i = nullptr;
            return i;
        }
        _Size = 1;
        _Data._Scopes[0] = scp;
        return *_Data._Scopes;
    }

    /// @brief Comparison Op== for Datums
    /// @param dat - Datum value
    /// @return bool value
    bool Datum::operator==(const Datum& dat) const
    {
        if(this == &dat)
        {
            return true;
        }

        if(_Tag == dat._Tag && _Size == dat._Size && _Capacity == dat._Capacity)
        {
            switch (_Tag)
            {
            case Fiea::Engine::Content::Datum::None:
                break;
            case Fiea::Engine::Content::Datum::Int:
                for (size_t i = 0; i < _Size; ++i)
                {
                    if (_Data._Digits[i] != dat._Data._Digits[i])
                    {
                        return false;
                    }
                }
                break;
            case Fiea::Engine::Content::Datum::Float:
                for (size_t i = 0; i < _Size; ++i)
                {
                    if (_Data._Rationals[i] != dat._Data._Rationals[i])
                    {
                        return false;
                    }
                }
                break;
            case Fiea::Engine::Content::Datum::String:
                for (size_t i = 0; i < _Size; ++i)
                {
                    if (_Data._Words[i] != dat._Data._Words[i])
                    {
                        return false;
                    }
                }
                break;
            case Fiea::Engine::Content::Datum::Pointer:
                for (size_t i = 0; i < _Size; ++i)
                {
                    if (_Data._Inheriter[i]->Equals( dat._Data._Inheriter[i] ) == false)
                    {
                        return false;
                    }
                }
                break;
            case Fiea::Engine::Content::Datum::ScopeTable:
                for (size_t i = 0; i < _Size; ++i)
                {
                    if (_Data._Scopes[i] != dat._Data._Scopes[i])
                    {
                        return false;
                    }
                }
                break;
            default:
                FIEA_ABORT_STR("Invalid type was somehow put into the tag.");
                return false;
                break;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    /// @brief Comparison Op!= for Datums
    /// @param dat - Datum value
    /// @return bool value
    bool Datum::operator!=(const Datum& dat) const
    {
        return !(*this == dat);
    }

    /// @brief Sets the type enum data for a default Datum
    /// @param - desired Type(Int32, Float, String, Pointer, ScopeTable)
    void Datum::SetType(Type datumType)
    {
        if(_Tag == None)
        {
            _Tag = datumType;
            switch (_Tag)
            {
            case Fiea::Engine::Content::Datum::None:
                break;
            case Fiea::Engine::Content::Datum::Int:
                _Data._Digits = nullptr;
                break;
            case Fiea::Engine::Content::Datum::Float:
                _Data._Rationals = nullptr;
                break;
            case Fiea::Engine::Content::Datum::String:
                _Data._Words = nullptr;
                break;
            case Fiea::Engine::Content::Datum::Pointer:
                _Data._Inheriter = nullptr;
                break;
            case Fiea::Engine::Content::Datum::ScopeTable:
                _Data._Scopes = nullptr;
                break;
            default:
                break;
            }
        }
        else
        {
            FIEA_ABORT_STR("Type has already been set. Cannot modify it.");
        }
    }
    /// @brief Checks your Datum type
    /// @param - checking Type
    /// @return bool value
    bool Datum::IsType(Type datumType) const
    {
        return (_Tag == datumType);
    }

    /// @brief Replaces a old int value w/ a new one; DOES NOT touch the size or the capacity of the array
    /// @param val - int32 val to be placed in the array
    /// @param idx - index to set val at in array
    void Datum::SetInt(const int32_t& val, size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        _Data._Digits[idx] = val;
    }
    /// @brief Grabs the int at your requested index if it exists
    /// @param idx - index
    /// @return - your requested int32 reference
    int32_t& Datum::GetInt(size_t idx)
    {
        if(_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Digits[idx];
    }
    /// @brief Grabs the int at your requested index if it exists
    /// @param idx - index
    /// @return - your requested int32 reference
    const int32_t& Datum::GetInt(size_t idx) const
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Digits[idx];
    }
    /// @brief Places a new int value to the end of an array; Modifies the size by one and the capacity by double 
    /// @param val - int32 to add @ the end of the array
    void Datum::PushInt(const int32_t& val)
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if (_Tag == Int)
        {
            int32_t* temp = _Data._Digits;
            if (_Size == _Capacity && _Data._Digits != nullptr)
            {
                _Capacity *= 2;
                //realloc shoves a copy of the old array to a newly allocated address
                _Data._Digits = static_cast<int32_t*>(realloc(static_cast<int32_t*>(_Data._Digits), sizeof(int32_t) * _Capacity));
            }
            else if (_Data._Digits == nullptr && _Capacity == 0)
            {
                _Capacity = 1;
                _Data._Digits = static_cast<int32_t*>(::operator new(sizeof(int32_t) * _Capacity));
            }

            if (_Data._Digits == nullptr && temp != nullptr)
            {
                ::operator delete(temp);
                FIEA_ABORT_STR("Realloc failed on PushInt.");
            }
            else if (_Data._Digits == nullptr && temp == nullptr)
            {
                FIEA_ABORT_STR("Int array failed to malloc.");
            }

            _Data._Digits[_Size++] = val;
        }
        else
        {
            FIEA_ABORT_STR("Your tag is not Int. Please only use appropriate pushes for your Datum Type.");
        }
    }
    /// @brief Deinstantiates the last element of the array unless size == 0
    void Datum::PopInt()
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if (_Data._Digits != nullptr && _Tag == Int)
        {
            if (_Size > 0)
            {
                --_Size;
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid Pop(). Please double-check that your Datum's int32 array != nullptr & that your Datum's tag == Int.");
        }
    }


    /// @brief Replaces a old int value w/ a new one; DOES NOT touch the size or the capacity of the array
    /// @param val - int32 val to be placed in the array
    /// @param idx - index to set val at in array
    void Datum::SetFloat(const float& val, size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        _Data._Rationals[idx] = val;
    }
    /// @brief Grabs the int at your requested index if it exists
    /// @param idx - index
    /// @return - float
    float& Datum::GetFloat(size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Rationals[idx];
    }
    /// @brief Grabs the int at your requested index if it exists
    /// @param idx - index
    /// @return - float
    const float& Datum::GetFloat(size_t idx) const
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Rationals[idx];
    }
    /// @brief Push float param into the float array
    /// @param val - float val to be pushed
    void Datum::PushFloat(const float& val)
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if(_Tag == Float)
        {
            float* temp = _Data._Rationals;
            if (_Size == _Capacity && _Size != 0 && _Data._Rationals != nullptr)
            {
                _Capacity *= 2;
                //realloc shoves a copy of the old array to a newly allocated address
                _Data._Rationals = static_cast<float*>(realloc(static_cast<float*>(_Data._Rationals), sizeof(float) * _Capacity));
            }
            else if (_Data._Rationals == nullptr && _Capacity == 0)
            {
                _Capacity = 1;
                _Data._Rationals = static_cast<float*>(::operator new(sizeof(float) * _Capacity));
            }

            if (_Data._Rationals == nullptr && temp != nullptr)
            {
                ::operator delete(temp);
                FIEA_ABORT_STR("Realloc failed on PushFloat.");
            }
            else if (_Data._Rationals == nullptr && temp == nullptr)
            {
                FIEA_ABORT_STR("Float array failed to malloc.");
            }

            _Data._Rationals[_Size++] = val;
        }
        else
        {
            FIEA_ABORT_STR("Your tag is not Float. Please only use appropriate pushes for your Datum Type.");
        }
    }
    /// @brief Deinstantiates the last element of the array unless size == 0
    void Datum::PopFloat()
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if(_Data._Rationals != nullptr && _Tag == Float)
        {
            if (_Size > 0)
            {
                --_Size;
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid Pop(). Please double-check that your Datum's float array != nullptr & that your Datum's tag == Float.");
        }
    }


    /// @brief Replaces a old string value w/ a new one; DOES NOT touch the size or the capacity of the array
    /// @param val - string val to be placed in the array
    /// @param idx - index to set val at in array
    void Datum::SetString(const string& val, size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        _Data._Words[idx] = val;
    }
    /// @brief Grabs a string from the string array and passes it back to the user
    /// @param idx - index
    /// @return Your requested string value
    string& Datum::GetString(size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Words[idx];
    }
    /// @brief Grabs a string from the string array and passes it back to the user
    /// @param idx - index
    /// @return Your requested const string value
    const string& Datum::GetString(size_t idx) const
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Words[idx];
    }
    /// @brief Adds a string to the string array
    /// @param val - string
    void Datum::PushString(const string& val)
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if (_Tag == String)
        {
            string* temp = _Data._Words;
            if (_Size == _Capacity && _Data._Words != nullptr)
            {
                _Capacity *= 2;
                //Find another way to do this step appropriately; don't want malloc to give a new address to an empty array
                _Data._Words = static_cast<string*>(realloc(static_cast<string*>(_Data._Words), sizeof(string) * _Capacity));
            }
            else if (_Data._Rationals == nullptr && _Capacity == 0)
            {
                _Capacity = 1;
                _Data._Words = static_cast<string*>(::operator new(sizeof(string) * _Capacity));
            }

            if (_Data._Words == nullptr)
            {
                _Data._Words = temp;
                ClearString();
                ::operator delete(_Data._Words);
                FIEA_ABORT_STR("Malloc failed on PushString.");
            }
            new(&_Data._Words[_Size++]) string(val);
        }
        else
        {
            FIEA_ABORT_STR("Your tag is not String. Please only use appropriate pushes for your Datum Type.");
        }
    }
    /// @brief Deletes the most recent string off the string array
    void Datum::PopString()
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if (_Data._Words != nullptr && _Tag == String)
        {
            if(_Size > 0)
            {
                _Data._Words[--_Size].clear();
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid Pop(). Please double-check that your Datum's string array != nullptr & that your Datum's tag == String.");
        }
    }
    /// @brief Deletes all the strings in the string array.
    void Datum::ClearString()
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if(_Size > 0)
        {
            for(size_t i = 0; i < _Size; ++i)
            {
                _Data._Words[i].~basic_string();
            }
        }
        _Size = 0;
    }


    /// @brief Replaces a old RTTI value w/ a new one; DOES NOT touch the size or the capacity of the array
    /// @param val - RTTI val to be placed in the array
    /// @param idx - index to set val at in array
   void Datum::SetRTTI(RTTI* const val, size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        _Data._Inheriter[idx] = val;
    }
   /// @brief Grabs a RTTI from the RTTI array and passes it back to the user
   /// @param idx - index
   /// @return Your requested RTTI value
    RTTI* Datum::GetRTTI(size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Inheriter[idx];
    }
    /// @brief Grabs a RTTI from the RTTI array and passes it back to the user
    /// @param idx - index
    /// @return Your requested RTTI value
    RTTI* const Datum::GetRTTI(size_t idx) const
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Inheriter[idx];
    }
    /// @brief Adds a RTTI to the RTTI array
    /// @param val - RTTI
    void Datum::PushRTTI(RTTI* const val)
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if (_Tag == Pointer)
        {
            RTTI** temp = _Data._Inheriter;
            if (_Size == _Capacity && _Size != 0 && _Data._Inheriter != nullptr)
            {
                _Capacity *= 2;
                //realloc shoves a copy of the old array to a newly allocated address
                _Data._Inheriter = static_cast<RTTI**>(realloc(static_cast<RTTI**>(_Data._Inheriter), sizeof(RTTI*) * _Capacity));
            }
            else if (_Data._Inheriter == nullptr && _Capacity == 0)
            {
                _Capacity = 1;
                _Data._Inheriter = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * _Capacity));
            }

            if (_Data._Inheriter == nullptr && temp != nullptr)
            {
                ::operator delete(temp);
                FIEA_ABORT_STR("Realloc failed on PushRTTI.");
            }
            else if (_Data._Inheriter == nullptr && temp == nullptr)
            {
                FIEA_ABORT_STR("RTTI array failed to malloc.");
            }

            _Data._Inheriter[_Size++] = val;
        }
        else
        {
            FIEA_ABORT_STR("Your tag is not RTTI. Please only use appropriate pushes for your Datum Type.");
        }

    }
    /// @brief Deletes the most recent RTTI off the RTTI array
    void Datum::PopRTTI()
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if (_Data._Inheriter != nullptr && _Tag == Pointer)
        {
            if (_Size > 0)
            {
                --_Size;
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid Pop(). Please double-check that your Datum's RTTI array != nullptr & that your Datum's tag == Pointer.");
        }
    }
    /// @brief Returns the index of the RTTI param if it exists (non-functional for now)
    /// @param rtti - The RTTI to find
    /// @return - INT16_MAX-1
    size_t Datum::IndexOfRTTI(RTTI& rtti)
    {
        return INT16_MAX-1;
    }
    /// @brief Removes the RTTI from your Datum if it exists
    /// @param idx - index of RTTI 
    void Datum::RemoveRTTIAt(size_t idx)
    {
    }

    /// @brief Replaces a old Scope value w/ a new one; DOES NOT touch the size or the capacity of the array
    /// @param val - Scope val to be placed in the array
    /// @param idx - index to set val at in array
    void Datum::SetScope(Scope* const val, size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        _Data._Scopes[idx] = val;
    }
    /// @brief Grabs a Scope from the Scope array and passes it back to the user
    /// @param idx - index
    /// @return Your requested Scope value
    Scope* Datum::GetScope(size_t idx)
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Scopes[idx];
    }
    /// @brief Grabs a Scope from the Scope array and passes it back to the user
    /// @param idx - index
    /// @return Your requested Scope value
    Scope* const Datum::GetScope(size_t idx) const
    {
        if (_Size <= idx || idx < 0)
        {
            FIEA_ACCESS_VIOLATION;
        }
        return _Data._Scopes[idx];
    }
    /// @brief Adds a Scope to the Scope array
    /// @param val - Scope
    void Datum::PushScope(Scope* const val)
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if (_Tag == ScopeTable)
        {
            Scope** temp = _Data._Scopes;
            if (_Size == _Capacity && _Size != 0 && _Data._Scopes != nullptr)
            {
                _Capacity *= 2;
                //realloc shoves a copy of the old array to a newly allocated address
                _Data._Scopes = static_cast<Scope**>(realloc(static_cast<Scope**>(_Data._Scopes), sizeof(Scope*) * _Capacity));
            }
            else if (_Data._Scopes == nullptr && _Capacity == 0)
            {
                _Capacity = 1;
                _Data._Scopes = static_cast<Scope**>(::operator new(sizeof(Scope*) * _Capacity));
            }

            if (_Data._Scopes == nullptr && temp != nullptr)
            {
                ::operator delete(temp);
                FIEA_ABORT_STR("Realloc failed on PushScope.");
            }
            else if (_Data._Scopes == nullptr && temp == nullptr)
            {
                FIEA_ABORT_STR("Scope array failed to malloc.");
            }

            _Data._Scopes[_Size++] = val;
        }
        else
        {
            FIEA_ABORT_STR("Your tag is not Scope. Please only use appropriate pushes for your Datum Type.");
        }
    }
    /// @brief Deletes the most recent Scope off the Scope array
    void Datum::PopScope()
    {
        //if this is external data, abort and notify the player of this invalid op
        if (_External == true)
        {
            FIEA_ABORT_STR("Invalid op: Using op= on data from an external source.");
        }

        if (_Data._Scopes != nullptr && _Tag == ScopeTable)
        {
            if (_Size > 0)
            {
                --_Size;
            }
        }
        else
        {
            FIEA_ABORT_STR("Invalid Pop(). Please double-check that your Datum's scope array != nullptr & that your Datum's tag == ScopeTable.");
        }
    }
    /// @brief Finds the index for a parameter within your Datum caller
    /// @param scope - the scope you want to find
    /// @return - index of Scope or INT16_MAX-1
    size_t Datum::IndexOfScope(Scope& scope)
    {
        if(IsType(ScopeTable) != false)
        {
            for (size_t i = 0; i < _Size; ++i)
            {
                //DOING EQUALITY CHECK FOR NOW
                if(_Data._Scopes[i] == &scope)
                {
                    return i;
                }
            }
        }
        return INT16_MAX-1;
    }
    /// @brief Removes the Scope from your Datum if it exists
    /// @param idx - index of Scope
    void Datum::RemoveScopeAt(size_t idx)
    {
        if(idx < _Size)
        {
            for(size_t i = idx; i < _Size-1; ++i)
            {
                _Data._Scopes[i] = _Data._Scopes[i + 1];
            }
            --_Size;
        }
        else
        {
            FIEA_ERROR(idx >= _Size);
        }
    }

    /// @brief Converts a string val to an int32 and passes into SetInt
    /// @param val - string val of an int to be placed in the int32 array
    /// @param idx - index to set val at in array
    void Datum::SetIntFromString(const string& numistr, size_t idx)
    {
        int32_t num = static_cast<int32_t>(strtol(numistr.c_str(), nullptr, 10));
        SetInt(num, idx);
    }

    /// @brief Gets int from the int array and passes back a string
    /// @param idx - index
    /// @return string version of requested int
    string Datum::GetIntToString(size_t idx) const
    {
        return std::to_string(GetInt(idx));
    }

    /// @brief Converts a string val to an int32 and passes into PushInt
    /// @param numistr - string val of an int to be placed in the int32 array
    void Datum::PushIntFromString(const string& numistr)
    {
        int32_t num = static_cast<int32_t>(strtol(numistr.c_str(), nullptr, 10));
        PushInt(num);
    }

    /// @brief Converts a string val to an float and passes into SetFloat
    /// @param val - string val of an float to be placed in the float array
    /// @param idx - index to set val at in array
    void Datum::SetFloatFromString(const string& numfstr, size_t idx)
    {
        float num = static_cast<float>(strtof(numfstr.c_str(), nullptr));
        SetFloat(num, idx);
    }

    /// @brief Gets int from the float array and passes back a string
    /// @param idx - index
    /// @return string version of requested int    
    string Datum::GetFloatToString(size_t idx) const
    {
        return std::to_string(GetFloat(idx));
    }

    /// @brief Converts a string val to an float and passes into PushFloat
    /// @param numistr - string val of an float to be placed in the float array
    void Datum::PushFloatFromString(const string& numfstr)
    {
        float num = static_cast<float>(strtof(numfstr.c_str(), nullptr));
        PushFloat(num);
    }

    
    /// @brief Returns Datum size
    /// @return Datum size
    size_t Datum::Size() const
    {
        //For external storage, size == capacity
        return _Size;
    }
    /// @brief Returns Datum capacity
    /// @return Datum capacity
    size_t Datum::Capacity() const
    {
        return _Capacity;
    }

    /// @brief Sets storage for a pre-allocated int32 array
    /// @param items - int32 array
    /// @param count - array size
    void Datum::SetStorage(int32_t* items, size_t count)
    {
        //if(_Size == 0 && _Capacity == 0 && _Tag == None)
        {
            _External = true;
            _Tag = Int;
            _Data._Digits = items;
            _Size = _Capacity = count;
        }
    }

    /// @brief Sets storage for a pre-allocated float array
    /// @param items - float array
    /// @param count - array size
    void Datum::SetStorage(float* items, size_t count)
    {
       // if (_Size == 0 && _Capacity == 0 && _Tag == None)
        {
            _External = true;
            _Tag = Float;
            _Data._Rationals = items;
            _Size = _Capacity = count;
        }
    }

    /// @brief Sets storage for a pre-allocated string array
    /// @param items - string array
    /// @param count - array size
    void Datum::SetStorage(string* items, size_t count)
    {
        if (_Size == 0 && _Capacity == 0 && _Tag == None)
        {
            _External = true;
            _Tag = String;
            _Data._Words = items;
            _Size = _Capacity = count;
        }
    }
    /// @brief Sets storage for a pre-allocated RTTI array
    /// @param items - RTTI array
    /// @param count - array size
    void Datum::SetStorage(RTTI** items, size_t count)
    {
        if (_Size == 0 && _Capacity == 0 && _Tag == None)
        {
            _External = true;
            _Tag = Pointer;
            _Data._Inheriter = items;
            _Size = _Capacity = count;
        }
    }
    /// @brief Sets storage for a pre-allocated Scope array
    /// @param items - Scope array
    /// @param count - array size
    void Datum::SetStorage(Scope** items, size_t count)
    {
        if (_Size == 0 && _Capacity == 0 && _Tag == None)
        {
            _External = true;
            _Tag = ScopeTable;
            _Data._Scopes = items;
            _Size = _Capacity = count;
        }
    }
    /// @brief Sets storage for any external field from attributed
    /// @param extField - address of an external field from AttributeBox
    /// @param enumType - enum type to give the 
    /// @param count - array size
    void Datum::SetStorage(void* extField, Type enumType, size_t count)
    {
        if (_Size == 0 && _Capacity == 0 && _Tag == None && enumType != None)
        {
            switch (enumType)
            {
            case Int:
                _Tag = Int;
                _Data._Digits = reinterpret_cast<int32_t*>(extField);
                break;
            case Float:
                _Tag = Float;
                _Data._Rationals = reinterpret_cast<float*>(extField);
                break;
            case String:
                _Tag = String;
                _Data._Words = reinterpret_cast<string*>(extField);
                break;
            case Pointer:
                _Tag = Pointer;
                _Data._Inheriter = reinterpret_cast<RTTI**>(extField);
                break;
            case ScopeTable:
                _Tag = ScopeTable;
                _Data._Scopes = reinterpret_cast<Scope**>(extField);
                break;
            }
            _External = true;
            _Size = _Capacity = count;
        }
    }
    /// @brief Returns whether or not Datum contains external data
    /// @return T or F
    bool Datum::IsExternal() const
    {
        return _External;
    }
}