#pragma once

#include "FieaGameEngine/List.h"

namespace Fiea::Engine
{
    template <typename T>
    List<T>::List() : _head(nullptr), _tail(nullptr), _backend(new Node("")), _size(0) {}

    template <typename T>
    List<T>::List(const List& oldList) : _head(nullptr), _tail(nullptr), _backend(new Node("")), _size(0)
    {
        for (List<T>::Iterator itr = oldList.begin(); itr != oldList.end(); ++itr)
        {
            push_back(*itr);
        }
    }

    template <typename T>
    List<T>::List(std::initializer_list<T> IList) : _head(nullptr), _tail(nullptr), _backend(new Node("")), _size(0)
    {
        for (List<T>::Iterator itr = IList.begin(); itr != IList.end(); ++itr)
        {
            push_back(*itr);
        }
    }

    template <typename T>
    List<T>::~List()
    {
        clear();
    }

    template <typename T>
    List<T>& List<T>::operator=(const List<T>& list)
    {
        if (list._size == 0 && (list._head == nullptr || list._tail == nullptr))
            return *this;
        else if (list._size == _size && list._head->data == _head->data && list._tail->data == _tail->data)
            return *this;

        //Clear out current List data before adding new data back in
        clear();

        _head = _tail = nullptr;
        _backend = new Node(""); 
        _size = 0;

        for (List<T>::Iterator itr = list.begin(); itr != list.end(); ++itr) 
        {
            push_back(*itr);
        }

        return *this;
    }

    template <typename T>
    void List<T>::push_back(const value_type& value)
    {
        if (_tail == nullptr) {
            _tail = new Node(value);
            _tail->data = value;
            _tail->next = _backend;
            _backend->prev = _head = _tail;
        }
        else {
            Node* node = new Node(value);
            _tail->next = node;
            node->next = _backend;
            node->prev = _tail;
            _backend->prev = _tail = node;
        }
        _size++;
    }

    template <typename T>
    void List<T>::pop_back()
    {
        if (_size > 1) {
            Node* temp = _tail->prev;
            temp->next = _backend;
            delete _tail;
            _backend->prev = _tail = temp;
            _size--;
        }
        else{
            Node* temp = _tail;
            _backend->prev = _head = _tail = nullptr;
            delete temp;
            _size = 0;
        }
    }

    template <typename T>
    void List<T>::push_front(const value_type& value)
    {
        if (_head == nullptr) {
            _head = new Node(value);
            _head->data = value;
            _backend->prev = _tail = _head;
            _tail->next = _backend;
        }
        else {
            Node* node = new Node(value);
            _head->prev = node;
            node->next = _head;
            _head = node;
        }
        _size++;
    }

    template <typename T>
    void List<T>::pop_front()
    {
        if (_size > 1) {
            Node* temp = _head->next;
            temp->prev = nullptr;
            delete _head;
            _head = temp;
            _size--;
        }
        else {
            Node* temp = _head;
            _backend->prev = _head = _tail = nullptr;
            delete temp;
            _size = 0;
        }
    }

    template <typename T>
    typename List<T>::value_type& List<T>::back()
    {
        if(_tail == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Invalid back call. No data left.");
        }
        return _tail->data;
    }

    template <typename T>
    const typename List<T>::value_type& List<T>::back() const
    {
        if (_tail == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Invalid back call. No data left.");
        }
        return _tail->data;
    }

    template <typename T>
    typename List<T>::value_type& List<T>::front()
    {
        if (_head == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Invalid front call. No data left.");
        }
        return _head->data;
    }

    template <typename T>
    const typename List<T>::value_type& List<T>::front() const
    {
        if (_head == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Invalid front call. No data left.");
        }
        return _head->data;
    }

    template <typename T>
    typename List<T>::size_type List<T>::size() const
    {
        return _size;
    }

    template <typename T>
    bool List<T>::empty() const
    {
        return (size() == 0);
    }

    //Iterator Funcs
    template <typename T>
    typename List<T>::Iterator List<T>::begin() const noexcept
    {
        Iterator ita(_head);
        return ita;
    }

    template <typename T>
    typename List<T>::Iterator List<T>::end() const noexcept
    {
        Iterator ita(_backend);
        return ita;
    }

    template<typename T>
    typename List<T>::Iterator List<T>::insert(Iterator where, const value_type& val)
    {
        Node* newnode = new Node(val);

        for(Node* nodeItr = _head; nodeItr != _tail->next; nodeItr = nodeItr->next)
        {
            if(&nodeItr == &(*where))
            {
                newnode->prev = nodeItr;
                newnode->next = nodeItr->next;
                nodeItr->next = newnode;
                nodeItr = newnode->next;
                nodeItr->prev = newnode;
                break;
            }
        }
        Iterator itr(newnode);
        return itr;
    }

    template<typename T>
    typename List<T>::Iterator List<T>::erase(Iterator where)
    {
        Node* nodeprev;
        Node* nodenext;
        for (Node* nodeItr = _head; nodeItr != _tail->next; nodeItr = nodeItr->next)
        {
            if (&nodeItr == &(*where))
            {
                nodeprev = nodeItr->prev;
                nodenext = nodeItr->next;
                nodeprev->next = nodenext;
                nodenext->prev = nodeprev;
                nodeItr->prev = nodeItr->next = nullptr;
                delete nodeItr;
                break;
            }
        }
        //return the node that replaced the original's spot
        Iterator itr(nodenext);
        return itr;
    }

    template<typename T>
    List<T>::Iterator::Iterator() : _node(nullptr) {}

    template<typename T>
    List<T>::Iterator::Iterator(Node* priorNode) : _node(priorNode) {}

    template<typename T>
    List<T>::Iterator::Iterator(const Iterator& itr) : _node(itr._node) {}

    template <typename T>
    typename List<T>::reference List<T>::Iterator::operator*()
    {
        if(_node == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Invalid null iterator dereference here with *.");
        }
        return _node->data;
    }
    template <typename T>
    typename List<T>::const_reference List<T>::Iterator::operator*() const
    {
        if (_node == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Invalid null iterator dereference here with * const.");
        }
        return _node->data;
    }
    template <typename T>
    typename List<T>::pointer List<T>::Iterator::operator->()
    {
        if (_node == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Invalid null iterator dereference here with ->.");
        }
        return &(_node->data);
    }
    template <typename T>
    typename List<T>::const_pointer List<T>::Iterator::operator->() const
    {
        if (_node == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Invalid null iterator dereference here with -> const.");
        }
        return &(_node->data);
    }

    template <typename T>
    typename List<T>::Iterator& List<T>::Iterator::operator++()
    {
        if(_node->next == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Out of Bounds iterator here. Use end() to not let this happen again.");
        }
        _node = _node->next;
        return *this;
    }
    template <typename T>
    typename List<T>::Iterator List<T>::Iterator::operator++(int)
    {
        if (_node->next == nullptr)
        {
            //throw an exception here
            throw std::runtime_error("Out of Bounds iterator here. Use end() to not let this happen again.");
        }
        Iterator temp(_node);
        _node = _node->next;
        return temp;
    }
    template <typename T>
    typename List<T>::Iterator& List<T>::Iterator::operator--()
    {
        if (_node->prev != nullptr)
        {
            _node = _node->prev;
        }
        return *this;
    }
    template <typename T>
    typename List<T>::Iterator List<T>::Iterator::operator--(int)
    {
        if (_node->prev != nullptr)
        {
            Iterator temp(_node);
            _node = _node->prev;
            return temp;
        }
        return *this;
    }

    template <typename T>
    List<T>::Node::Node(const value_type& value) : data(value), next(nullptr), prev(nullptr) {}

    template <typename T>
    void List<T>::clear()
    {
        if (_head != nullptr && _tail != nullptr && _size != 0)
        {
            for (Node* node = _head; node != _tail->next; node = node->next)
            {
                //Check if values are properly copied over
                if (node->prev != nullptr)
                {
                    Node* temp = node->prev;
                    temp->next = nullptr;
                    delete temp;
                }
                node->prev = nullptr;
            }
            _tail->next = nullptr;
            delete _tail;
            //_head = _tail = nullptr;
            _size = 0;
        }
        _backend->prev = nullptr;
        delete _backend;
        //_backend = nullptr;
    }
}