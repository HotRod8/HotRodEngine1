#include "Tests.h"
#include "../src/Memory/Heap.h"
#include "FieaGameEngine/MemoryService.h"
#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//using Heapy = Fiea::Engine::Memory::Heap;


namespace Fiea::Engine::Tests
{
// the following macro is a general gist of what's needed within part 3 of the HW
//#ifdef FIEA_MEMORY_DEBUG
//#define FNEW(_al, _type) new(_al, __FILE__, __LINE__) _type
//#else
//#define FNEW(_al, _type) new(_al) _type
//#endif

    TEST_CLASS(AllocTest)
    {
        TEST_MEMCHECK;

        TEST_METHOD(MakeAndDestroyService)
        {
            Memory::MakeService();
            Memory::IMemoryService* singleMem = Memory::GetMemoryService();
            Assert::IsNotNull(singleMem);
            Memory::IAllocatorProvider* singleAlloc = Memory::GetAllocatorProvider();
            Assert::IsNotNull(singleAlloc);

            Memory::DestroyService();
        }
        TEST_METHOD(GetAllocatorProvider)
        {
            Memory::MakeService();
            Memory::IAllocatorProvider* singleAlloc = Memory::GetAllocatorProvider();
            Assert::IsNotNull(singleAlloc);

            Memory::DestroyService();
        }
        TEST_METHOD(GetMemoryService)
        {
            Memory::MakeService();
            Memory::IMemoryService* singleMem = Memory::GetMemoryService();
            Assert::IsNotNull(singleMem);

            Memory::DestroyService();
        }
        TEST_METHOD(CreateAndDestroyMemHeap)
        {
            string str = "newHeap";
            size_t size = 100_z;
            Memory::MakeService();
            Memory::IMemoryService* singleMem = Memory::GetMemoryService();
            Assert::IsNotNull(singleMem);
            Memory::IAllocatorProvider* singleAlloc = Memory::GetAllocatorProvider();
            Assert::IsNotNull(singleAlloc);

            Memory::Heap* h = singleMem->CreateHeap(str.c_str(), size);
            size = h->Align(size);
            Assert::IsNotNull(h);

            singleMem->DestroyHeap(h);

            Memory::DestroyService();
        }
        TEST_METHOD(GetAllocs)
        {
            string str = "distilled water";
            Memory::MakeService();
            Memory::IMemoryService* singleMem = Memory::GetMemoryService();
            Assert::IsNotNull(singleMem);
            Memory::IAllocatorProvider* singleAlloc = Memory::GetAllocatorProvider();
            Assert::IsNotNull(singleAlloc);

            Memory::Heap* heapAlloc = singleMem->CreateHeap(str.c_str(), 128_z);

            Memory::Allocator al1 = singleAlloc->GetAllocator(heapAlloc);
            Assert::IsNotNull(&al1);
            Memory::Allocator al2 = singleAlloc->GetAllocator(str.c_str());
            Assert::IsNotNull(&al2);

            singleMem->DestroyHeap(heapAlloc);

            Memory::DestroyService();
        }
        TEST_METHOD(DebugAllocation)
        {
            size_t max = 2560_z;
            string str = "poopy water";
            Memory::MakeService();
            Memory::IMemoryService* singleMem = Memory::GetMemoryService();
            Assert::IsNotNull(singleMem);
            Memory::IAllocatorProvider* singleAlloc = Memory::GetAllocatorProvider();
            Assert::IsNotNull(singleAlloc);

            Memory::Heap* heapAlloc = singleMem->CreateHeap(str.c_str(), max);
            Memory::Heap* nullAlloc = nullptr;
            Memory::Allocator al1 = singleAlloc->GetAllocator(heapAlloc);
            Assert::IsNotNull(&al1);
            // FIEA_ASSERT gets called w/ the below attempt to get an allocator w/ a nullptr
//            Memory::Allocator al2 = singleAlloc->GetAllocator(nullAlloc);
//            Assert::IsNull(&al2);

            // FNEW Test
            string* block1 = FNEW(al1, string);
            Assert::IsNotNull(block1);
            Assert::AreEqual((heapAlloc->Available() + heapAlloc->Used() + heapAlloc->Overhead()), max);
            Assert::IsTrue(heapAlloc->Used() == sizeof(string));
            Assert::IsTrue(heapAlloc->Overhead() == (heapAlloc->GetHeaderSize()));

            uint32_t* block2 = FNEW(al1, uint32_t);
            Assert::IsNotNull(block2);
            Assert::AreEqual((heapAlloc->Available() + heapAlloc->Used() + heapAlloc->Overhead()), max);
            Assert::IsTrue(heapAlloc->Used() == sizeof(string) + sizeof(uint32_t));
            Assert::IsTrue(heapAlloc->Overhead() == (heapAlloc->GetHeaderSize() * 2));

            long* block3 = FNEW(al1, long);
            Assert::IsNotNull(block3);
            Assert::AreEqual((heapAlloc->Available() + heapAlloc->Used() + heapAlloc->Overhead()), max);
            Assert::IsTrue(heapAlloc->Used() == sizeof(string) + sizeof(uint32_t) + sizeof(long));
            Assert::IsTrue(heapAlloc->Overhead() == (heapAlloc->GetHeaderSize() * 3));

            // FALLOC Test
            string* testblk1 = static_cast<string*>(FALLOC(al1, sizeof(string)));
            Assert::IsNotNull(testblk1);
            uint32_t* testblk2 = static_cast<uint32_t*>(FALLOC(al1, sizeof(uint32_t)));
            Assert::IsNotNull(testblk2);
            long* testblk3 = static_cast<long*>(FALLOC(al1, sizeof(long)));
            Assert::IsNotNull(testblk3);

            // FFREE Test
            FFREE(al1, testblk1);
            FFREE(al1, testblk2);
            FFREE(al1, testblk3);

            // FDELETE Test
            FDELETE(al1, block3);
            FDELETE(al1, block2);
            FDELETE(al1, block1);

            singleMem->DestroyHeap(heapAlloc);
            Memory::DestroyService();
        }
        TEST_METHOD(GeneralAllocation)
        {
            size_t max = 2560_z;
            string str = "poopy water";
            Memory::MakeService();
            Memory::IMemoryService* singleMem = Memory::GetMemoryService();
            Assert::IsNotNull(singleMem);
            Memory::IAllocatorProvider* singleAlloc = Memory::GetAllocatorProvider();
            Assert::IsNotNull(singleAlloc);

            Memory::Heap* heapAlloc = singleMem->CreateHeap(str.c_str(), max);
            Memory::Heap* nullAlloc = nullptr;
            Memory::Allocator al1 = singleAlloc->GetAllocator(heapAlloc);
            Assert::IsNotNull(&al1);

            // FNEW Test
            string* block1 = FNEW(al1, string);
            Assert::IsNotNull(block1);
            Assert::AreEqual((heapAlloc->Available() + heapAlloc->Used() + heapAlloc->Overhead()), max);
            Assert::IsTrue(heapAlloc->Used() == sizeof(string));
            Assert::IsTrue(heapAlloc->Overhead() == (heapAlloc->GetHeaderSize()));

            uint32_t* block2 = FNEW(al1, uint32_t);
            Assert::IsNotNull(block2);
            Assert::AreEqual((heapAlloc->Available() + heapAlloc->Used() + heapAlloc->Overhead()), max);
            Assert::IsTrue(heapAlloc->Used() == sizeof(string) + sizeof(uint32_t));
            Assert::IsTrue(heapAlloc->Overhead() == (heapAlloc->GetHeaderSize() * 2));

            long* block3 = FNEW(al1, long);
            Assert::IsNotNull(block3);
            Assert::AreEqual((heapAlloc->Available() + heapAlloc->Used() + heapAlloc->Overhead()), max);
            Assert::IsTrue(heapAlloc->Used() == sizeof(string) + sizeof(uint32_t) + sizeof(long));
            Assert::IsTrue(heapAlloc->Overhead() == (heapAlloc->GetHeaderSize() * 3));

            // FALLOC Test
            string* testblk1 = static_cast<string*>(FALLOC(al1, sizeof(string)));
            Assert::IsNotNull(testblk1);
            uint32_t* testblk2 = static_cast<uint32_t*>(FALLOC(al1, sizeof(uint32_t)));
            Assert::IsNotNull(testblk2);
            long* testblk3 = static_cast<long*>(FALLOC(al1, sizeof(long)));
            Assert::IsNotNull(testblk3);

            // FFREE Test
            FFREE(al1, testblk1);
            FFREE(al1, testblk2);
            FFREE(al1, testblk3);

            // FDELETE Test
            FDELETE(al1, block3);
            FDELETE(al1, block2);
            FDELETE(al1, block1);

            singleMem->DestroyHeap(heapAlloc);
            Memory::DestroyService();
        }
    };
}

