#pragma once
#include "FieaGameEngine/Types.h"
#include <cctype>
#include <stdlib.h>
//Use isdigit() when data string isn't a number
//Use atoi to convert back into a string
namespace Fiea::Engine
{
    class RTTI;
}

namespace Fiea::Engine::Content
{
    class Scope;

    using intInitList = std::initializer_list<int32_t>;
    using floatInitList = std::initializer_list<float>;
    using strInitList = std::initializer_list<string>;
    using rttiInitList = std::initializer_list<RTTI*>;
    using scopeInitList = std::initializer_list<Scope*>;

    class Datum
    {
    public:
        Datum();
        Datum(const Datum&);
        Datum(Datum&&) noexcept;
        Datum& operator=(const Datum&);
        Datum& operator=(Datum&&) noexcept;
        ~Datum();

        Datum(const int32_t&);
        Datum(const float&);
        Datum(const string&);
        Datum(RTTI* const);
        Datum(Scope* const);

        Datum(intInitList il);
        Datum(floatInitList il);
        Datum(strInitList il);
        Datum(rttiInitList il);
        Datum(scopeInitList il);

        int32_t& operator=(const int32_t&);
        float& operator=(const float&);
        string& operator=(const string&);
        RTTI* operator=(RTTI* const);
        Scope* operator=(Scope* const);

        // Datum "types"
        enum Type 
        {
            None = 0,
            Int,
            Float,
            String,
            Pointer,
            ScopeTable
        }; // represents what type each Datum object is going to use
        void SetType(Type); // simple setter
        bool IsType(Type) const; // we'll offer this, instead of a getter because the only anticipated operation would be the compare

        //    - In this section, where you see TYPE, you may attempt to use templates for these methods, or you may provide overloads
        void SetInt(const int32_t& val, size_t idx = 0); // just sets a value at an index, may NOT grow the size!
        int32_t& GetInt(size_t idx = 0); // simple mutable getter. if you are not using templates, use function names GetAs*, where * is a Datum::Type, like GetAsInt
        const int32_t& GetInt(size_t idx = 0) const; // simple const getter
        void PushInt(const int32_t& val); // pushes a new value to the back on the datum's storage, grows the size and CAN grow the capacity
        void PopInt(); // safely remove the last item from the datum's storage, including destruction (if necessary)

        void SetFloat(const float& val, size_t idx = 0); // just sets a value at an index, may NOT grow the size!
        float& GetFloat(size_t idx = 0); // simple mutable getter. if you are not using templates, use function names GetAs*, where * is a Datum::Type, like GetAsInt
        const float& GetFloat(size_t idx = 0) const; // simple const getter
        void PushFloat(const float& val); // pushes a new value to the back on the datum's storage, grows the size and CAN grow the capacity
        void PopFloat(); // safely remove the last item from the datum's storage, including destruction (if necessary)


        void SetString(const string& val, size_t idx = 0); // just sets a value at an index, may NOT grow the size!
        string& GetString(size_t idx = 0); // simple mutable getter. if you are not using templates, use function names GetAs*, where * is a Datum::Type, like GetAsInt
        const string& GetString(size_t idx = 0) const; // simple const getter
        void PushString(const string& val); // pushes a new value to the back on the datum's storage, grows the size and CAN grow the capacity
        void PopString(); // safely remove the last item from the datum's storage, including destruction (if necessary)
        void ClearString(); // safely remove ALL items from the datum's storage


        void SetRTTI(RTTI* const val, size_t idx = 0); 
        RTTI* GetRTTI(size_t idx = 0); 
        RTTI* const GetRTTI(size_t idx = 0) const;
        void PushRTTI(RTTI* const val); 
        void PopRTTI(); 
        size_t IndexOfRTTI(RTTI& rtti);
        void RemoveRTTIAt(size_t idx);


        void SetScope(Scope* const val, size_t idx = 0);
        Scope* GetScope(size_t idx = 0);
        Scope* const GetScope(size_t idx = 0) const;
        void PushScope(Scope* const val);
        void PopScope();
        size_t IndexOfScope(Scope& scope);
        void RemoveScopeAt(size_t idx);


        // string based modifiers
        void SetIntFromString(const string&, size_t idx = 0);
        string GetIntToString(size_t idx = 0) const;
        void PushIntFromString(const string&);
        void SetFloatFromString(const string&, size_t idx = 0);
        string GetFloatToString(size_t idx = 0) const;
        void PushFloatFromString(const string&);

        size_t Size() const;
        size_t Capacity() const;

        bool operator==(const Datum&) const;
        bool operator!=(const Datum&) const;

        //function to accept external storage
        void SetStorage(int32_t* items, size_t count = 1);
        void SetStorage(float* items, size_t count = 1);
        void SetStorage(string* items, size_t count = 1);
        void SetStorage(RTTI** items, size_t count = 1);
        void SetStorage(Scope** items, size_t count = 1);
        void SetStorage(void* extField, Type enumType, size_t count = 1);
        //void SetStorage(Type types, size_t count = 1);

        bool IsExternal() const;
    private:
        union DatumTypes
        {
            void* _Void;
            int32_t* _Digits;
            float* _Rationals;
            string* _Words;
            RTTI** _Inheriter;
            Scope** _Scopes;
        };
        DatumTypes _Data;
        size_t _Size;
        size_t _Capacity;
        Type _Tag;
        bool _External;
    };
}