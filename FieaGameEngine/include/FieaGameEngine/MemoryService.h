#pragma once
#include "FieaGameEngine/Types.h"
#include "../src/Memory/Track.h"

// new an object of _type, using allocator _al and our new form of operator new
#ifdef FIEA_MEMORY_DEBUG
#define FNEW(_al, _type) new(_al, __FILE__, __LINE__) _type
#define FDELETE(_al, _ptr)                   \
        std::destroy_at(_ptr);                                   \
        ::operator delete((_ptr), (_al), nullptr, 0)
#define FALLOC(_al, _size) _al.Alloc(_size, __FILE__, __LINE__)
#define FFREE(_al, _ptr) _al.Free(_ptr);
#else
#define FNEW(_al, _type) new(_al) _type
#define FDELETE(_al, _ptr) \
        std::destroy_at(_ptr); \
        ::operator delete((_ptr), (_al), nullptr, 0)
#define FALLOC(_al, _size) _al.Alloc(_size);
#define FFREE(_al, _ptr) _al.Free(_ptr);
#endif

#define FPUSH(_iap, _al) _iap.PushDefaultAllocator(_al, __FILE__, __LINE__);
#define FPOP(_iap, _al) _iap.PopDefaultAllocator(_al, __FILE__, __LINE__);

//for when the stack is used, we could make our own push and pop to handle the internal stackinfo
// or we could use this below:
// #define FIEA_MEM_SCOPEPUSH(_al)
// #define PROXY_OBJECT_FOR_PUSHING(_al) ProxyObjectForPushing(_al, __FILE__)
// delete _ptr using _al with the correct operator delete 
// destroy_at(): built-in op to delete the pointer in this case

namespace Fiea::Engine::Memory
{
    class Heap;

    struct Allocator
    {
    public:
        friend class MemoryService;

        Allocator(const Allocator& other) = default;
        Allocator& operator=(const Allocator& rhs) = default;
        ~Allocator() = default;

        [[nodiscard]]
        void* Alloc(size_t size) const; // make an allocation from a heap
#ifdef FIEA_MEMORY_DEBUG
        [[nodiscard]]
        void* Alloc(size_t size, const char* file, size_t line) const; // make an allocation from a heap
#endif
        void Free(void* ptr) const; // free an allocation from a particular heap

    private:
        Allocator() = delete;
#ifdef FIEA_MEMORY_DEBUG
        Allocator(Heap& heap, Tracker& track, const char* label);
        Tracker& _TrackMe;
#else
        Allocator(Heap& heap, const char* label);
#endif

        Heap& _Heap;

        // Can't use this if not static because you can't use member
        // variables at compile time
        static inline const size_t MAX_LABEL_SIZE = 16;

        //const char* _Label; - Can't rely on a pointer since it can be lost with 
        //                      scope
        char _Label[MAX_LABEL_SIZE];
    };

    struct IMemoryService
    {
        virtual Heap* CreateHeap(const char* name, size_t size) = 0; // remove static CreateHeap method from Heap class and implement here
        virtual void DestroyHeap(Heap* heap) = 0; // remove static DestroyHeap method from Heap class and implement here
#ifdef FIEA_MEMORY_DEBUG
        // pass through to Tracker::Count
        virtual size_t CountAllocations(const Heap* heap = nullptr) const = 0; 
        // Output diagnostic data to the provided ostream for the given heap, or all heaps if a heap is not provided
        // Heap Statistics - Name, Used bytes, Free bytes, Overhead bytes
        // Allocation information in CSV via Tracker::Report
        virtual void DumpAllocations(std::ostream& output, const Heap* heap = nullptr) const = 0; 
#endif
    };

    struct IAllocatorProvider
    {
        virtual Allocator GetAllocator(Heap* heap, const char* label = nullptr) = 0; // return an allocator that can allocate from a Heap
        virtual Allocator GetAllocator(const char* heapName, const char* label = nullptr) = 0; // return an allocator that can allocate from a Heap

#ifdef FIEA_MEMORY_DEBUG
        virtual void PushDefaultAllocator(const Allocator&, const char* file, size_t line) = 0;
        virtual void PopDefaultAllocator(const Allocator&, const char* file, size_t line) = 0;
#else
        virtual void PushDefaultAllocator(const Allocator&) = 0;
        virtual void PopDefaultAllocator(const Allocator&) = 0;
#endif
    };

    IMemoryService* GetMemoryService(); // returns the singleton instance as an interface pointer (see below)
    IAllocatorProvider* GetAllocatorProvider(); // returns the singleton instance as an interface pointer (see below)

    void MakeService(); 
    //void CreateService();
    void DestroyService();
    //MemoryService* ms = new MemoryService(); // - can't work 4 some class w/ no definitions
    //IMemoryService* ims = ms; // - can't work 4 some class w/ no definitions
    //IAllocatorProvider* iap = ms; // - can't work 4 some class w/ no definitions
    //Implementor* i = static_cast<Implementor*>(ims);
}

#ifdef FIEA_MEMORY_DEBUG
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& _al, const char* file, size_t line);
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& _al, const char* file, size_t line);
#else
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator&);
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator&);
#endif

/*
* static_cast<>()       // convert from parent/chlid to child/parent w/ standard conversion functions
* // compiler will add in necessary conversion functions and prevent some implicit ones
* reinterpret_cast<>()  // the "very literal" version of static_cast;
* // ignores type conversion warnings and doesn't add in necessary conversion functions, which means it returns the same adderess that was passed in. Just as a different type.
* dynamic_cast<>()      // used for RTTI; 2 types of cast: upcasts(child to parent) and downcasts(parent to child)
* // Upcasts - guaranteed @ compile time so they're completed via static_cast
* // Downcasts - essentially makes an instance of child class 
* const_cast<>()        // only changes const-ness of a type; rarely use this if we do our job right
*/

/* Fake code below
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator&);
void* operator new(size_t size, void* ptr);
new(alloc) Thing();

keyword new (const Allocator& alloc, TYPE)
{
    obj = ::opnew(sizeof(TYPE), alloc);
    obj->TYPE();
    return obj;
}
*/