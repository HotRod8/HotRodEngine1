#include "Tests.h"
#include "../../FieaGameEngine/include/FieaGameEngine/ClockService.h"
#include "Support/FooManager.h"

namespace Fiea::Engine::Tests
{
    TEST_CLASS(ServiceProviderTest)
    {
        TEST_MEMCHECK;

        TEST_METHOD(ServerTester)
        {
            ServiceMgr* manager = ServiceMgr::Instance();

            IFooMaker* fooMaker = manager->ProvideInterface<IFooMaker>();
            fooMaker->Make("hello", 29);

            fooMaker->Make("foo", 30);

            fooMaker->Make("bar", 42);

            IFooTaker* fooTaker = manager->ProvideInterface<IFooTaker>();
            int32_t x = fooTaker->Take("hello");

            Assert::IsTrue(x == 29);

            manager->Reset();
        }

        TEST_METHOD(ClockChimer)
        {
            ServiceMgr* manager = ServiceMgr::Instance();

            //Clock::ITimeProvider* timeProvider = manager->ProvideInterface<Clock::ITimeProvider>();
            ITimeProvider* timeProvider = manager->ProvideInterface<ITimeProvider>();
            // basic retrieval
            uint32_t secondsSinceEpoch = timeProvider->Time();

            //Clock::ITimeUpdater* timeUpdater = manager->ProvideInterface<Clock::ITimeUpdater>();
            ITimeUpdater* timeUpdater = manager->ProvideInterface<ITimeUpdater>();
            // for manual updating
            timeUpdater->Update(5000); // go 5000 seconds into the future

            // for auto updating (in a game loop?)
            timeUpdater->Update();

            manager->Reset();
        }
    };
}