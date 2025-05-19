#pragma once
#include "FieaGameEngine/MemoryService.h"
#include "../src/Memory/Heap.h"
#include "FieaGameEngine/ServiceMgr.h"
#include <map>
#include <stack>

namespace Fiea::Engine::Memory
{
    struct DebugAlloc
    {
        Allocator _al;
#ifdef FIEA_MEMORY_DEBUG
        string file;
        size_t line;
#endif 
    };

    using heapmap = std::map<string, Heap*>;
    using Stack = std::stack<DebugAlloc>;

    struct IDefaultAllocator
    {
        // Perform an allocation using the allocator at the top of the stack of default allocators, or malloc if no allocators have been pushed
        virtual void* DefaultAlloc(size_t size) = 0;
        // Determine the heap that the allocation was made from, and free it
        // Attempt to use free() if the allocation is not from one of our existing Heaps
        virtual void DefaultFree(void* ptr) = 0;
    };

    class MemoryService : public Singleton<MemoryService>, public IMemoryService, public IAllocatorProvider, public IDefaultAllocator
    {
    public:

        //Memory::IMemoryService* memServ = ServiceMgr::ProvideInterface<Memory::IMemoryService>();
        //Memory::IAllocatorProvider* allocProv = ServiceMgr::ProvideInterface<Memory::IAllocatorProvider>();
        //Memory::IDefaultAllocator* defAlloc = ServiceMgr::ProvideInterface<Memory::IDefaultAllocator>();

        // Implement IMemoryService functions as public API
        // remove static CreateHeap method from Heap class and implement here
        
        /// @brief 
        /// @param name 
        /// @param size 
        /// @return 
        [[nodiscard]]
        Heap* CreateHeap(const char* name, size_t size) override
        {
            if (size % Heap::ALIGNMENT > 0)
            {
                size += Heap::ALIGNMENT - (size % Heap::ALIGNMENT);
            }
            void* ptr = malloc(sizeof(Heap) + size);
            Heap* newHead = new(ptr) Heap(name, size);
            string s(name);
            //map.insert(std::make_pair<string, Heap*>(*s, newHead));
            map.emplace(s, newHead);
            return newHead;
        }

        // remove static DestroyHeap method from Heap class and implement here

        /// @brief 
        /// @param heap 
        void DestroyHeap(Heap* heap) override
        {
            if (heap == nullptr)
            {
                return;
            }

            // Destroy heap's reference on the map and then delete that heap from the map if it exists
            heapmap::iterator itr = map.find(heap->GetName());
            FIEA_ASSERT(itr != map.end());
            FIEA_ASSERT(itr->second == heap);
            map.erase(itr->first);
            heap->~Heap();
            free(heap);
        }

        // Implement IAllocatorProvider as public API 
        // return an allocator that can allocate from a Heap

        /// @brief 
        /// @param heap 
        /// @param label 
        /// @return 
        [[nodiscard]]
        Allocator GetAllocator(Heap* heap, const char* label = nullptr) override
        {
            FIEA_ASSERT(heap != nullptr);
#ifdef FIEA_MEMORY_DEBUG
            return Allocator(*heap, pacemaker, label);
#else
            return Allocator(*heap, label);
#endif
        }

        // return an allocator that can allocate from a Heap

        /// @brief 
        /// @param heapName 
        /// @param label 
        /// @return 
        [[nodiscard]]
        Allocator GetAllocator(const char* heapName, const char* label = nullptr) override 
        {
            auto it = map.find(heapName);
            FIEA_ASSERT(map.find(heapName) != map.end());
            Heap* h = it->second;
#ifdef FIEA_MEMORY_DEBUG
            Allocator al(*h, pacemaker, label);
#else
            Allocator al(*h, label);
#endif
            return al;
        }

#ifdef FIEA_MEMORY_DEBUG
        /// @brief 
        /// @param heap 
        /// @return 
        size_t CountAllocations(const Heap* heap = nullptr) const override
        {
            string s(heap->GetName());
            return pacemaker.Count(s.c_str());
        }

        /// @brief 
        /// @param output 
        /// @param heap 
        void DumpAllocations(std::ostream& output, const Heap* heap = nullptr) const override
        {
            if(heap == nullptr)
            {
                for(heapmap::const_iterator itr = map.begin(); itr != map.end(); ++itr)
                {
                    output << "HeapName: " << itr->second->GetName() << ", UsedBytes: " << itr->second->Used() << ", FreeBytes: " << itr->second->Available() << ", HeaderBytes: " << itr->second->Overhead() << std::endl;
                    string s(itr->first);
                    pacemaker.Report(output, s.c_str());
                }
            }
            else
            {
                for (heapmap::const_iterator itr = map.begin(); itr != map.end(); ++itr)
                {
                    if(itr->second == heap)
                    {
                        output << "HeapName: " << heap->GetName() << ", UsedBytes: " << heap->Used() << ", FreeBytes: " << heap->Available() << ", HeaderBytes: " << heap->Overhead() << std::endl;
                        string s(itr->first);
                        pacemaker.Report(output, s.c_str());
                    }
                }
            }
        }
#endif

#ifdef FIEA_MEMORY_DEBUG
        /// @brief 
        /// @param  
        /// @param file 
        /// @param line 
        void PushDefaultAllocator(const Allocator& alloc, const char* file, size_t line) override
        {
            string s(file);
            DebugAlloc types = {alloc, s.c_str(), line};
            myStack.push(types);
        }

        /// @brief 
        /// @param  
        /// @param file 
        /// @param line 
        void PopDefaultAllocator(const Allocator& alloc, const char* file, size_t line) override
        {
            myStack.pop();
        }
#else
        /// @brief 
        /// @param  
        void PushDefaultAllocator(const Allocator& alloc) override
        {
            DebugAlloc types = { alloc };
            myStack.push(types);
        }
        /// @brief 
        /// @param  
        void PopDefaultAllocator(const Allocator& alloc) override
        {
            myStack.pop();
        }
#endif
        /// @brief 
        /// @param size 
        /// @return 
        void* DefaultAlloc(size_t size) override
        {
            if(myStack.empty() != true)
            {
                DebugAlloc& types = myStack.top();
                return FALLOC(types._al, size);
            }
            else
            {
                return nullptr;
            }
        }
        /// @brief 
        /// @param ptr 
        void DefaultFree(void* ptr) override
        {
            //Internal issues here to fix later
            for(heapmap::iterator itr = map.begin(); itr != map.end(); ++itr)
            {
                if(itr->second->Contains(ptr) == true)
                {
                    itr->second->Free(ptr);
                    return;
                }
            }
            free(ptr);
        }
        /// @brief 
        /// @return 
        static IDefaultAllocator* GetDefaultAllocator()
        {
            // returns the singleton instance as an interface pointer
            return static_cast<IDefaultAllocator*>(MemoryService::Instance());
        }

    private:
        heapmap map;
        Stack myStack;
#ifdef FIEA_MEMORY_DEBUG
        Tracker pacemaker;
#endif // FIEA_MEMORY_DEBUG

    };

#ifdef FIEA_MEMORY_DEBUG
    /// @brief 
    /// @param heap 
    /// @param label 
    Allocator::Allocator(Heap& heap, Tracker& track, const char* label) : _Heap(heap), _TrackMe(track)
    {
        if (label != nullptr)
        {
            strncpy(_Label, label, MAX_LABEL_SIZE);
        }
        _Label[MAX_LABEL_SIZE - 1] = '\0';
    }
    /// @brief 
    /// @param size 
    /// @return 
    void* Allocator::Alloc(size_t size, const char* file, size_t line) const
    {
        void* ptr = _Heap.Alloc(size);
        string s(_Heap.GetName());
        _TrackMe.Track(ptr, s.c_str(), _Label, file, line);
        return ptr;
    }

    /// @brief 
    /// @param ptr 
    void Allocator::Free(void* ptr) const
    {
        FIEA_ASSERT(ptr != nullptr);
        _TrackMe.Untrack(ptr);
        _Heap.Free(ptr);
    }
#else
    /// @brief 
    /// @param heap 
    /// @param label 
    Allocator::Allocator(Heap& heap, const char* label) : _Heap(heap)
    {
        if (label != nullptr)
        {
            strncpy(_Label, label, MAX_LABEL_SIZE);
        }
        _Label[MAX_LABEL_SIZE - 1] = '\0';
    }

    /// @brief 
    /// @param size 
    /// @return 
    void* Allocator::Alloc(size_t size) const
    {
        return _Heap.Alloc(size);
    }
    /// @brief 
    /// @param ptr 
    void Allocator::Free(void* ptr) const
    {
        FIEA_ASSERT(ptr != nullptr);
        _Heap.Free(ptr);
    }
#endif // FIEA_MEMORY_DEBUG

    /// @brief 
    /// @return 
    IMemoryService* GetMemoryService()
    {
        // Make sure to IMemoryService's vtable w/ the right cast
        // If your getting the wrong function here, double check the returned address w/ the struct layout(Alt + L), the disassembly panel(Ctrl + K, G), and the call stack window(Ctrl + Alt + C)
        return static_cast<IMemoryService*>(MemoryService::Instance());
    }

    /// @brief 
    /// @return 
    IAllocatorProvider* GetAllocatorProvider()
    {
        // Make sure to IAllocatorProvider's vtable w/ the right cast
        // If your getting the wrong function here, double check the returned address w/ the struct layout(Alt + L), the disassembly panel(Ctrl + K, G), and the call stack window(Ctrl + Alt + C)
        return static_cast<IAllocatorProvider*>(MemoryService::Instance());
    }

    /// @brief 
    void MakeService()
    {
        MemoryService::Create();
    }

    /// @brief 
    void DestroyService()
    {
        FIEA_ASSERT(MemoryService::Instance() != nullptr);
        MemoryService::Destroy();
    }
}

#ifdef FIEA_MEMORY_DEBUG
/// @brief 
/// @param size 
/// @param _al 
/// @param file 
/// @param line 
/// @return 
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& _al, const char* file, size_t line)
{
    return FALLOC(_al, size);
}

/// @brief 
/// @param ptr 
/// @param _al 
/// @param file 
/// @param line 
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& _al, const char* file, size_t line)
{
    FIEA_ASSERT(ptr != nullptr);
    _al.Free(ptr);
}
#else

/// @brief 
/// @param size 
/// @param alloc 
/// @return 
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& alloc)
{
    return alloc.Alloc(size);
}
/// @brief 
/// @param ptr 
/// @param alloc 
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& alloc)
{
    FIEA_ASSERT(ptr != nullptr);
    alloc.Free(ptr);
}
#endif // FIEA_MEMORY_DEBUG
//
//_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size)
//void* operator new(size_t size)
//{
//    FIEA_ASSERT(size != 0);
//    if (size == 0)
//    {
//        ++size;
//    }
//
//    //Call GetDefaultAllocator under here when you're ready
//    auto* memService = Fiea::Engine::Memory::MemoryService::GetDefaultAllocator();
//    if(memService != nullptr)
//    {
//        void* ptr = memService->DefaultAlloc(size);
//        if(ptr != nullptr)
//        {
//            return ptr;
//        }
//    }
//
//    void* ptr = malloc(size);
//    if(ptr != nullptr)
//    {
//        return ptr;
//    }
//
//    throw std::bad_alloc();
//}
//
///// @brief 
///// @param ptr 
//void operator delete(void* ptr)
//{
//    // Buggy code - pls go over how to get this working
//    auto* ida = Fiea::Engine::Memory::MemoryService::GetDefaultAllocator();
//    if(ida != nullptr)
//    {
//        ida->DefaultFree(ptr);
//    }
//}