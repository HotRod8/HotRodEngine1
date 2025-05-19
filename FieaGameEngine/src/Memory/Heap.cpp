#pragma once

#include "Heap.h"
namespace Fiea::Engine::Memory
{
    // Private methods

    Heap::Heap(const char* name, size_t size)
    {
        _name = name;
        _Start._Size = size;
    }

    /// @brief 
    Heap::~Heap()
    {
        //_DestroyHeader(_Start._Next);
        //_Start.~Header();
    }

    //if (&_Start == nullptr)
    //{
    //    return;
    //}

    //Header* prev = &_Start;
    //for (Header* block = &_Start; block->_Next != nullptr; block = block->_Next)
    //{
    //    if(prev->_Next == block)
    //    {
    //        prev->~Header();
    //        prev = block;
    //    }
    //    block->_Size = 0;
    //    block->_Empty = true;
    //}
    //prev->~Header();

    Heap::Header::Header(size_t s) : _Empty(true), _Size(s), _Next(nullptr){}

    Heap::Header::~Header(){}

    /// @brief Recursive function that deletes every header created in the heap.
    /// @param block The Header* parameter to be destroyed. The first one that points to nullptr will be destroyed first and the rest will be destroyed soon afterwards.
    void Heap::_DestroyHeader(Header*& block)
    {
        if(block == nullptr)
        {
            return;
        }

        _DestroyHeader(block->_Next);
        block->~Header();
    }

    /// @brief Internal recursive allocation function that will reserve any free size remaining in the heap for the new header allocation.
    /// @param current - A Header* parameter to grab a Header address and see if it's already full or not.
    /// @param size - The data size that may be reserved by the new header if within range.
    /// @return         A pointer to the data block of the new header.
    void* Heap::_TryAlloc(Header* current, size_t size)
    {
        if(current == nullptr)
        {
            return nullptr;
        }
        if(!current->_Empty)
        {
            return _TryAlloc(current->_Next, size);
        }
        if(current->_Size == size)
        {
            return (void*)(current + sizeof(Header));
        }
        if(current->_Size < size)
        {
            return _TryAlloc(current->_Next, size);
        }

        //If we get to this part of the code, we successfully reached an appropriate size of data
        // 
        //When doing the replacement new, start at current. Then add the size of the header and the size of the data
        //to point to the beginning of the next block in the inline heap allocation
        current->_Next = new(current + sizeof(Header) + size) Header(current->_Size - size - sizeof(Header));
        current->_Size = size;
        current->_Empty = false;
        //Return a pointer to the data block
        return (current + sizeof(Header));
    }

    /// @brief          Merges a pair of consecutive free blocks together, which knocks out a header in the heap and adds the sizes together
    /// @param block - The free header that will merge with its next header.
    void Heap::_CoalesceOne(Header* block)
    {
        Header* nxtBlock = nullptr;

        nxtBlock = block->_Next;
        block->_Next = nxtBlock->_Next;
        block->_Size += (sizeof(Header) + nxtBlock->_Size);
    }

    /// @brief          Gives you the passed header's next header
    /// @param current - The header to be iterated on.
    /// @return         The next header is passed back to you as a pointer. You are required to catch it.
    [[nodiscard]]
    Heap::Header* Heap::_Next(Header* current) const
    {
        return current->_Next;
    }

    // Public methods

    /// @brief      Creates a brand-new block of memory to shove your data into with malloc. You must call DestroyHeap to delete the heap.
    /// @param name - Simply pass in a string to name the heap. If using new for a string, make sure to delete it yourself.
    /// @param size - Size of the heap. Auto-aligned internally. When making the heap size, call Align() during initiallization to set the size appropriately.
    /// @return     The allocated heap is passed back to you as a pointer. You are required to catch it.
    //[[nodiscard]]
    //Heap* Heap::CreateHeap(const char* name, size_t size)
    //{
    //    if(size % ALIGNMENT > 0)
    //    {
    //        size += ALIGNMENT - (size % ALIGNMENT);
    //    }
    //    void* ptr = malloc(sizeof(Heap) + size);
    //    Heap* newHead = new(ptr) Heap(name, size);
    //    return newHead;
    //}

    /// @brief      Heap Desructor that must be called manually to delete a heap.
    /// @param heap - Destroys the heap passed in.
    //void Heap::DestroyHeap(Heap* heap)
    //{
    //    if(heap == nullptr)
    //    {
    //        return;
    //    }

    //    heap->~Heap();
    //    free(heap);
    //}

    /// @brief  Simple name accessor.
    /// @return Get a name string.
    const std::string& Heap::GetName() const
    {
        return _name;
    }

    /// @brief  Simple header size accessor
    /// @return Returns the header size as a size_t.
    const size_t Heap::GetHeaderSize() const
    {
        return sizeof(Header);
    }

    /// @brief      Allocates the header with the passed in size
    /// @param size - Size of the new header. Size is auto-aligned inside this function.
    /// @return Returns a pointer to the data block of your new header.
    void* Heap::Alloc(size_t size)
    {
        if (size % ALIGNMENT > 0)
        {
            size += ALIGNMENT - (size % ALIGNMENT);
        }
        return _TryAlloc(&_Start, size);
    }

    /// @brief Free here only frees specific headers and their data blocks. Then calls coalesce to handle contiguously free blocks
    /// @param ptr - You're supposed to pass in the data block to free. Will not accept anything else.
    void Heap::Free(void* ptr)
    {
        if(ptr == nullptr)
        {
            return;
        }

        for (Header* block = &_Start; block != nullptr; block = block->_Next)
        {
            if (ptr == (void*)(block + GetHeaderSize())) 
            {
                block->_Size = 0;
                block->_Empty = true;
                if(block->_Next->_Empty == true)
                {
                    //Coalesce the data before allocating new data if there are multiple blocks
                    _CoalesceOne(block);
                }
            }
        }
    }
    //If multiple headers have been freed contiguously, merge them together.

    /// @brief Merges all pairs of consecutive free blocks together, which knocks out one header at a time in the heap and adds the sizes together
    void Heap::CoalesceAll()
    {
        Header* prevEmpty = nullptr;
        for (Header* block = &_Start; block != nullptr; block = block->_Next)
        {
            if (block->_Empty == true && prevEmpty == nullptr)
            {
                prevEmpty = block;
            }
            else if (block->_Empty == true && prevEmpty->_Next == block)
            {
                _CoalesceOne(prevEmpty);
            }
            else if (block->_Empty == true)
            {
                prevEmpty = block;
            }
        }
    }

    /// @brief Aligns any size to the Heap Constructor and Allocator's required alignment
    /// @param x - Aligns this size appropriately
    /// @return  Returns aligned size.
    size_t Heap::Align(size_t x)
    {
        if (x % ALIGNMENT > 0)
        {
            return (x + (ALIGNMENT - (x % ALIGNMENT)));
        }
        else return x;
    }

    /// @brief This function calculates the amount of used data bytes in the heap.
    /// @return Return the number of used data in the heap.
    size_t Heap::Used() const
    {
        size_t storedspace = 0;
        // const_cast<Header*> doesn't work because it will create a brand-new pointer for your _Start header,
        // thus you will not be referencing any other headers from that new address
        for(const Header* x = &_Start; x != nullptr; x = x->_Next)
        {
            if(!x->_Empty)
            {
                storedspace += x->_Size;
            }
        }
        return storedspace;
    }

    /// @brief This function calculates the amount of unused data bytes in the heap.
    /// @return Return the number of free data in the heap.
    size_t Heap::Available() const
    {
        size_t openspace = 0;
        // const_cast<Header*> doesn't work because it will create a brand-new pointer for your _Start header,
        // thus you will not be referencing any other headers from that new address
        for (const Header* x = &_Start; x != nullptr; x = x->_Next)
        {
            //However, we can change the pointer value to move the next pointer and check its data w/ no issues.
            //Cuz (const Header)* keeps the values const while allowing us to modify the pointer to move to the next pointer
            if (x->_Empty)
            {
                openspace += x->_Size;
            }
        }
        return openspace;
    }

    /// @brief This function calculates the amount of data bytes in the heap headers.
    /// @return Return the number of data used by the heap headers.
    size_t Heap::Overhead() const
    {
        size_t headerspace = 0;
        for (Header* x = _Start._Next; x != nullptr; x = x->_Next)
        {
            headerspace += sizeof(Header);
        }
        return headerspace;
    }

    /// @brief Checks if the passed pointer is contained within the heap.
    /// @param ptr - Pointer location to check.
    /// @return Return if the ptr is in the heap or not.
    bool Heap::Contains(const void* ptr) const
    {
        const Header* beginning = &_Start;
        //Same thing as (const Header)* comment.
        const void* end = beginning + Used() + Available() + Overhead();
        if(ptr >= (const void*)beginning && ptr < end)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
