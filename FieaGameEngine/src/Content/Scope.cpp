#include "Scope.h"
#include <sstream>

namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(Fiea::Engine::Content::Scope);
    FACTORY_DEFINE(Fiea::Engine::Content::Scope);


    /// @brief Default ctor w/ def capacity param = 0
    /// @param capacity - optional capacity preset for all 
    Scope::Scope(size_t capacity) : _Size(0), _Capacity(capacity), _Parent(nullptr)
    {
        if(capacity > 0)
        {
            _DatumMap.reserve(_Capacity);
            _ValVect.reserve(_Capacity);
        }
        FIEA_ASSERT(capacity >= 0);
    }

    /// @brief 
    /// @param other 
    Scope::Scope(const Scope& other)
    {
        //Def constructors don't work for this because they perform shallow copies on dynamic data
        //Hence, I'm manually going each of other's immediate children, checking if any of them have type ScopeTable, and copying their elements over to our DatumMap and ValVect
        for (size_t i = 0; i < other._Size; ++i)
        {
            string key = "";
            Datum* dat = other._ValVect[i];
            bool matchFound = false;

            for (datumMap::const_iterator itr = other._DatumMap.begin(); itr != other._DatumMap.end(); ++itr)
            {
                if (itr->second == *dat)
                {
                    key = itr->first;
                    break;
                }
            }
            if(matchFound != true)
            {
                FIEA_ERROR("No match of a ValVect element found in the DatumMap.");
            }

            //If the data is external, only do shallow copies of the data
            if (dat->IsExternal() == false)
            {
                if (other._ValVect[i]->IsType(Datum::Type::ScopeTable) == true)
                {
                    for (size_t j = 0; j < dat->Size(); ++j)
                    {
                        AppendScope(key, dat->GetScope(j)->Clone());
                        FIEA_ASSERT(_DatumMap.find(key) != _DatumMap.end());
                        _ValVect[i] = &_DatumMap[key];
                    }
                }
                else
                {
                    _DatumMap.emplace(key, Datum(*dat));
                    FIEA_ASSERT(_DatumMap.find(key) != _DatumMap.end());
                    _ValVect.push_back(&_DatumMap[key]);
                }
            }
            else
            {
                _DatumMap = other._DatumMap;
                _ValVect = other._ValVect;
                break;
            }
        }
        _Parent = nullptr; 
        _Size = other._Size;
        _Capacity = other._Capacity;
    }

    /// @brief 
    /// @param other 
    /// @return 
    Scope& Scope::operator=(const Scope& other)
    {
        if(this != &other)
        {
            //If parent isn't nullptr, we need to orphan ourselves from our parent and then set our Parent ref to nullptr
            if(_Parent != nullptr)
            {
                Scope* scp = Orphan();
                _Parent = nullptr;
            }
            Clear();

            _Size = other._Size;
            _Capacity = other._Capacity;

            //Def constructors don't work for this because they perform shallow copies on dynamic data
            //Hence, I'm manually going each of other's immediate children, checking if any of them have type ScopeTable, and copying their elements over to our DatumMap and ValVect
            for (size_t i = 0; i < other._Size; ++i)
            {
                string key = "";
                Datum* dat = other._ValVect[i];

                for (datumMap::const_iterator itr = other._DatumMap.begin(); itr != other._DatumMap.end(); ++itr)
                {
                    if (itr->second == *dat)
                    {
                        key = itr->first;
                        break;
                    }
                }

                //If the data is external, only do shallow copies of the data
                if (dat->IsExternal() == false)
                {
                    if (other._ValVect[i]->IsType(Datum::Type::ScopeTable) == true)
                    {
                        for (size_t j = 0; j < dat->Size(); ++j)
                        {
                            AppendScope(key, dat->GetScope(j)->Clone());
                            FIEA_ASSERT(_DatumMap.find(key) != _DatumMap.end());
                            _ValVect[i] = &_DatumMap[key];
                        }
                    }
                    else
                    {
                        _DatumMap.emplace(key, Datum(*dat));
                        FIEA_ASSERT(_DatumMap.find(key) != _DatumMap.end());
                        _ValVect.push_back(&_DatumMap[key]);
                    }
                }
                else
                {
                    _DatumMap = other._DatumMap;
                    _ValVect = other._ValVect;
                    break;
                }
            }
        }
        return *this;
    }

    /// @brief 
    /// @param rhs 
    Scope::Scope(Scope&& rhs) noexcept : _Parent(nullptr), _Size(rhs._Size), _Capacity(rhs._Capacity), _DatumMap(std::move(rhs._DatumMap)), _ValVect(std::move(rhs._ValVect))
    {
        //Make sure to set all Scopes in the DatumMap to have this Scope as their parent.
        for (datumMap::const_iterator itr = _DatumMap.begin(); itr != _DatumMap.end(); ++itr)
        {
            if (itr->second.IsType(Datum::Type::ScopeTable) == true)
            {
                for(size_t i = 0; i < itr->second.Size(); ++i)
                {
                    itr->second.GetScope()->SetParent(*this);
                }
            }
        }
        rhs.Clear();
    }

    /// @brief 
    /// @param rhs 
    /// @return 
    Scope& Scope::operator=(Scope&& rhs) noexcept
    {
        if (this != &rhs)
        {
            //If parent isn't nullptr, we need to orphan ourselves from our parent and then set our Parent ref to nullptr
            if (_Parent != nullptr)
            {
                Scope* scp = Orphan();
                _Parent = nullptr;
            }
            Clear();

            _Size = rhs._Size;
            _Capacity = rhs._Capacity;
            _DatumMap = std::move(rhs._DatumMap);
            _ValVect = std::move(rhs._ValVect);
            //Make sure to set all Scopes in the DatumMap to have this Scope as their parent.
            for (datumMap::const_iterator itr = _DatumMap.begin(); itr != _DatumMap.end(); ++itr)
            {
                if (itr->second.IsType(Datum::Type::ScopeTable) == true)
                {
                    for (size_t i = 0; i < itr->second.Size(); ++i)
                    {
                        itr->second.GetScope()->SetParent(*this);
                    }
                }
            }
        }
        rhs.Clear();
        return *this;
    }

    /// @brief 
    Scope::~Scope()
    {
        Clear();
    }

    // Why not implement RO5?
    // 
    // Main reason for not implementing RO5 in general is b/c we don't OWN our data.
    // Use defaulted RO5 if we have references to other data objects.
    // Despite the variable size difference, we'll still save more memory by
    // not allocating a new heaps to a scope copy. We also don't need pointers
    // to make an easy move construction thanks to all containers having move 
    // implementations internally to rely on.

    //Other functions
    /// @brief Equals op for comparing equality b/tw 2 Scopes
    /// @param rhs - passed Scope param
    /// @return T/F for equality
    bool Scope::operator==(const Scope& rhs) const
    {
        if(_Size == rhs._Size && _Capacity == rhs._Capacity)
        {
            size_t i = 0;
            for (datumMap::const_iterator thisItr = _DatumMap.begin(), rhsItr = rhs._DatumMap.begin(); thisItr != _DatumMap.end() && rhsItr != rhs._DatumMap.end() && i < _Size; ++i, ++thisItr, ++rhsItr)
            {
                if (*thisItr != *rhsItr || *_ValVect[i] != *rhs._ValVect[i])
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    /// @brief Not equals op for comparing inequality b/tw 2 Scopes
    /// @param rhs - passed Scope param
    /// @return T/F for inequality
    bool Scope::operator!=(const Scope& rhs) const
    {
        return !(*this == rhs);
    }

    /// @brief return the Datum* at the requested key, or nullptr if not found
    /// @param key - string key for internal map
    /// @return - Either get the Datum* @ key or nullptr
    Datum* Scope::Find(const std::string& key)
    {
        if (_DatumMap.find(key) == _DatumMap.end())
        {
            return nullptr;
        }
        datumMap::iterator itr = _DatumMap.find(key);
        return &itr->second;
    }

    /// @brief return the const Datum* at the requested key, or nullptr if not found
    /// @param key - string key for internal map
    /// @return - Either get the const Datum* @ key or nullptr
    const Datum* Scope::Find(const std::string& key) const
    {
        if (_DatumMap.find(key) == _DatumMap.end())
        {
            return nullptr;
        }
        datumMap::const_iterator itr = _DatumMap.find(key);
        return &itr->second;
    }

    /// @brief Return a reference to the Datum at this key. Create a new Datum, if one is not found
    /// @param key - Map key to set or find
    /// @return - Datum reference @ that key
    Datum& Scope::Append(const std::string& key)
    {
        if(_DatumMap.find(key) == _DatumMap.end())
        {
            ++_Size;
            if (_Capacity == 0)
            {
                _Capacity = 1;
            }
            else if (_Size >= _Capacity)
            {
                _Capacity *= 2;
            }
            _DatumMap.emplace(key, Datum());
            _ValVect.push_back(&_DatumMap[key]);
        }
        return _DatumMap[key];
    }

    /// @brief syntactic sugar to wrap Append
    /// @param key - string key
    /// @return Datum obj value
    Datum& Scope::operator[](const std::string& key)
    {
        return Append(key);
    }

    /// @brief Retrieve a Datum by index. Out-of-range requests do not append
    /// @param idx - index
    /// @return Datum val from the Vector
    Datum& Scope::operator[](size_t idx)
    {
        if(_Size <= idx)
        {
            FIEA_ERROR(_Size > idx);
        }
        return *_ValVect[idx];
    }

    /// @brief Retrieve a Datum by index. Out-of-range requests do not append
    /// @param idx - index
    /// @return Datum val from the Vector
    const Datum& Scope::operator[](size_t idx) const
    {
        if (_Size <= idx)
        {
            FIEA_ERROR(_Size > idx);
        }
        return const_cast<const Datum&>(*_ValVect[idx]);
    }

    /// @brief Outputs the size of general Scope data
    /// @return - size for number of elements
    size_t Scope::Size() const
    {
        return _Size;
    }

    /// @brief Clears all the elements in this Scope
    void Scope::Clear()
    {
        if(_Size > 0 && _Capacity > 0)
        {
            for(datumMap::iterator itr = _DatumMap.begin(); itr != _DatumMap.end(); ++itr)
            {
                if(itr->second.IsType(Datum::Type::ScopeTable) == true)
                {
                    for(size_t i = 0; i < itr->second.Size(); ++i)
                    {
                        delete itr->second.GetScope(i);
                    }
                }
            }
        }
        _DatumMap.clear();
        _ValVect.clear();
        _Parent = nullptr;
        _Size = 0;
    }

    /// @brief Appends a Scope to the caller's Scope Map
    /// @param key - map index
    /// @param scope - scope value
    /// @return Returns the scope you made
    Scope& Scope::AppendScope(const std::string& key, Scope* scope)
    {
        if(scope == nullptr)
        {
            scope = new Scope();
        }

        //2 parts here: no Datum mapping, pre-existing Datum at key
        if(_DatumMap.find(key) != _DatumMap.end())
        {
            //6 parts for pre-existing Datum:
            //empty Datum type -> set type to Scope and add Scope*
            datumMap::iterator itr = _DatumMap.find(key);
            if(itr->second.IsType(Datum::Type::None) == true)
            {
                itr->second.SetType(Datum::Type::ScopeTable);
                scope->SetParent(*this);
                itr->second.PushScope(scope);
                size_t idx = itr->second.IndexOfScope(*scope);
                if(idx != -1)
                {
                    return *itr->second.GetScope(idx);
                }
                else
                {
                    FIEA_ABORT_STR("Scope w/ None type wasn't appended properly.");
                }
            }
            //Scope* Datum type -> add Scope* to the end
            else if(itr->second.IsType(Datum::Type::ScopeTable) == true)
            {
                scope->SetParent(*this);
                itr->second.PushScope(scope);
                size_t idx = itr->second.IndexOfScope(*scope);
                if (idx != -1)
                {
                    return *itr->second.GetScope(idx);
                }
                else
                {
                    FIEA_ABORT_STR("Scope w/ None type wasn't appended properly.");
                }
            }
            //Other Datum type -> throw exception b/c won't delete in this function
            else
            {
                FIEA_ERROR("Invalid key: Points to a different type than Scope**. Won't allow chages to pre-set types.");
                return *this;
            }
        }
        else
        {
            //create new Datum mapping, set Datum type to Scope*, and add scope to it
            ++_Size, ++_Capacity;
            scope->SetParent(*this);
            _DatumMap.emplace(key, Datum(scope));
            _ValVect.push_back(&_DatumMap[key]);
            return *_DatumMap[key].GetScope();
        }
    }

    /// @brief Adopts the Scope to the caller's Scope Map
    /// @param key - map index
    /// @param child - scope value
    void Scope::Adopt(const std::string& key, Scope& child)
    {
        if(child._Parent != nullptr)
        {
            Scope* orphan = child.Orphan();
            AppendScope(key, orphan);
        }
        else
        {
            AppendScope(key, &child);
        }
    }

    /// @brief Grabs caller's parent Scope
    /// @return Parent Scope*
    Scope* Scope::GetParent() const
    {
        return _Parent;
    }

    /// @brief Sets caller's parent Scope
    /// @param parent Parent Scope
    void Scope::SetParent(Scope& parent)
    {
        _Parent = &parent;
    }

    /// @brief Null the caller's reference to a parent 
    /// @return You, an Adam of Scopes 
    Scope* Scope::Orphan()
    {
        bool datTableExists = false;
        size_t idx;
        if(_Parent != nullptr)
        {
            FIEA_ASSERT(_Parent != nullptr);
            for(datumMap::iterator itr = _Parent->_DatumMap.begin(); itr != _Parent->_DatumMap.end(); ++itr)
            {
                if(itr->second.IsType(Datum::Type::ScopeTable) == true)
                {
                    datTableExists = true;
                    idx = itr->second.IndexOfScope(*this);
                    if(idx != INT16_MAX-1)
                    {
                        Datum* dat = _Parent->FindContainedScope(*this, idx);
                        FIEA_ASSERT(*dat == itr->second);
                        itr->second.RemoveScopeAt(idx);
                        _Parent = nullptr;
                        FIEA_ASSERT(_Parent == nullptr);
                        return this;
                    }
                }
            }
            //We only reach here if the player has no ScopeTable Datums or if we can't find a proper index of our caller in any of our ScopeTable datums
            if(datTableExists == true)
            {
                FIEA_ERROR(idx < 0);
            }
            else
            {
                FIEA_ERROR(datTableExists != true);
            }
        }
        FIEA_ASSERT(_Parent == nullptr);
        return this;
    }

    /// @brief Searches the map for your desired nested Scope
    /// @param child - The parent Scope
    /// @param idx - position for Scope schildren
    /// @return Wrapper that contains your Scope child
    Datum* Scope::FindContainedScope(const Scope& child, size_t& idx)
    {
        //Loop thru the _DatumMap to find the Datum containing the paramter child
        for(datumMap::iterator itr = _DatumMap.begin(); itr != _DatumMap.end(); ++itr)
        {
            //If child is SAME as Datum child, return that Datum
            //DOING EQUALITY CHECK FOR NOW
            if(itr->second.GetScope(idx) == &child)
            {
                return &itr->second;
            }
        }
        return nullptr;
    }

    /// @brief Searches the map for your desired nested Scope
    /// @param child - The parent Scope
    /// @param idx - position for Scope schildren
    /// @return Wrapper that contains your Scope child
    const Datum* Scope::FindContainedScope(const Scope& child, size_t& idx) const
    {
        //Loop thru the _DatumMap to find the Datum containing the paramter child
        for (datumMap::const_iterator itr = _DatumMap.begin(); itr != _DatumMap.end(); ++itr)
        {
            //If child is SAME as Datum child, return that Datum
            if (itr->second.GetScope(idx) == &child)
            {
                return &itr->second;
            }
        }
        return nullptr;
    }

    /// @brief Checks if your caller is an Ancestor of your Scope param. Please make sure the param is related your caller.
    /// @param descendent - Related Scope
    /// @return Is Ancestor or not
    bool Scope::IsAncestorOf(const Scope& descendent) const
    {
        return descendent.IsDescendentOf(*this);
    }

    /// @brief Checks if your caller is an Descendant of your Scope param. Please make sure the param is related your caller.
    /// @param ancestor - Related Scope
    /// @return Is Descendant or not
    bool Scope::IsDescendentOf(const Scope& ancestor) const
    {
        if(_Parent == &ancestor)
        {
            return true;
        }
        else if(_Parent == nullptr)
        {
            return false;
        }
        else
        {
            return _Parent->IsDescendentOf(ancestor);
        }
    }

    /// @brief Copy of the Scope caller
    /// @return new copy constructed Scope of the caller
    Scope* Scope::Clone() const
    {
        return new Scope(*this);
    }

    //RTTI virtual functions
    /// @brief Turns a Scope to a string 
    /// @return - Datatype(size)
    string Scope::ToString() const
    {
        std::ostringstream ostr;
        ostr << TypeName() << "(" << Size() << ")" << std::endl;
        return ostr.str();
    }
    /// @brief Tests equality b/tw 2 kids of RTTI
    /// @param rhs - RTTI* type
    /// @return T/F if param == Scope
    bool Scope::Equals(const RTTI* rhs) const
    {
        const Scope* other = rhs->As<Scope>();
        if (other == nullptr) return false;
        return *this == *other;
    }
}