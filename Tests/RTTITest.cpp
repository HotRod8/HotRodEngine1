#include "Support/Foo.h"
#include "Support/SupportClass.h"
#include "Tests.h"
using namespace Fiea::Engine::Tests::Support;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(RTTI_Tester)
    {
        TEST_MEMCHECK;

        TEST_METHOD(IsAndAs)
        {
            //Can't make an RTTI object b/c it's a pure virtual class
            Foo foo(190);
            const Foo foo2(55);
            EmptyChild orphan(32);
            const EmptyChild orfan(152);

            RTTI::IdType fooId = foo.TypeIdInstance();
            RTTI::IdType foo2Id = foo2.TypeIdInstance();
            RTTI::IdType orphanId = orphan.TypeIdInstance();
            RTTI::IdType orfanId = orfan.TypeIdInstance();
            RTTI::IdType wrongId = 0;

            Assert::IsTrue(foo.Is(fooId) == true);
            Assert::IsTrue(foo.Is(wrongId) == false);
            Assert::IsTrue(foo2.Is(foo2Id) == true);
            Assert::IsTrue(foo2.Is(wrongId) == false);
            Assert::IsTrue(orphan.Is(orphanId) == true);
            Assert::IsTrue(orphan.Is(wrongId) == false);
            Assert::IsTrue(orfan.Is(orfanId) == true);
            Assert::IsTrue(orfan.Is(wrongId) == false);

            Assert::IsNotNull(foo.As<Foo>());
            Assert::IsNotNull(foo2.As<Foo>());
            Assert::AreEqual(foo.As<Foo>(), &foo);
            Assert::AreEqual(foo2.As<Foo>(), &foo2);
            Assert::IsNotNull(orphan.As<EmptyChild>());
            Assert::IsNotNull(orfan.As<EmptyChild>());
            Assert::AreEqual(orphan.As<EmptyChild>(), &orphan);
            Assert::AreEqual(orfan.As<EmptyChild>(), &orfan);

        }
        TEST_METHOD(EqualsAndToString)
        {
            Foo foo(190);
            const Foo foo2(55);
            EmptyChild orphan(95);
            const EmptyChild orfan(152);

            string fooStr = Foo::TypeName();
            string strFoo = Foo::TypeName();
            string orphanStr = EmptyChild::TypeName();
            string orfanStr = EmptyChild::TypeName();
            string tempStr = "";
            string tempStr2 = "";

            Assert::IsTrue(foo.Equals(&foo2) == false);
            foo.SetData(55);
            Assert::IsTrue(foo2.Equals(&foo) == true);
            Assert::IsTrue(orphan.Equals(&orfan) == false);
            Assert::IsTrue(orfan.Equals(&orphan) == false);

            Assert::AreEqual(fooStr.c_str(), strFoo.c_str());
            Assert::AreEqual(orphanStr.c_str(), orfanStr.c_str());
            tempStr += fooStr + "(55)\n";

            Assert::AreEqual(tempStr.c_str(), foo.ToString().c_str());
            Assert::AreEqual(tempStr.c_str(), foo2.ToString().c_str());
            tempStr.clear();
            tempStr += fooStr + "(45)\n";
            tempStr2 += fooStr + "(27)\n";
            Assert::AreNotEqual(tempStr.c_str(), foo.ToString().c_str());
            Assert::AreNotEqual(tempStr2.c_str(), foo2.ToString().c_str());
            
            Assert::AreNotEqual(tempStr.c_str(), orphan.ToString().c_str());
            Assert::AreNotEqual(tempStr2.c_str(), orfan.ToString().c_str());
            tempStr.clear();
            tempStr2.clear();
            tempStr += orphanStr + "(95)\n";
            tempStr2 += orphanStr + "(152)\n";
            Assert::IsTrue(strcmp(tempStr.c_str(), orphan.ToString().c_str()) == 0);
            Assert::IsTrue(strcmp(tempStr2.c_str(), orfan.ToString().c_str()) == 0);
        }
    };
}

#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<Fiea::Engine::Tests::Support::EmptyChild>(const Fiea::Engine::Tests::Support::EmptyChild& t)
    {
        RETURN_WIDE_STRING("EmptyChild: " << t.GetValue());
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Tests::Support::EmptyChild>(const Fiea::Engine::Tests::Support::EmptyChild* t)
    {
        RETURN_WIDE_STRING("EmptyChild: " << t->GetValue());
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Tests::Support::EmptyChild>(Fiea::Engine::Tests::Support::EmptyChild* t)
    {
        RETURN_WIDE_STRING("EmptyChild: " << t->GetValue());
    }
}