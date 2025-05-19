#pragma once
#include"FieaGameEngine/Types.h"

namespace Fiea::Engine::Memory
{
    class Heap
    {
    private:
        Heap(const char* name, size_t size); // private, will only be invoked from CreateHeap, may add params if needed
        Heap(const Heap&) = delete;
        Heap& operator=(const Heap&) = delete;
        ~Heap(); // not virtual, what modifier should we consider for the class declaration?

        // prevent copy assignment or copy construction... we have no application for copying a heap
        static const size_t ALIGNMENT = 4;                  // you may pad your allocations to increments of ALIGNMENT
        //   you may also force (with code) a heap size that prevents "leftover" blocks that are too small to allocate
        //   if desired, you could instead take alignment as a heap parameter and make alignment a property of the heap
        //   in either case, i recommend enforcing that alignment is a multiple of your header size... but why?
        struct Header 
        {
            // internal constructors create the header
            Header(size_t s = 0);
            Header(const Header&) = delete;
            Header& operator=(const Header&) = delete;
            ~Header();

            // internal variables refer to the actual data from an image header
            size_t _Size;
            bool _Empty;
            Header* _Next;
        };                                // ideal use-case for private nested class
        void _DestroyHeader(Header*& block);
        void* _TryAlloc(Header* current, size_t size);  // attempt an allocation of the requested size... what to return, on failure?
        void _CoalesceOne(Header* block);                                //Make Coalesce function for merging adjacent free blocks together
        Header* _Next(Header* current) const;           // helper to get from one block to the next... could this function as a loop increment?
        string _name;                                   // name of the heap
        Header _Start;                                  // by making this the last member field, may be able to improve code readability
    public:
        friend class MemoryService;

        const std::string& GetName() const;       // simple accessor
        const size_t GetHeaderSize() const;      // simple accessor
        void* Alloc(size_t size);            // allocate memory
        void Free(void* ptr);                // free memory
        void CoalesceAll();                  //Make Coalesce function for merging adjacent free blocks together
        size_t Align(size_t x);              //Align your size with the class alignment field for comparisons
        size_t Used() const;                 // how many bytes are currently allocated
        size_t Available() const;            // how many bytes are currently free
        size_t Overhead() const;             // how many bytes are currently devoted to Headers or other overhead
        // note: the sum of these should match the requested size for the heap (including any padding for alignment)
        bool Contains(const void* ptr) const;// does the requested pointer fall into the range of memory the heap owns? (may return true even if the ptr does not point to an active block!) 
    };
}

