#pragma once

namespace Fiea::Engine 
{
    template <typename T>
    class List 
    {
        using size_type = size_t;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
    private:
        struct Node
        {
            value_type data;
            Node* next;
            Node* prev;

            Node(const value_type& value);
        };

        Node* _head;
        Node* _tail;
        Node* _backend;
        size_type _size;

        void clear();

    public:
        List();
        List(const List&);
        List(std::initializer_list<value_type> IList); // constructor which accepts an initializer_list of elements and populates the list with them
        ~List();
        List& operator=(const List&);

        //Iterators
        class Iterator 
        {
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;

        public:
            Iterator();
            Iterator(Node*);
            Iterator(const Iterator&);

            friend bool operator==(const Iterator& ita, const Iterator& itb)
            {
                if (ita._node == nullptr || itb._node == nullptr)
                    return false;
                else
                    return (ita._node == itb._node);
            }
            friend bool operator!=(const Iterator& ita, const Iterator& itb)
            {
                if (ita._node == nullptr || itb._node == nullptr)
                    return false;
                else
                    return (ita._node != itb._node);
            }

            reference operator*();
            const_reference operator*() const;
            pointer operator->();
            const_pointer operator->() const;

            Iterator& operator++();
            Iterator operator++(int);
            Iterator& operator--();
            Iterator operator--(int);
        private:
            Node* _node;
        };

        Iterator begin() const noexcept;
        Iterator end() const noexcept;

        Iterator insert(Iterator where, const value_type& val); // for inserting an element at a specific position
        Iterator erase(Iterator where); // for removing an element at a specific position

        //Setters
        void push_back(const value_type& value);
        void pop_back();
        void push_front(const value_type& value);
        void pop_front();

        //Getters
        value_type& back();
        const value_type& back() const;
        value_type& front();
        const value_type& front() const;

        //Info
        size_type size() const;
        bool empty() const;
    };
}

#include "FieaGameEngine/List.inc"

