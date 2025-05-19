//#include "Tests.h"
//#include "../src/Memory/Heap.h"
//#include <CppUnitTest.h>
//
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//using Heapy = Fiea::Engine::Memory::Heap;
//
//namespace Fiea::Engine::Tests
//{
//    TEST_CLASS(HeapTest)
//    {
//        TEST_MEMCHECK;
//        static void strdelete(string* str)
//        {
//            str->clear();
//            delete str;
//        }
//
//        TEST_METHOD(HeapConstructor)
//        {
//            string str = "constructed";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->GetName() == str);
//
//            Heapy::DestroyHeap(heap);
//        }
//
//        TEST_METHOD(HeapDestructor)
//        {
//            string* str = new string("constructed");
//            Heapy* heap = Heapy::CreateHeap(str->c_str(), 1000_z);
//            Heapy::DestroyHeap(heap);
//            strdelete(str);
//        }
//
//        TEST_METHOD(GetName)
//        {
//            const string str = "My Name Is Roderick Shaw";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->GetName() == str);
//            Heapy::DestroyHeap(heap);
//        }
//
//        TEST_METHOD(Allocation)
//        {
//            string str = "constructed";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//            size_t x = heap->Align(125_z), y = heap->Align(227_z);
//            void* ptr = heap->Alloc(x);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Used() == x);
//
//            ptr = heap->Alloc(y);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Used() == x + y);
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 2));
//
//            size_t z = heap->Align(1000_z - 2_z - heap->GetHeaderSize() - heap->Overhead() - heap->Used());
//            ptr = heap->Alloc(z);
//
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 3));
//            Assert::IsTrue(heap->Used() == x + y + z);
//
//            Heapy::DestroyHeap(heap);
//        }
//
//        TEST_METHOD(Free)
//        {
//            string str = "constructed";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//            size_t x = heap->Align(103_z), y = heap->Align(222_z), z = heap->Align(111_z), w = heap->Align(100_z);
//
//            void* ptr = heap->Alloc(x);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Used() == x);
//
//            ptr = heap->Alloc(y);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Used() == x + y);
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 2));
//
//            void* ptr2 = heap->Alloc(z);
//
//            Assert::IsTrue(heap->Used() == x + y + z);
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 3));
//
//            void* ptr3 = heap->Alloc(w);
//
//            Assert::IsTrue(heap->Used() == x + y + z + w);
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 4));
//
//            heap->Free(ptr);
//            Assert::IsTrue(heap->Used() == x + z + w);
//
//            heap->Free(ptr3);
//            Assert::IsTrue(heap->Used() == x + z);
//
//            Heapy::DestroyHeap(heap);
//        }
//
//        TEST_METHOD(CoalesceAll)
//        {
//            string str = "constructed";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//            size_t x = heap->Align(100_z), y = heap->Align(222_z), z = heap->Align(100_z), w = heap->Align(100_z);
//            void* ptr = heap->Alloc(100_z);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Used() == x);
//
//            ptr = heap->Alloc(222_z);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Used() == x + y);
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 2));
//
//            void* ptr2 = heap->Alloc(100_z);
//
//            Assert::IsTrue(heap->Used() == x + y + z);
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 3));
//
//            void* ptr3 = heap->Alloc(100_z);
//
//            Assert::IsTrue(heap->Used() == x + y + z + w);
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 4));
//
//            heap->Free(ptr);
//            Assert::IsTrue(heap->Used() == x + z + w);
//
//            heap->Free(ptr2);
//            Assert::IsTrue(heap->Used() == x + w);
//
//            heap->CoalesceAll();
//            Assert::IsTrue(heap->Overhead() == (heap->GetHeaderSize() * 3));
//
//            Heapy::DestroyHeap(heap);
//        }
//
//        TEST_METHOD(UsedData)
//        {
//            string str = "constructed";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Used() == 0_z);
//
//            Heapy::DestroyHeap(heap);
//        }
//
//        TEST_METHOD(AvailableData)
//        {
//            string str = "constructed";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//            size_t size = heap->GetHeaderSize();
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Available() == 1000_z);
//
//            Heapy::DestroyHeap(heap);
//        }
//
//        TEST_METHOD(Overhead)
//        {
//            string str = "constructed";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//            size_t size = heap->GetHeaderSize();
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->Overhead() == 0_z);
//
//            Heapy::DestroyHeap(heap);
//        }
//
//        TEST_METHOD(ContainsPtr)
//        {
//            string str = "constructed";
//            Heapy* heap = Heapy::CreateHeap(str.c_str(), 1000_z);
//
//            Assert::AreEqual((heap->Available() + heap->Used() + heap->Overhead()), 1000_z);
//            Assert::IsTrue(heap->GetName() == str);
//
//            Heapy::DestroyHeap(heap);
//        }
//    };
//}