#pragma once
#include "../src/Memory/Track.h"

#ifdef FIEA_MEMORY_DEBUG
namespace Fiea::Engine::Memory
{
    void Tracker::Track(void* ptr, const char* heapName, const char* label, const char* file, size_t line)  
    {
        DebugMemoryInfo t;
        t._Ptr = ptr, t._HeapName = heapName, t._Label = label, t._File = file, t._Line = line;
        debugger.emplace(ptr, t);
    }
    void Tracker::Untrack(void* ptr) 
    {
        FIEA_ASSERT(ptr != nullptr);
        debugger.erase(ptr);
    }
    size_t Tracker::Count(const char* heapName) const
    {
        size_t num = 0;
        if (heapName == nullptr)
        {
            for (debugmap::const_iterator dup = debugger.begin(); dup != debugger.end(); ++dup)
            {
                ++num;
            }
            return num;
        }
        else
        {
            for(debugmap::const_iterator dup = debugger.begin(); dup != debugger.end(); ++dup)
            {
                if (dup->second._HeapName == heapName)
                {
                    ++num;
                }
            }
            return num;
        }
    }
    void Tracker::Report(std::ostream& output, const char* heapName) const
    {
        for (debugmap::const_iterator dup = debugger.begin(); dup != debugger.end(); ++dup)
        {
            if (dup->second._HeapName == heapName)
            {
                output << "Heap Report: " << dup->second._Ptr << "," << dup->second._HeapName << "," << dup->second._Label << "," << dup->second._File << "," << dup->second._Line;
            }
        }
    }
}
#endif
