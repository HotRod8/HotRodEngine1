#include "Tests.h"
#include "Support/Foo.h"
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
    //// spell system
    //struct ISpell
    //{
    //    virtual void Cast() = 0;
    //    virtual ~ISpell() {}
    //};

    //struct ISpellMaker
    //{
    //    virtual ISpell* Create() = 0;
    //};

    //class SpellManager
    //{
    //public:
    //    void Add(string spellName, ISpellMaker* spell) {
    //        _SpellMakers[spellName] = spell;

    //        auto pair = _SpellMakers.insert({ spellName, spell });
    //        if (pair.second)
    //        {
    //            // successfully added
    //        }
    //        else
    //        {
    //            // failed to insert, already had it?
    //        }
    //        auto& itToTheItem = pair.first;

    //    }

    //    void Cast(string spellToCast) {
    //        auto it = _SpellMakers.find(spellToCast);
    //        if (it != _SpellMakers.end())
    //        {
    //            ISpell* theSpell = it->second->Create();
    //            Cast(theSpell);
    //        }
    //    }
    //    void Cast(ISpell* spellToCast) {
    //        spellToCast->Cast();
    //    }

    //private:
    //    std::unordered_map<string, ISpellMaker*> _SpellMakers{};
    //};


    //// up in higher level code
    //struct Fireball : public ISpell
    //{
    //    virtual void Cast() override {}
    //};
    //struct FireballFactory : public ISpellMaker
    //{
    //    // Inherited via ISpellMaker
    //    ISpell* Create() override
    //    {
    //        return new Fireball;
    //    }
    //};

    //struct GravityHold : public ISpell
    //{
    //    virtual void Cast() override {}
    //};
    //struct GravityHoldFactory : public ISpellMaker
    //{
    //    // Inherited via ISpellMaker
    //    ISpell* Create() override
    //    {
    //        return new GravityHold;
    //    }
    //};

    //struct SummonCatlikeFamiliar : public ISpell
    //{
    //    virtual void Cast() override {}
    //};
    //struct SummonCatlikeFamiliarFactory : public ISpellMaker
    //{
    //    // Inherited via ISpellMaker
    //    ISpell* Create() override
    //    {
    //        return new SummonCatlikeFamiliar;
    //    }
    //};


    TEST_CLASS(CoreTests)
    {
        //using IdType = size_t;
        ////Scope will only take member fields for now
        //class Scope{};
        ////need to establish relationship w/ RTTI
        ////parent pointers are a great first start
        //class Bar : public Scope
        //{
        //public:
        //    static inline IdType sId = 2_z;

        //    //confirmation check for each scope object
        //    template<typename T>
        //    bool Is(T* inst)
        //    {
        //        return inst.Id == T.Id;
        //    }

        //    IdType id;
        //    Bar() : id(sId) {}
        //};

        //class BarChild: public Bar
        //{
        //public:
        //    static inline IdType sId = 1_z;

        //    //confirmation check for each scope object
        //    template<typename T>
        //    bool Is(T* inst)
        //    {
        //        //to account for multiple levels of inheritance
        //        return inst.Id == T.Id;
        //    }

        //    IdType id;
        //    BarChild() : id(sId) {}
        //};

        //void f(Bar* bc, Foo* foo = nullptr)
        //{
        //    bool isBar = bc->Is<Bar>(bc);

        //    bool isBarChild = bc->Is<BarChild>();
        //}

        //Everything above counts as the "boilerplate" of FooTests.cpp
        static int GetInt(int32_t* ptr) {
            //_ASSERT acts an modifiassert replace
            FIEA_ASSERT(ptr != nullptr);
            return *ptr;
        }

        static void OtherTestMethod() 
        {
            GetInt(nullptr);
        }

        TEST_METHOD(GetInt)
        {
            int x = 5;
            Assert::AreEqual(x, GetInt(&x));

            ExpectSEH<EXCEPTION_ACCESS_VIOLATION>([] { GetInt(nullptr); });

            //FIEA_ASSERT_HIDE();

            //ExpectSEH<FIEA_ABORT_CODE>([] { GetInt(nullptr); });
        }
        //TEST_METHOD(GetInt) 
        //{
        //    int32_t x = 7;
        //    Assert::AreEqual(x, GetInt(&x));
        //    
        //    //int oldAssertMode = _CrtSetReportMode();
        //    //ExpectSEH<FIEA_ACCESS_VIOLATION>(OtherTestMethod);
        //    {
        //        ScopedAssertHide __unused;
        //    }

        //    // If you throw an exception here, the rest of the code below will never execute.
        //    //_CrtSetReportMode(oldAssertMode);
        //    //Assert::AreEqual(0, GetInt(nullptr));
        //}
    };
}