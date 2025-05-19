#include "../FieaGameEngine/src/Content/Scope.h"
#include "Support/Foo.h"
#include "FieaGameEngine/Types.h"
#include "Tests.h"

namespace Fiea::Engine::Tests
{
    using Scope = Fiea::Engine::Content::Scope;
    using Fiea::Engine::Content::Datum;

    TEST_CLASS(ScopeTester)
    {
        //TEST_MEMCHECK;
        TEST_METHOD(DefaultAndRO5)
        {
            {
                Scope scp1;
                Scope scp2(29);

                Assert::AreNotEqual(scp1, scp2);
                Assert::IsFalse(scp1 == scp2);
                Assert::IsTrue(scp1 != scp2);
            }

            {
                string str1 = "ints", str2 = "floats", str3 = "strings", str4 = "Any inheritable thing", str5 = "Scopes";
                int32_t ints[] = {1, 2, 3};
                float floats[] = {1.0f, 2.0f, 3.0f};
                string strings[] = {"one", "two", "three"};
                Foo foo1(10), foo2(25), foo3(1010);
                Foo* foos[] = {&foo1, &foo2, &foo3};
                //Foo foo(10);

                Scope scp2(29);
                Scope scp1 = scp2;

                Assert::AreEqual(scp1, scp2);
                Assert::IsFalse(scp1 != scp2);
                Assert::IsTrue(scp1 == scp2);

                Scope scp3, scp4, scp5;
                Scope* scps[] = { &scp1, &scp2, &scp3, &scp4, &scp5 };
                Datum& dat1 = scp1[str1];
                Datum& dat2 = scp2[str2];
                Datum& dat3 = scp3[str3];
                Datum& dat4 = scp4[str4];
                Datum& dat5 = scp5[str5];

                dat1.SetStorage(ints);
                dat2.SetStorage(floats);
                dat3.SetStorage(strings);
                dat4.SetStorage(reinterpret_cast<RTTI**>(foos));
                dat5.SetStorage(scps);

                //dat1.SetType(Datum::Type::Int);
                //dat2.SetType(Datum::Type::Float);
                //dat3.SetType(Datum::Type::String);
                //dat4.SetType(Datum::Type::Pointer);
                //dat5.SetType(Datum::Type::ScopeTable);

                //for (size_t i = 0; i < 3; ++i)
                //{
                //    dat1.PushInt(91 + (int32_t)i);
                //    dat2.PushFloat(50.0f + (float)i);
                //    dat3.PushString("Thanks.");
                //    dat4.PushRTTI(reinterpret_cast<RTTI*>(&foo));
                //}
                //scp5.Adopt(str5, scp4);
                //scp5.Adopt(str5, scp3);
                //scp5.Adopt(str5, scp2);

                //Assert::IsTrue(dat5.GetScope(0) == &scp4);
                //Assert::IsTrue(dat5.GetScope(1) == &scp3);
                //Assert::IsTrue(dat5.GetScope(2) == &scp2);

                for(size_t i = 0; i < 3; ++i)
                {
                    Assert::AreEqual(dat1.GetInt(i), ints[i]);
                    Assert::AreEqual(dat2.GetFloat(i), floats[i]);
                    Assert::AreEqual(dat3.GetString(i), strings[i]);
                    Assert::AreEqual(reinterpret_cast<Foo*>(dat4.GetRTTI(i)), foos[i]);
                }
                for (size_t i = 0; i < dat5.Size(); ++i)
                {
                    Assert::AreEqual(dat5.GetScope(i), scps[i]);
                }
            }

            //{
            //    string str1 = "ints", str2 = "floats", str3 = "strings", str4 = "Any inheritable thing", str5 = "Scopes";
            //    int32_t ints[] = { 1, 2, 3 };
            //    float floats[] = { 1.0f, 2.0f, 3.0f };
            //    string strings[] = { "one", "two", "three" };
            //    Foo foo1(10), foo2(25), foo3(1010);
            //    Foo* foos[] = { &foo1, &foo2, &foo3 };

            //    Scope scp1;
            //    Scope scp2(29);
            //    scp1 = scp2;

            //    Assert::AreEqual(scp1, scp2);
            //    Assert::IsFalse(scp1 != scp2);
            //    Assert::IsTrue(scp1 == scp2);

            //    Scope scp3, scp4, scp5;
            //    Scope* scps[] = { &scp1, &scp2, &scp3, &scp4, &scp5 };
            //    Datum& dat1 = scp1[str1];
            //    Datum& dat2 = scp2[str2];
            //    Datum& dat3 = scp3[str3];
            //    Datum& dat4 = scp4[str4];
            //    Datum& dat5 = scp5[str5];

            //    dat1.SetStorage(ints);
            //    dat2.SetStorage(floats);
            //    dat3.SetStorage(strings);
            //    dat4.SetStorage(reinterpret_cast<RTTI**>(foos));
            //    dat5.SetStorage(scps);

            //    for (size_t i = 0; i < 3; ++i)
            //    {
            //        Assert::AreEqual(dat1.GetInt(i), ints[i]);
            //        Assert::AreEqual(dat2.GetFloat(i), floats[i]);
            //        Assert::AreEqual(dat3.GetString(i), strings[i]);
            //        Assert::AreEqual(reinterpret_cast<Foo*>(dat3.GetRTTI(i)), foos[i]);
            //    }
            //    for (size_t i = 0; i < dat5.Size(); ++i)
            //    {
            //        Assert::AreEqual(dat5.GetScope(i), scps[i]);
            //    }

            //}

            //{
            //    string str1 = "ints", str2 = "floats", str3 = "strings", str4 = "Any inheritable thing", str5 = "Scopes";
            //    int32_t ints[] = { 1, 2, 3 };
            //    float floats[] = { 1.0f, 2.0f, 3.0f };
            //    string strings[] = { "one", "two", "three" };
            //    Foo foo1(10), foo2(25), foo3(1010);
            //    Foo* foos[] = { &foo1, &foo2, &foo3 };

            //    Scope scp2(29);
            //    Scope scp1 = std::move(scp2);
            //    Scope scp3 = scp1;

            //    Assert::AreEqual(scp1, scp3);
            //    Assert::IsFalse(scp1 != scp3);
            //    Assert::IsTrue(scp1 == scp3);

            //    Scope scp4, scp5;
            //    Scope* scps[] = { &scp1, &scp2, &scp3, &scp4, &scp5 };
            //    Datum& dat1 = scp1[str1];
            //    Datum& dat2 = scp2[str2];
            //    Datum& dat3 = scp3[str3];
            //    Datum& dat4 = scp4[str4];
            //    Datum& dat5 = scp5[str5];

            //    dat1.SetStorage(ints);
            //    dat2.SetStorage(floats);
            //    dat3.SetStorage(strings);
            //    dat4.SetStorage(reinterpret_cast<RTTI**>(foos));
            //    dat5.SetStorage(scps);

            //    for (size_t i = 0; i < 3; ++i)
            //    {
            //        Assert::AreEqual(dat1.GetInt(i), ints[i]);
            //        Assert::AreEqual(dat2.GetFloat(i), floats[i]);
            //        Assert::AreEqual(dat3.GetString(i), strings[i]);
            //        Assert::AreEqual(reinterpret_cast<Foo*>(dat3.GetRTTI(i)), foos[i]);
            //    }
            //    for (size_t i = 0; i < dat5.Size(); ++i)
            //    {
            //        Assert::AreEqual(dat5.GetScope(i), scps[i]);
            //    }

            //}

            //{
            //    string str1 = "ints", str2 = "floats", str3 = "strings", str4 = "Any inheritable thing", str5 = "Scopes";
            //    int32_t ints[] = { 1, 2, 3 };
            //    float floats[] = { 1.0f, 2.0f, 3.0f };
            //    string strings[] = { "one", "two", "three" };
            //    Foo foo1(10), foo2(25), foo3(1010);
            //    Foo* foos[] = { &foo1, &foo2, &foo3 };

            //    Scope scp1;
            //    Scope scp2(29);
            //    scp1 = std::move(scp2);
            //    Scope scp3 = scp1;

            //    Assert::AreEqual(scp1, scp3);
            //    Assert::IsFalse(scp1 != scp3);
            //    Assert::IsTrue(scp1 == scp3);

            //    Scope scp4, scp5;
            //    Scope* scps[] = { &scp1, &scp2, &scp3, &scp4, &scp5 };
            //    Datum& dat1 = scp1[str1];
            //    Datum& dat2 = scp2[str2];
            //    Datum& dat3 = scp3[str3];
            //    Datum& dat4 = scp4[str4];
            //    Datum& dat5 = scp5[str5];

            //    dat1.SetStorage(ints);
            //    dat2.SetStorage(floats);
            //    dat3.SetStorage(strings);
            //    dat4.SetStorage(reinterpret_cast<RTTI**>(foos));
            //    dat5.SetStorage(scps);

            //    for (size_t i = 0; i < 3; ++i)
            //    {
            //        Assert::AreEqual(dat1.GetInt(i), ints[i]);
            //        Assert::AreEqual(dat2.GetFloat(i), floats[i]);
            //        Assert::AreEqual(dat3.GetString(i), strings[i]);
            //        Assert::AreEqual(reinterpret_cast<Foo*>(dat3.GetRTTI(i)), foos[i]);
            //    }
            //    for (size_t i = 0; i < dat5.Size(); ++i)
            //    {
            //        Assert::AreEqual(dat5.GetScope(i), scps[i]);
            //    }
            //}
        }
        TEST_METHOD(Comparisons)
        {
            {
                string str1 = "num1";
                string str2 = "num2";
                string str3 = "num3";
                Scope scp1;
                Scope scp2;

                Assert::AreNotSame(scp1, scp2);
                Assert::AreEqual(scp1, scp2);

                //Test failure for comparisons
                scp1[str1];
                scp1[str2];
                scp1[str3];
                Assert::AreNotEqual(scp1, scp2);
                Assert::IsTrue(scp1 != scp2);
                scp2[str3];
                scp2[str2];
                scp2[str1];
                Assert::AreNotEqual(scp1, scp2);
                Assert::IsFalse(scp1 == scp2);

                //Test multiple objects as a successfull case
                scp2.Clear();
                scp2[str1];
                scp2[str2];
                scp2[str3];
                Assert::AreEqual(scp1, scp2);
                Assert::IsTrue(scp1 == scp2);
                Assert::IsFalse(scp1 != scp2);
            }
        }
        TEST_METHOD(Append)
        {
            {
                string str1 = "num1";
                Scope scp1, scp2;
                Datum dat1, dat2;

                Assert::AreEqual(scp1, scp2);
                Assert::AreNotSame(scp1, scp2);
                Assert::IsFalse(scp1 != scp2);
                Assert::IsTrue(scp1 == scp2);

                //Append a string into Scope data w/ empty initiallization 
                dat1 = scp1.Append(str1);
                Assert::AreEqual(scp1.Size(), 1_z);
                dat2 = scp2.Append(str1);
                Assert::AreEqual(scp1.Size(), 1_z);
                Assert::IsTrue(dat1 == dat2);
                Assert::IsFalse(dat1 != dat2);
                Assert::AreNotSame(scp1, scp2);

                //Test if Datum exists already
                dat1 = scp1.Append(str1);
                dat2 = scp2.Append(str1);
                Assert::IsTrue(dat1 == dat2);
                Assert::IsFalse(dat1 != dat2);
                Assert::AreNotSame(scp1, scp2);
            }
        }
        TEST_METHOD(Operators)
        {
            {
                string str1 = "num1";
                Scope scp1, scp2;
                Datum dat1, dat2;

                Assert::AreEqual(scp1, scp2);
                Assert::AreNotSame(scp1, scp2);
                Assert::IsFalse(scp1 != scp2);
                Assert::IsTrue(scp1 == scp2);

                //Use operator[](syntax sugar for Append) for keys on each scope 
                dat1 = scp1[str1];
                Assert::AreEqual(scp1.Size(), 1_z);
                dat2 = scp2[str1];
                Assert::AreEqual(scp1.Size(), 1_z);
                Assert::AreEqual(dat1, dat2);
                Assert::AreNotSame(scp1, scp2);
            }

            {
                size_t idx1 = 0, idx2 = 1;
                string str1 = "num1", str2 = "num2";
                Scope scp1, scp2;
                Datum dat1, dat2;

                dat1 = scp1[str1];
                Assert::AreEqual(scp1.Size(), 1_z);
                dat2 = scp2[str1];
                Assert::AreEqual(scp1.Size(), 1_z);
                Assert::AreEqual(dat1, dat2);
                Assert::AreNotSame(scp1, scp2);

                //Use operator[] for indexes on each scope 
                dat1 = scp1[idx1];
                Assert::AreEqual(scp1.Size(), 1_z);
                Assert::ExpectException<FieaAbortException>([&scp2, idx2] { scp2[idx2]; });
                Assert::AreEqual(scp1.Size(), 1_z);
                Assert::AreEqual(dat1, dat2);
                Assert::AreNotSame(scp1, scp2);

                dat2 = scp2[str2];
                Assert::AreEqual(scp2.Size(), 2_z);
                dat1 = scp2[idx2];
                Assert::AreEqual(scp1.Size(), 1_z);
                Assert::AreEqual(dat1, dat2);
                Assert::AreNotSame(scp1, scp2);
            }

            {
                size_t idx1 = 0, idx2 = 1;
                string str1 = "num1", str2 = "num2";
                Scope scp1, scp2;
                Datum dat1, dat2;

                dat1 = scp1[str1];
                Assert::AreEqual(scp1.Size(), 1_z);
                dat2 = scp2[str1];
                Assert::AreEqual(scp1.Size(), 1_z);
                Assert::AreEqual(dat1, dat2);
                Assert::AreNotSame(scp1, scp2);

                //Use operator[] for indexes on each scope 
                const Scope child1 = scp1;
                const Scope child2 = scp2;

                Assert::AreEqual(dat1, child1[idx1]);
                Assert::AreEqual(child1.Size(), 1_z);
                Assert::ExpectException<FieaAbortException>([&dat2, &child2, idx2] {dat2 == child2[idx2]; });
                Assert::AreEqual(child1.Size(), 1_z);
                Assert::AreEqual(dat1, dat2);
                Assert::AreNotSame(child1, child2);

                Datum dat3 = scp2[str2];
                const Scope child3 = scp2;
                Assert::AreEqual(child3.Size(), 2_z);
                Datum dat4 = child3[idx2];
                Assert::ExpectException<FieaAbortException>([&dat3, &child3] {dat3 == child3[2]; });
                Assert::AreEqual(dat3, dat4);
            }
        }
        TEST_METHOD(Find)
        {
            {
                string str1 = "num1";
                Scope child, orphan;
                Datum dat1, dat2;

                Assert::AreEqual(child, orphan);
                Assert::AreNotSame(child, orphan);
                Assert::IsFalse(child != orphan);
                Assert::IsTrue(child == orphan);

                //Append a string into Scope data w/ empty initiallization 
                dat1 = child[str1];
                Assert::AreEqual(child.Size(), 1_z);
                dat2 = orphan[str1];
                Assert::AreEqual(child.Size(), 1_z);
                Assert::AreEqual(dat1, dat2);
                Assert::AreNotSame(child, orphan);

                //Test if Datum exists already
                const Scope disabled = orphan;
                Datum* childFinder = child.Find(str1);
                const Datum* orphanFinder = disabled.Find(str1);
                Assert::AreEqual(*orphanFinder, *childFinder);
                Assert::AreNotSame(*childFinder, *orphanFinder);

                childFinder = child.Find("Johnny Boy");
                orphanFinder = disabled.Find("Qutaro Kujo");
                Assert::IsTrue(childFinder == nullptr);
                Assert::IsTrue(orphanFinder == nullptr);
            }
        }
        TEST_METHOD(ToStrClearAndEquals)
        {
            {
                Scope tony(64), tigger(64);
                Foo foo;
                string tonyStr = Scope::TypeName();
                string strTigger = Scope::TypeName();
                string str1 = "num1";
                string str2 = "num2";
                string tempStr = "";
                string tempStr2 = "";

                tigger[str1];
                Assert::IsTrue(tony.Equals(&tigger) == false);
                tony[str1];
                Assert::IsTrue(tigger.Equals(&tony) == true);

                Assert::AreEqual(tonyStr.c_str(), strTigger.c_str());
                tempStr += tonyStr + "(" + std::to_string(tony.Size()) + ")\n";
                tempStr2 += strTigger + "(" + std::to_string(tigger.Size()) + ")\n";

                Assert::AreEqual(tempStr.c_str(), tony.ToString().c_str());
                Assert::AreEqual(tempStr2.c_str(), tigger.ToString().c_str());
                Assert::AreEqual(tempStr2.c_str(), tony.ToString().c_str());
                Assert::AreEqual(tempStr.c_str(), tigger.ToString().c_str());
                Assert::AreNotEqual(tonyStr.c_str(), tony.ToString().c_str());
                Assert::AreNotEqual(tonyStr.c_str(), tigger.ToString().c_str());

                tigger.Clear();
                tony.Clear();
                tempStr.clear();
                tempStr2.clear();

                tigger[str2];
                Assert::IsTrue(tony.Equals(&tigger) == false);
                tony[str2];
                Assert::IsTrue(tigger.Equals(&tony) == true);

                Assert::IsTrue(tony.Equals(&foo) == false);
                Assert::IsTrue(tigger.Equals(&foo) == false);

                Assert::AreEqual(tonyStr.c_str(), strTigger.c_str());
                tempStr += tonyStr + "(" + std::to_string(tony.Size()) + ")\n";
                tempStr2 += strTigger + "(" + std::to_string(tigger.Size()) + ")\n";

                Assert::AreEqual(tempStr.c_str(), tony.ToString().c_str());
                Assert::AreEqual(tempStr2.c_str(), tigger.ToString().c_str());
                Assert::AreEqual(tempStr2.c_str(), tony.ToString().c_str());
                Assert::AreEqual(tempStr.c_str(), tigger.ToString().c_str());
                Assert::AreNotEqual(tonyStr.c_str(), tony.ToString().c_str());
                Assert::AreNotEqual(tonyStr.c_str(), tigger.ToString().c_str());
            }
        }
        TEST_METHOD(AdoptMyClassOrElseDie)
        {
            Scope* winnieDaPooh = new Scope();
            Scope* tigger = new Scope();
            Scope* honeyJar1 = new Scope();
            string beeHive1 = "Hello, I\'m eating some delicious honey.";

            Scope& honeyJar3 = winnieDaPooh->AppendScope(beeHive1, new Scope(12));
            Scope& honeyJar2 = winnieDaPooh->AppendScope(beeHive1, new Scope(18));

            Assert::AreEqual(winnieDaPooh, honeyJar3.GetParent());
            Assert::AreEqual(winnieDaPooh, honeyJar2.GetParent());

            tigger->Adopt(beeHive1, *honeyJar1);
            tigger->Adopt(beeHive1, honeyJar2);

            Assert::AreEqual(tigger, honeyJar1->GetParent());
            Assert::AreEqual(tigger, honeyJar2.GetParent());

            Scope& honeyJar4 = winnieDaPooh->AppendScope(beeHive1, nullptr);
            Scope& scp1 = honeyJar3.AppendScope(beeHive1, new Scope(12));
            Scope& scp2 = honeyJar3.AppendScope(beeHive1, new Scope(12));
            Scope& scp3 = honeyJar4.AppendScope(beeHive1, new Scope(24));

            Assert::AreEqual(&honeyJar3, scp1.GetParent());
            Assert::IsTrue(honeyJar3.IsAncestorOf(scp2) == true);
            Assert::IsTrue(honeyJar3.IsDescendentOf(*winnieDaPooh) == true);

            Assert::IsTrue(winnieDaPooh->IsAncestorOf(scp2) == true);
            Assert::IsTrue(scp2.IsDescendentOf(*winnieDaPooh) == true);

            Scope* cleanJar = honeyJar3.Orphan();
            Assert::IsTrue(cleanJar->IsDescendentOf(*tigger) == false);
            Assert::IsTrue(cleanJar->IsAncestorOf(scp2) == true);

            delete winnieDaPooh;
            delete tigger;
        }
    };
}

#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<Fiea::Engine::Tests::Scope>(const Fiea::Engine::Tests::Scope& t)
    {
        if (&t == nullptr)
        {
            FIEA_ABORT_STR("Tried to dereference a Scope object with nullptr address.");
        }
        else
        {
            RETURN_WIDE_STRING("EmptyChild: " << t.Size());
        }
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Tests::Scope>(const Fiea::Engine::Tests::Scope* t)
    {
        if (t == nullptr)
        {
            FIEA_ABORT_STR("Tried to dereference a Scope object with nullptr address.");
        }
        else
        {
            RETURN_WIDE_STRING("EmptyChild: " << t->Size());
        }
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Tests::Scope>(Fiea::Engine::Tests::Scope* t)
    {
        if (t == nullptr)
        {
            FIEA_ABORT_STR("Tried to dereference a Scope object with nullptr address.");
        }
        else
        {
            RETURN_WIDE_STRING("EmptyChild: " << t->Size());
        }
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Datum>(const Fiea::Engine::Content::Datum& t)
    {
        RETURN_WIDE_STRING("Datum: " << t.Size() << " elements.");
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Datum>(const Fiea::Engine::Content::Datum* t)
    {
        RETURN_WIDE_STRING("Datum: " << t->Size() << " elements.");
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Datum>(Fiea::Engine::Content::Datum* t)
    {
        RETURN_WIDE_STRING("Datum: " << t->Size() << " elements.");
    }
}

//void* operator new(size_t n)
//{
//    return malloc(n);
//}
//void operator delete(void* ptr)
//{
//    free(ptr);
//}