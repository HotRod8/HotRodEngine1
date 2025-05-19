#include "Tests.h"
#include "FieaGameEngine/List.h"
//#include <list>
#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using StringList = Fiea::Engine::List<std::string>;
//using StringList = std::list<string>;

// Explicit specialization for StringList (std::list<std::string>)
//template <>
//inline std::wstring ToString<const StringList>(const StringList& list)
//{
//    std::wostringstream oss;
//    oss << L"List<string> with " << list.size() << L" elements: [";
//    for (const auto& elem : list)
//    {
//        oss << std::wstring(elem.begin(), elem.end()) << L", ";
//    }
//    std::wstring result = oss.str();
//    if (list.size() > 0)
//    {
//        result.erase(result.size() - 2); // Remove trailing ", "
//    }
//    result += L"]";
//    return result;
//}
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<StringList>(const StringList& t)
    {
        //StringList lists;
        //for (auto it : t) {

        //}
        RETURN_WIDE_STRING("List<string>, " << t.size() << " elements");
    }

    template<>
    inline std::wstring ToString<StringList>(StringList* t)
    {
        //StringList lists;
        //for (auto it : *t) {

        //}
        RETURN_WIDE_STRING("List<string>, " << t->size() << " elements");
    }

    template<>
    inline std::wstring ToString<StringList>(const StringList* t)
    {
        //StringList lists;
        //for (auto it : *t) {

        //}
        RETURN_WIDE_STRING("List<string>, " << t->size() << " elements");
    }
}

//If you want to do something akin to printf, use Logger::WriteMessage to redirect output to the Debug log.
namespace Fiea::Engine::Tests
{
    TEST_CLASS(ListTests)
    {
        TEST_MEMCHECK;

        // StringList
        TEST_METHOD(Constructor)
        {
            //For default constructor, simply check if it has 0 elements or not
            StringList a;
            Assert::AreEqual(0_z, a.size());
            Assert::IsTrue(a.empty());
        }

        TEST_METHOD(CopySemantics)
        {
            string greetinglist[] = { "Hello!", "Greetings!", "Wassup!", "Howya doin'!" };
            StringList a;
            for (size_t i = 0; i < 4_z; ++i) {
                a.push_back(greetinglist[i]);
            }
            Assert::AreEqual(4_z, a.size());

            //Tests if one list object is a copy of another list object
            StringList b(a);
            Assert::IsTrue(a.size() == b.size());
            Assert::AreEqual(4_z, b.size());
            //Assert::AreSame(a, b);
            for(StringList::Iterator ita = a.begin(), itb = b.begin(); ita != a.end() && itb != b.end(); ++ita, ++itb)
            {
                //Check if values are properly copied over
                Assert::AreEqual(*ita, *itb);
            }

            Assert::AreSame(b, b);

            string hello = "Hello!";
            StringList c;
            c.push_back(hello);

            StringList d = c;
            Assert::IsTrue(c.size() == d.size());
            Assert::AreEqual(1_z, c.size());
            Assert::AreNotSame(c.back(), d.back());
            Assert::AreEqual(c.back(), d.back());

            //To make sure copying the value really works, we need to test it with the return type of an assignment operation as well
            StringList e = d = c;
            Assert::IsTrue(c.size() == e.size());
            Assert::AreEqual(1_z, e.size());
            Assert::AreNotSame(c.back(), e.back());
            Assert::AreEqual(c.back(), e.back());
        }

        TEST_METHOD(EqualityOperators)
        {
            string wordlist[] = {"Brown", "Orange", "Red", "Turquoise"};
            StringList a;
            for (size_t i = 0; i < 4_z; ++i) {
                a.push_back(wordlist[i]);
            }

            int i = 0;
            for (StringList::Iterator ita = a.begin(); ita != a.end(); ++ita, ++i)
            {
                //Check if values are properly copied over
                Assert::AreEqual(wordlist[i], *ita);
            }

            StringList b;
            b = a;
            for (StringList::Iterator ita = a.begin(), itb = b.begin(); ita != a.end() && itb != b.end(); ++ita, ++itb)
            {
                //Check if values are properly copied over
                Assert::AreEqual(*ita, *itb);
                Assert::IsTrue(*ita == *itb);
                Assert::IsFalse(*ita != *itb);
            }

            StringList c, d;
            d = c;
            for (StringList::Iterator itd = d.begin(); itd != d.end(); ++itd)
            {
                //If it comes into the loop, the code is bugged.
                Assert::AreEqual(0_z, d.size());
            }

            d = b = a;
            for (StringList::Iterator ita = a.begin(), itb = d.begin(); ita != a.end() && itb != d.end(); ++ita, ++itb)
            {
                //Check if values are properly copied over
                Assert::AreEqual(*ita, *itb);
                Assert::IsTrue(*ita == *itb);
                Assert::IsFalse(*ita != *itb);
            }
        }

        //Needs work with future test cases
        TEST_METHOD(Destructor)
        {
            size_t zilch = 0;
            StringList a; 
            Assert::AreEqual(zilch, a.size());
        }

        TEST_METHOD(Iterators) 
        {
            string wordlist[] = { "Brown", "Orange", "Red", "Turquoise" };
            StringList a;
            for (size_t i = 0; i < 4_z; ++i) {
                a.push_back(wordlist[i]);
            }
            //Test List::iterator begin()
            //Test List::iterator end()
            size_t i = 0;
            for (StringList::Iterator ita = a.begin(); ita != a.end(); ++ita, ++i)
            {
                //Check if values are properly copied over
                Assert::AreEqual(wordlist[i], *ita);
                Assert::IsTrue(wordlist[i] == *ita);
                Assert::IsFalse(wordlist[i] != *ita);
            }
        }

        //Create failure cases in case of an empty list later
        TEST_METHOD(Modifiers)
        {
            string newcolor = "Blue";
            StringList list;

            //Test void push_back(const value_type& value)
            list.push_back(newcolor);
            Assert::AreEqual(newcolor, list.back());
            Assert::IsTrue(newcolor == list.back());
            Assert::IsFalse(newcolor != list.back());
            //Test void pop_back()
            list.pop_back();
            //ExpectSEH<EXCEPTION_ACCESS_VIOLATION>([list] { list.back(); });
            
            //Assert::IsTrue(newcolor != list.back());
            //Assert::IsFalse(newcolor == list.back());
            ////list.pop_back();

            ////Test void push_front(const value_type& value)
            //list.push_front(newcolor);
            //Assert::AreEqual(newcolor, list.front());
            //Assert::IsTrue(newcolor == list.front());
            //Assert::IsFalse(newcolor != list.front());
            ////Test void pop_front()
            //list.pop_front();
            //Assert::AreNotEqual(newcolor, list.front());
            //Assert::IsTrue(newcolor != list.front());
            //Assert::IsFalse(newcolor == list.front());
            //list.pop_front();
        }

        //Create failure cases in case of an empty list later
        TEST_METHOD(Accessors)
        {
            string wordarray[] = { "Brown", "Orange", "Red", "Turquoise", "Blue" };
            StringList list;
            for (size_t i = 0; i < 5_z; ++i)
            {
                list.push_back(wordarray[i]);
            }
            const StringList constlist = list;
            //Test value_type& back()
            Assert::AreEqual(5_z, list.size());
            if (!list.empty()) { Assert::AreEqual(wordarray[5_z - 1], list.back()); }
            //Test const value_type& back() const
            Assert::AreEqual(5_z, constlist.size());
            if (!constlist.empty()) { Assert::AreEqual(wordarray[5_z - 1], constlist.back()); }
            //Test value_type& front()
            Assert::AreEqual(5_z, list.size());
            if (!list.empty()) { Assert::AreEqual(wordarray[0], list.front()); }
            //Test const value_type& front() const
            Assert::AreEqual(5_z, constlist.size());
            if (!constlist.empty()) { Assert::AreEqual(wordarray[0], constlist.front()); }
        }

        
        TEST_METHOD(Info)
        {
            string colorarray[] = { "Brown", "Orange", "Red", "Turquoise", "Blue" };
            StringList list;
            for (size_t i = 0; i < 5_z; ++i)
            {
                list.push_back(colorarray[i]);
            }
            //Test size_type size() const
            //Test bool empty() const
            Assert::AreEqual(5_z, list.size());
            Assert::IsFalse(list.empty());

            for (size_t i = 0; i < 5_z; ++i)
            {
                list.pop_back();
            }
            Assert::AreEqual(0_z, list.size());
            Assert::IsTrue(list.empty());
        }

        // StringList::iterator
        TEST_METHOD(Dereference)
        {
            string wordlist[] = { "Brown", "Orange", "Red", "Turquoise" };
            StringList a;
            for (size_t i = 0; i < 4_z; ++i) {
                a.push_back(wordlist[i]);
            }
            const StringList b = a;
            //Test const reference operator*() const
            //Test const pointer operator->() const;
            int i = 0;
            for (StringList::Iterator ita = a.begin(); ita != a.end(); ++ita, ++i)
            {
                //Check if values are properly copied over
                Assert::AreEqual(wordlist[i], *ita);
                //Test -> w/ String appends to the data 
                ita->append(" color!");
                Assert::AreEqual(wordlist[i] + " color!", *ita);
            }

            //Test reference operator*();
            //Test pointer operator->();
            i = 0;
            for (StringList::Iterator itb = b.begin(); itb != b.end(); ++itb, ++i)
            {
                //Check if values are properly copied over
                Assert::AreEqual(wordlist[i], *itb);
                //Test -> w/ String appends to the data 
                itb->append(" car!");
                Assert::AreEqual(wordlist[i] + " car!", *itb);
            }
        }

        TEST_METHOD(Navigation) 
        {
            string wordarray[] = { "Brown", "Orange", "Red", "Turquoise", "Blue" };
            StringList list;
            for (size_t i = 0; i < 5_z; ++i){
                list.push_back(wordarray[i]);
            }
            StringList::Iterator ita = list.begin();
            //Test iterator& operator++()
            ++ita;
            ++ita;
            Assert::AreEqual(wordarray[2], *ita);
            //Test iterator operator++(int)
            StringList::Iterator itb = ita;
            itb++;
            itb++;
            Assert::AreEqual(wordarray[4], *itb);
            //Test iterator& operator--()
            StringList::Iterator itc = itb;
            --itc;
            Assert::AreEqual(wordarray[3], *itc);
            --ita;
            Assert::AreEqual(wordarray[1], *ita);
            //Test iterator operator--(int)
            itc--;
            Assert::AreEqual(wordarray[2], *itc);
            itb--;
            Assert::AreEqual(wordarray[3], *itb);
        }

        TEST_METHOD(Comparators)
        {
            string wordlist[] = { "Brown", "Orange", "Red", "Turquoise" };
            StringList a;
            for (size_t i = 0; i < 4_z; ++i) {
                a.push_back(wordlist[i]);
            }
            //Test bool operator==(const iterator&, const iterator&)
            //Test bool operator!=(const iterator&, const iterator&)
            StringList b = a;
            for (StringList::Iterator ita = a.begin(), itb = b.begin(); ita != a.end() && itb != b.end(); ++ita, ++itb)
            {
                //Check if values are properly copied over
                Assert::AreEqual(*ita, *itb);
                Assert::IsTrue(*ita == *itb);
                Assert::IsFalse(*ita != *itb);
            }
        }
    };
}