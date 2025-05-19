#include "../../Tests/Support/FooManager.h"
#include "../../Tests/Support/Foo.h"
#include <unordered_map>

namespace Fiea::Engine
{
    using Foo = Tests::Support::Foo;
    using map = std::unordered_map<string, Foo>;

    //This manager's job is to make a Foo to hold ints given with Make and return them (by name) in Take
    class FooManager : public IFooMaker, public IFooTaker
    {
    public:
        /// @brief - Creates a new <string, Foo> pair
        /// @param name - string name (pair key)
        /// @param value - data for Foo obj (pair val)
        void Make(const char* name, int32_t value) override
        {
            Foo f(value);
            string s(name);
            foomap.emplace(s, f);
        }
        /// @brief - Gives back any Foo data relating the key name
        /// @param name - pair key
        /// @return - number data inside the Foo object
        int32_t Take(const char* name) override
        {
            string s(name);
            map::iterator itr = foomap.find(s);
            if(itr == foomap.end())
            {
                throw std::out_of_range("Foo not found");
            }
            return itr->second.Data();
        }
    private:
        map foomap;
        FIEA_SERVICE_BIND2(FooManager, IFooMaker, IFooTaker);
    };
}