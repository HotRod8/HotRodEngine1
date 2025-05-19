#include "Tests.h"
#include "../include/FieaGameEngine/FactoryService.h"
#include "../src/Content/Scope.h"
#include "Support/ScopedFoo.h"

namespace Fiea::Engine::Tests
{
    using Scope = Fiea::Engine::Content::Scope;
    using ScopeFactory = Fiea::Engine::Content::ScopeFactory;
    using ScopedFooFactory = Fiea::Engine::Tests::Support::ScopedFooFactory;

    TEST_CLASS(FactoryTester)
    {
        TEST_MEMCHECK;
        
        TEST_METHOD(ScopeFactoryTester)
        {
            ScopeFactory scpFact;
            Scope* scp1 = scpFact.Create();
            Scope* scp2 = scpFact.Create();

            Assert::AreNotSame(scp1, scp2);
            Assert::AreEqual(*scp1, *scp2);

            string val = scpFact.GetProductName();

            Assert::AreEqual(val, scpFact.GetProductName());

            delete scp1;
            delete scp2;
        }
        TEST_METHOD(ScopedFooFactoryTester)
        {
            ScopedFooFactory scpFact;
            Scope* scp1 = scpFact.Create();
            Scope* scp2 = scpFact.Create();
            ScopedFoo* scpFoo1 = std::move(static_cast<ScopedFoo*>(scp1));
            ScopedFoo* scpFoo2 = std::move(static_cast<ScopedFoo*>(scp2));

            Assert::AreNotSame(scpFoo1, scpFoo2);
            Assert::AreEqual(*scpFoo1, *scpFoo2);

            string val = scpFact.GetProductName();

            Assert::AreEqual(val, scpFact.GetProductName());

            delete scpFoo1;
            delete scpFoo2;
        }
        TEST_METHOD(FactoryServiceTester)
        {
            const string scpType = "Scope";
            const string scpFooType = "ScopedFoo";
            const string scpFakeType = "ScrapedFoo";
            FactoryService server;

            server.Register<Scope>(std::make_unique<ScopeFactory>());
            server.Register<Scope>(std::make_unique<ScopedFooFactory>());

            Scope* scp1 = server.CreateProduct<Scope>(scpType);
            Scope* scp2 = server.CreateProduct<Scope>(scpType);
            Scope* scp3 = server.CreateProduct<Scope>(scpFakeType);
            Scope* scpFoo1 = server.CreateProduct<Scope>(scpFooType);
            Scope* scpFoo2 = server.CreateProduct<Scope>(scpFooType);
            Scope* scpFoo3 = server.CreateProduct<Scope>(scpFakeType);

            Assert::AreNotSame(scp1, scp2);
            Assert::AreEqual(*scp1, *scp2);
            Assert::ExpectException<FieaAbortException>([scp1, scp3]() { Assert::AreNotSame(scp1, scp3); });
            Assert::ExpectException<FieaAbortException>([scp1, scp3]() { Assert::AreEqual(*scp1, *scp3); });
            Assert::ExpectException<FieaAbortException>([scp2, scp3]() { Assert::AreNotSame(scp3, scp2); });
            Assert::ExpectException<FieaAbortException>([scp2, scp3]() { Assert::AreEqual(*scp2, *scp3); });

            Assert::AreNotSame(scpFoo1, scpFoo2);
            Assert::AreEqual(*scpFoo1, *scpFoo2);
            Assert::ExpectException<FieaAbortException>([scpFoo1, scpFoo3]() { Assert::AreNotSame(scpFoo1, scpFoo3); });
            Assert::ExpectException<FieaAbortException>([scpFoo1, scpFoo3]() { Assert::AreEqual(*scpFoo1, *scpFoo3); });
            Assert::ExpectException<FieaAbortException>([scpFoo2, scpFoo3]() { Assert::AreNotSame(scpFoo3, scpFoo2); });
            Assert::ExpectException<FieaAbortException>([scpFoo2, scpFoo3]() { Assert::AreEqual(*scpFoo2, *scpFoo3); });

            delete scp1;
            delete scp2;
            delete scpFoo1;
            delete scpFoo2;
        }
    };
}

#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<Fiea::Engine::Tests::Scope>(const Fiea::Engine::Tests::Scope& t)
    {
        if(&t == nullptr)
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
}