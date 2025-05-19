#include "Tests.h"
#include "Support/Foo.h"
#include "../FieaGameEngine/src/Content/Datum.h"
#include "../FieaGameEngine/src/Content/Scope.h"

namespace Fiea::Engine::Tests
{
    using Datum = Content::Datum;
    using Scope = Content::Scope;

    TEST_CLASS(DatumTester)
    {
        TEST_METHOD(Construction)
        {
            //Val&& param == Universal ref!(generally)
            //When using val&& param, use std::move(param) for ops and exprs to use in debugging tools
            //Efficiency speed is same w/ or w/o move
            //If u get val&& param, use std::move(param) to return the rval
            Scope scp1(0);
            Scope scp2(11);
            Scope scp3(12);
            Scope scp4(13);
            Scope scp5(10);

            Foo foo0(6);
            Foo foo1(11);
            Foo foo2(12);
            Foo foo3(13);
            Foo foo4(10);

            Datum* d0 = new Datum();
            Datum d1(6);
            Datum d2(6.0f);
            Datum d3("six");
            Datum d3a(&foo0);
            Datum d3b(&scp1);
            Datum d4({ 11, 12, 13 });
            Datum d5({ 11.0f, 12.0f, 13.0f });
            Datum d6({ "eleven", "twelve", "thirteen" });
            Datum d6a({ &foo1, &foo2, &foo3 });
            Datum d6b({ &scp2, &scp3, &scp4 });
            Datum d7(d1);
            Datum d8(d2);
            Datum d9(d3);
            Datum d9a(d3a);
            Datum d9b(d3b);
            Datum d10(std::move(Datum(10)));
            Datum d11(std::move(Datum(10.0f)));
            Datum d12(std::move(Datum("ten")));
            Datum d12a(std::move(Datum(&foo4)));
            Datum d12b(std::move(Datum(&scp5)));

            Assert::AreEqual(0_z, d0->Size());
            Assert::AreEqual(1_z, d1.Size());
            Assert::AreEqual(1_z, d2.Size());
            Assert::AreEqual(1_z, d3.Size());
            Assert::AreEqual(1_z, d3a.Size());
            Assert::AreEqual(1_z, d3b.Size());
            Assert::AreEqual(3_z, d4.Size());
            Assert::AreEqual(3_z, d5.Size());
            Assert::AreEqual(3_z, d6a.Size());
            Assert::AreEqual(3_z, d6b.Size());
            Assert::AreEqual(3_z, d6.Size());
            Assert::AreEqual(1_z, d7.Size());
            Assert::AreEqual(1_z, d8.Size());
            Assert::AreEqual(1_z, d9.Size());
            Assert::AreEqual(1_z, d9a.Size());
            Assert::AreEqual(1_z, d9b.Size());
            Assert::AreEqual(1_z, d10.Size());
            Assert::AreEqual(1_z, d11.Size());
            Assert::AreEqual(1_z, d12.Size());
            Assert::AreEqual(1_z, d12a.Size());
            Assert::AreEqual(1_z, d12b.Size());

            Assert::IsTrue(d0->Size() == 0_z && d0->Capacity() == 0_z);
            Assert::IsTrue(d1.Size() == 1_z && d1.Capacity() == 1_z);
            Assert::IsTrue(d2.Size() == 1_z && d2.Capacity() == 1_z);
            Assert::IsTrue(d3.Size() == 1_z && d3.Capacity() == 1_z);
            Assert::IsTrue(d3a.Size() == 1_z && d3a.Capacity() == 1_z);
            Assert::IsTrue(d3b.Size() == 1_z && d3b.Capacity() == 1_z);
            Assert::IsTrue(d4.Size() == 3_z && d4.Capacity() == 3_z);
            Assert::IsTrue(d5.Size() == 3_z && d5.Capacity() == 3_z);
            Assert::IsTrue(d6.Size() == 3_z && d6.Capacity() == 3_z);
            Assert::IsTrue(d6a.Size() == 3_z && d6a.Capacity() == 3_z);
            Assert::IsTrue(d6b.Size() == 3_z && d6b.Capacity() == 3_z);
            Assert::IsTrue(d7.Size() == 1_z && d7.Capacity() == 1_z);
            Assert::IsTrue(d8.Size() == 1_z && d8.Capacity() == 1_z);
            Assert::IsTrue(d9.Size() == 1_z && d9.Capacity() == 1_z);
            Assert::IsTrue(d9a.Size() == 1_z && d9a.Capacity() == 1_z);
            Assert::IsTrue(d9b.Size() == 1_z && d9b.Capacity() == 1_z);
            Assert::IsTrue(d10.Size() == 1_z && d10.Capacity() == 1_z);
            Assert::IsTrue(d11.Size() == 1_z && d11.Capacity() == 1_z);
            Assert::IsTrue(d12.Size() == 1_z && d12.Capacity() == 1_z);
            Assert::IsTrue(d12a.Size() == 1_z && d12a.Capacity() == 1_z);
            Assert::IsTrue(d12b.Size() == 1_z && d12b.Capacity() == 1_z);

            //Assert::AreEqual(0_z, d0->Size());
            Assert::AreEqual(6, d1.GetInt(0));
            Assert::AreEqual(6.0f, d2.GetFloat(0));
            Assert::AreEqual((string)"six", d3.GetString(0));
            Assert::AreEqual(&foo0, d3a.GetRTTI(0)->As<Foo>());
            Assert::AreEqual(&scp1, d3b.GetScope(0)->As<Scope>());
            Assert::AreEqual(11, d4.GetInt(0));
            Assert::AreEqual(12.0f, d5.GetFloat(1));
            Assert::AreEqual((string)"thirteen", d6.GetString(2));
            Assert::AreEqual(&foo1, d6a.GetRTTI(0)->As<Foo>());
            Assert::AreEqual(&scp2, d6b.GetScope(0)->As<Scope>());
            Assert::AreEqual(6, d7.GetInt(0));
            Assert::AreEqual(6.0f, d8.GetFloat(0));
            Assert::AreEqual((string)"six", d9.GetString(0));
            Assert::AreEqual(&foo0, d9a.GetRTTI(0)->As<Foo>());
            Assert::AreEqual(&scp1, d9b.GetScope(0)->As<Scope>());
            Assert::AreEqual(10, d10.GetInt(0));
            Assert::AreEqual(10.0f, d11.GetFloat(0));
            Assert::AreEqual((string)"ten", d12.GetString(0));
            Assert::AreEqual(&foo4, d12a.GetRTTI(0)->As<Foo>());
            Assert::AreEqual(&scp5, d12b.GetScope(0)->As<Scope>());

            d3a.PopRTTI();
        }
        TEST_METHOD(Assignment)
        {
            Scope scp1(109);
            Scope scp2(110);

            Foo foo1(11);
            Foo foo2(12);

            Datum* d1 = new Datum();
            Datum* d2 = new Datum();
            Datum* d3 = new Datum();
            Datum* d4 = new Datum();
            Datum* d4a = new Datum();
            Datum* d4b = new Datum();
            Datum* d5 = new Datum();
            Datum* d6 = new Datum(6);
            Datum* d7 = new Datum(6.0f);
            Datum* d8 = new Datum("six");
            Datum* d8a = new Datum(&foo1);
            Datum* d8b = new Datum(&scp1);

            *d1 = *d6;
            *d2 = *d7;
            *d3 = *d8;
            *d4 = *d5;
            *d4a = *d8a;
            *d4b = *d8b;

            Assert::IsTrue(*d4 == *d5);
            Assert::IsTrue(*d3 == *d8);
            Assert::IsTrue(*d2 == *d7);
            Assert::IsTrue(*d1 == *d6);
            Assert::IsTrue(*d4a == *d8a);
            Assert::IsTrue(*d4b == *d8b);

            *d5 = std::move(Datum());
            *d6 = std::move(Datum(239));
            *d7 = std::move(Datum(239.0f));
            *d8 = std::move(Datum("Two hundred thirty-nine"));
            *d8a = std::move(Datum(&foo2));
            *d8b = std::move(Datum(&scp2));

            Assert::IsTrue((*d4 != *d5) == false);
            Assert::IsTrue(*d3 != *d8);
            Assert::IsTrue(*d2 != *d7);
            Assert::IsTrue(*d1 != *d6);
            Assert::IsTrue(*d4a != *d8a);
            Assert::IsTrue(*d4b != *d8b);

            Scope scp3;
            Foo foo3;

            Datum d9(102);
            Datum d10(102.0f);
            Datum d11("one o' two");
            Datum d12(&foo3);
            Datum d13(&scp3);

            d9 = 12;
            d10 = 12.0f;
            d11 = "twelve";
            d12 = &foo1;
            d13 = &scp1;
        }
        TEST_METHOD(Modifiers)
        {
            Foo foo1(25);
            Foo foo2(97);
            Foo foo3(108);
            Scope scp1(9);
            Scope scp2(92);
            Scope scp3(240);

            Datum* d0 = new Datum();
            Datum d1({ 11, 12, 13 });
            Datum d2({ 11.0f, 12.0f, 13.0f });
            Datum d3({ "eleven", "twelve", "thirteen" });
            Datum d4({ &foo1, &foo2, &foo3 });
            Datum d5({ &scp1, &scp2, &scp3 });

            d1.PushInt(15);
            //d1.PushFloat(15.0f);
            //d1.PushString("fifteen");

            //d2.PushInt(15);
            d2.PushFloat(15.0f);
            //d2.PushString("fifteen");

            //d3.PushInt(15);
            //d3.PushFloat(15.0f);
            d3.PushString("fifteen");

            Foo foo4(14);
            Scope scp4;

            d4.PushRTTI(&foo4);

            d5.PushScope(&scp4);
            
            d0->SetType(Datum::Type::Float);
            //d0->SetType(Datum::Type::String);
            //d0->SetType(Datum::Type::Int);


            Assert::IsTrue(d1.IsType(Datum::Type::String) == false);
            Assert::IsTrue(d1.IsType(Datum::Type::Float) == false);
            Assert::IsTrue(d1.IsType(Datum::Type::Int) == true);
            Assert::IsTrue(d1.IsType(Datum::Type::Pointer) == false);
            Assert::IsTrue(d1.IsType(Datum::Type::ScopeTable) == false);

            Assert::IsTrue(d2.IsType(Datum::Type::String) == false);
            Assert::IsTrue(d2.IsType(Datum::Type::Float) == true);
            Assert::IsTrue(d2.IsType(Datum::Type::Int) == false);
            Assert::IsTrue(d2.IsType(Datum::Type::Pointer) == false);
            Assert::IsTrue(d2.IsType(Datum::Type::ScopeTable) == false);

            Assert::IsTrue(d3.IsType(Datum::Type::String) == true);
            Assert::IsTrue(d3.IsType(Datum::Type::Float) == false);
            Assert::IsTrue(d3.IsType(Datum::Type::Int) == false);
            Assert::IsTrue(d3.IsType(Datum::Type::Pointer) == false);
            Assert::IsTrue(d3.IsType(Datum::Type::ScopeTable) == false);

            Assert::IsTrue(d4.IsType(Datum::Type::String) == false);
            Assert::IsTrue(d4.IsType(Datum::Type::Float) == false);
            Assert::IsTrue(d4.IsType(Datum::Type::Int) == false);
            Assert::IsTrue(d4.IsType(Datum::Type::Pointer) == true);
            Assert::IsTrue(d4.IsType(Datum::Type::ScopeTable) == false);

            Assert::IsTrue(d5.IsType(Datum::Type::String) == false);
            Assert::IsTrue(d5.IsType(Datum::Type::Float) == false);
            Assert::IsTrue(d5.IsType(Datum::Type::Int) == false);
            Assert::IsTrue(d5.IsType(Datum::Type::Pointer) == false);
            Assert::IsTrue(d5.IsType(Datum::Type::ScopeTable) == true);

            Foo foo5(140);
            Scope scp5(49);

            d1.SetInt(14, 3_z);
            d2.SetFloat(14.0f, 3_z);
            d3.SetString("fourteen", 3_z);
            d4.SetRTTI(&foo5, 3_z);
            d5.SetScope(&scp5, 3_z);

            //d0->PushInt(35);
            d0->PushFloat(35.5f);
            //d0->PushString("thirty-five");

            //d0->PopInt();
            //d0->PopInt();
            d0->PopFloat();
            d0->PopFloat();
            //d0->PopString();
            //d0->PopString();

            d1.PopInt();
            d2.PopFloat();
            d3.PopString();
            d4.PopRTTI();
            d5.PopScope();

            Assert::IsTrue(d1 != d3);
            Assert::IsTrue(d1 != d2);
            Assert::IsTrue(d1 != d4);
            Assert::IsTrue(d1 != d5);
            Assert::IsTrue(d2 != d3);
            Assert::IsTrue(d2 != d4);
            Assert::IsTrue(d2 != d5);
            Assert::IsTrue(d3 != d4);
            Assert::IsTrue(d3 != d5);
            Assert::IsTrue(d4 != d5);

            Assert::IsTrue(d1 == d1);
            Assert::IsFalse(*d0 == d2);
        }
        TEST_METHOD(StrModifiers)
        {
            string fiftyi = "50";
            string fiftyf = "50.0f";
            Datum* dati = new Datum();
            Datum* datf = new Datum();

            dati->SetType(Datum::Type::Int);
            dati->PushIntFromString(fiftyi);
            dati->PushIntFromString("100");
            size_t diSize = dati->Size();

            string datiNum1 = dati->GetIntToString(0);
            string datiNum2 = dati->GetIntToString(1);

            for(size_t i = 0; i < diSize; ++i)
            {
                dati->SetIntFromString("37", i);
            }

            datf->SetType(Datum::Type::Float);
            datf->PushFloatFromString(fiftyf);
            datf->PushFloatFromString("100");
            size_t dfSize = datf->Size();

            string datfNum1 = datf->GetFloatToString(0);
            string datfNum2 = datf->GetFloatToString(1);

            for (size_t i = 0; i < dfSize; ++i)
            {
                datf->SetFloatFromString("37", i);
            }
        }
        TEST_METHOD(ExtStorage)
        {
            Foo foo1(45);
            Foo foo2(57);
            Foo foo3(348);
            Scope scp1;
            Scope scp2(10);
            Scope scp3(24);

            int32_t intArr[] = {45, 57, 348, 2947};
            float floatArr[] = {16.9f, 39.2f, 2019.042f};
            string strArr[] = { "no way", "don't touch me" };
            RTTI* fooArr[] = { &foo1, &foo2, &foo3 };
            Scope* scopeArr[] = { &scp1, &scp2, &scp3 };

            Datum* d1 = new Datum();
            Datum* d2 = new Datum();
            Datum* d3 = new Datum();
            Datum* d4 = new Datum();
            Datum* d5 = new Datum();

            d1->SetStorage(intArr, 4_z);
            d2->SetStorage(floatArr, 3_z);
            d3->SetStorage(strArr, 2_z);
            d4->SetStorage(fooArr, 3_z);
            d5->SetStorage(scopeArr, 3_z);

            d1->SetIntFromString("16662", 1);
            string newStrI = d1->GetIntToString(3);

            for(size_t i = 0; i < 4_z; ++i)
            {
                Assert::IsTrue(d1->GetInt(i) == intArr[i]);
            }

            d2->SetFloatFromString("50.7140f", 1);
            string newStrF = d2->GetFloatToString(1);

            for (size_t i = 0; i < 3_z; ++i)
            {
                Assert::IsTrue(d2->GetFloat(i) == floatArr[i]);
            }

            d3->SetString(newStrI, 0);
            d3->SetString(newStrF, 1);

            Assert::AreEqual(d3->GetString(0), newStrI);
            Assert::AreEqual(d3->GetString(1), newStrF);
        }
    };
}

#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Scope>(const Fiea::Engine::Content::Scope& t)
    {
        RETURN_WIDE_STRING("Scope: " << t.Size() << " elements.");
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Scope>(const Fiea::Engine::Content::Scope* t)
    {
        RETURN_WIDE_STRING("Scope: " << t->Size() << " elements.");
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Scope>(Fiea::Engine::Content::Scope* t)
    {
        RETURN_WIDE_STRING("Scope: " << t->Size() << " elements.");
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