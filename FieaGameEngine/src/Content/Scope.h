#pragma once
#include "FieaGameEngine/Types.h"
#include "Datum.h"
#include "../../include/FieaGameEngine/FactoryService.h"
#include <unordered_map>
#include <vector>

/*
Requirements

Scopes may contain nested Scopes
    If a Scope has Scope children, it OWNS them
        When copying / moving, children should be copied / moved appropriately
        When destroying, children should be destroyed
    If a Scope has no parent (we'll call this a "Root Scope", it is owned by the code that created it
    Creation of a Table - type Datum can only happen through a Scope method below
        If a Scope is referenced from a Datum, it MUST have a parent that contains that Datum
        If a Scope is a Root Scope, then it may not be referenced by a Datum
    Parentage is NOT a part of equivalence
        Two Scopes with different parentage may be considered Equal if they contain Datums with the same values
        When copying or moving a Scope, parentage is NOT transferred!

API

Update any methods, like Clear, which need to do additional work to meet these requirements
If you require additional functionality from the Datum class, implement in the Datum class
    Do not do "interesting" work on Datums directly within Scope code
    For example, you may require Datum::IndexOf and /or Datum::RemoveAt to implement some of the methods below
    Consider whether to add methods to Datum's public API, or whether some should be private, with Scope as a friend class?
*/


namespace Fiea::Engine::Content
{
    using datumVect = std::vector<Datum*>;
    using datumMap = std::unordered_map<std::string, Datum>;

    class Scope : public RTTI
    {
    public:
        explicit Scope(size_t capacity = 0);

        //Rule of 5 API - Why default?
        Scope(const Scope& other);
        Scope& operator=(const Scope& other);
        Scope(Scope&& rhs) noexcept;
        Scope& operator=(Scope&& rhs) noexcept;
        ~Scope();

        //Other functions
        bool operator==(const Scope& rhs) const;
        bool operator!=(const Scope& rhs) const;
        Datum* Find(const std::string& key); // return the Datum at the requested key, or nullptr if not found
        const Datum* Find(const std::string& key) const;
        Datum& Append(const std::string& key); // return a reference to the Datum at this key. create a new Datum, if one is not found
        Datum& operator[](const std::string& key); // syntactic sugar to wrap Append
        Datum& operator[](size_t idx); // retrieve a Datum by index. out-of-range requests do not append
        const Datum& operator[](size_t idx) const;
        size_t Size() const; // the number of key->Datum pairs currently held
        void Clear(); // get rid of all contained key->Datum pairs, including any necessary destruction

        //Part 4 functions
        // for each of these methods, use a Table Datum, if one exists. Otherwise, create it
        Scope& AppendScope(const std::string& key, Scope* scope = nullptr); // Add a reference to the provided Scope to a Table-type Datum at 'key'
        void Adopt(const std::string& key, Scope& child); // Assume parentage of the provided child

        Scope* GetParent() const;
        [[nodiscard]] Scope* Orphan(); // remove parentage and return the pointer to the caller, since they will now own this Root Scope

        // search (non-recursive) for a nested Scope which is the SAME (not just equal) as the provided scope and return the Datum holding it.
        // Populate idx with the index within the Datum
        Datum* FindContainedScope(const Scope& child, size_t& idx);
        const Datum* FindContainedScope(const Scope& child, size_t& idx) const;

        // hint: one of these should be easy to implement... is there some way to implement the tougher one by using the easier one?
        bool IsAncestorOf(const Scope& descendent) const;
        bool IsDescendentOf(const Scope& ancestor) const;

        //DA CLONE FUNCTION!!!!
        virtual Scope* Clone() const; // in Scope class

        //RTTI virtual functions
        string ToString() const override;
        bool Equals(const RTTI* rhs) const override;

        RTTI_DECLARATIONS(Scope, RTTI);
    private:
        void SetParent(Scope& parent); // implement as PRIVATE, and use internally when changing the parent to ensure that 
        //  parent may be safely set (current parent is null, new parent not a child of this Scope, etc)

        //Why are these owned by a class? 
        // B/c Scope doesn't need references. Using vals allows us to use default w/ no downsides.
        // Also, they'll get constructed before our main constructor enters its scope.

        //If datumMap needs a dynamic Datum, I'll need to make the RO5 functions for Scope
        datumMap _DatumMap;
        datumVect _ValVect;
        Scope* _Parent;
        size_t _Size;
        size_t _Capacity;
    };
    FACTORY_DECLARE(Scope, Scope);

}