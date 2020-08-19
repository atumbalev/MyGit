#pragma once

#include <cstddef>
#include <initializer_list>
#include "ListNode.hpp"
#include "ListIterator.hpp"
#include "ListConstIterator.hpp"

template <typename T>
class List
{
public:
    List() : first(nullptr), last(nullptr), m_size(0) {}
    List(std::initializer_list<T> list);
    List(const List<T>& other) : m_size(0)
    {
        copy(other);
    }

    ~List()
    {
        clear();
    }

    ListConstIterator<T> cbegin() const;
    ListConstIterator<T> cend() const;

    ListIterator<T> begin();
    ListIterator<T> end();

    void insert_after(ListConstIterator<T> position, const T& value)
    {
        Node<T>* beforeInserted = const_cast<Node<T>*>(position.m_ptr);
        Node<T>* afterInserted = beforeInserted->next;
        Node<T>* newNode;

        try
        {
            newNode = new Node<T>(value);
        }
        catch (const std::bad_alloc& er)
        {
            delete newNode;
            throw er;
        }

        beforeInserted->next = newNode;
        newNode->next = afterInserted;
    }

    template<typename Head, typename ...Tail>
    void insert_after(ListConstIterator<T> position, Head head, Tail... tail)
    {
        insert_after(position, std::move(head));
        insert_after(position, std::move(tail)...);
    }

    void splice(List<T>& other, ListConstIterator<T> position)
    {
        Node<T>* beforeInserted = const_cast<Node<T>*>(position.m_ptr);
        Node<T>* afterInserted = beforeInserted->next;

        beforeInserted->next = other.first;
        other.last->next = afterInserted;
        m_size += other.m_size;

        other.first = other.last = nullptr;
        other.m_size = 0;
    }

    void splice(List<T>& other)
    {
        splice(other, ListConstIterator<T>(last));
    }

    void removeAfter(ListConstIterator<T> it)
    {
        Node<T>* beforeRemoved = const_cast<Node<T>*>(it.m_ptr);
        Node<T>* removed = beforeRemoved->next;
        Node<T>* afterRemoved = removed->next;

        beforeRemoved->next = afterRemoved;

        delete removed;
    }

    void remove(const T& value)
    {
        for (ListConstIterator<T> it = cbegin(); it != ListConstIterator(last); ++it)
        {
            if (it.m_ptr->next->data == value)
            {
                removeAfter(it);
                return;
            }
        }
    }

    void push_front(const T& val);
    void push_back(const T& val)
    {
        Node<T>* newNode = new Node<T>(val);
        if (empty())
        {

        }

        last->next = newNode;
        last = newNode;
    }
    void pop_front();

    size_t size() const;
    bool empty() const;

    void clear()
    {
        while (!empty())
        {
            pop_front();
        }
    }

private:
    Node<T>* first;
    Node<T>* last;
    size_t m_size;

    void copy(const List<T>& other)
    {
        for (ListConstIterator<T> it = other.cbegin(); it != other.cend(); ++it)
        {
            push_front(*it);
        }
    }

};

#include "List.inl"
#include "ListIterator.hpp"