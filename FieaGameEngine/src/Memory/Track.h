#pragma once
#include "FieaGameEngine/Types.h"
#include "../src/Memory/Heap.h"
#include <ostream>
#include <map>

namespace Fiea::Engine::Memory
{
#ifdef FIEA_MEMORY_DEBUG
    class Tracker
    {
    public:
        void Track(void* ptr, const char* heapName, const char* label, const char* file, size_t line); // record the information for this allocation
        void Untrack(void* ptr); // discard the information for this allocation
        size_t Count(const char* heapName = nullptr) const; // return the number of recorded allocations in the requested heap, or all heaps if none is provided
        void Report(std::ostream& output, const char* heapName = nullptr) const;  // output the debug information in CSV for all of the allocations in the requested heap, or all heaps if none is provided
    private:
        struct DebugMemoryInfo 
        {
            void* _Ptr;
            const char* _HeapName;
            const char* _Label;
            const char* _File;
            size_t _Line;
        };
        using debugmap = std::map<void*, DebugMemoryInfo>;
        debugmap debugger;
    };
#endif
}