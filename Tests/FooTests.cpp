#include "Tests.h"
#include "Support/Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(FooTests)
    {
        TEST_MEMCHECK;

        TEST_METHOD(Constructor)
        {
            const Foo a;
            Assert::AreEqual(0, a.Data());

            const Foo b(10);
            Assert::AreEqual(10, b.Data());
        }

        TEST_METHOD(CopySemantics)
        {
            const Foo a(10);
            Assert::AreEqual(10, a.Data());

            const Foo b(a);
            Assert::AreEqual(a, b);

            //Foo c;
            //c = a;
            //Assert::AreEqual(c, a);
        }

        TEST_METHOD(EqualityOperators)
        {
            const Foo a(10);
            const Foo b(a);

            // Assert::AreNotEqual does not invoke operator!=
            Assert::AreEqual(a, b);
            Assert::IsTrue(a == b);
            Assert::IsFalse(a != b);

            const Foo c;
            Assert::AreNotEqual(a, c);
            Assert::IsTrue(a != c);
            Assert::IsFalse(a == c);
        }

        TEST_METHOD(SetData)
        {
            Foo a;
            Assert::AreEqual(0, a.Data());

            const int data = 10;
            a.SetData(data);
            Assert::AreEqual(data, a.Data());
        }

        TEST_METHOD(RTTIFuncs)
        {
            //Don't call w/ () for some reason?
            Foo foo;

            {
                Foo foo2(27);
                //Static functions can't be called by an object. Call by class type instead
                string typeStr = Foo::TypeName();
                string strType = Foo::TypeName();
                RTTI::IdType fooId = foo.TypeIdInstance();
                RTTI::IdType foo2Id = foo2.TypeIdInstance();
                RTTI::IdType wrongId = 0;

                Assert::AreEqual(typeStr, strType);
                Assert::AreEqual(fooId, foo2Id);
                Assert::IsTrue(foo.Is(fooId));
                Assert::IsFalse(foo.Is(wrongId));

                Assert::IsTrue(foo.Equals(static_cast<RTTI*>(&foo2)) == false);
                //Assert::IsTrue(foo.Equals(static_cast<RTTI*>(nullptr)) == false);
                foo.SetData(27);
                Assert::IsTrue(foo.Equals(static_cast<RTTI*>(&foo2)) == true);

                Assert::AreEqual(typeStr + "(27)\n", foo.ToString());
                Assert::AreEqual(typeStr + "(27)\n", foo2.ToString());
                foo2.SetData(45);
                Assert::AreNotEqual(typeStr + "(45)\n", foo.ToString());
                Assert::AreNotEqual(typeStr + "(27)\n", foo2.ToString());
            }
        }
    };
}