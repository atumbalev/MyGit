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
    using Iterator = ListIterator<T>;
    using ConstIterator = ListConstIterator<T>;

public:
    List() : m_first(nullptr), m_last(nullptr), m_size(0) {}
    List(std::initializer_list<T> list);
    List(const List<T>& other);
    List(List&& other);
    List& operator=(const List& other);
    List& operator=(List&& other);
    ~List();

    ConstIterator cbegin() const;
    ConstIterator cend() const;
    ConstIterator clast() const;

    Iterator begin();
    Iterator end();
    Iterator last();

    Iterator insert_after(ListConstIterator<T> position, const T& value);

    template<typename Head, typename ...Tail>
    void insert_after(ListConstIterator<T> position, Head head, Tail... tail);

    void splice(List<T>& other, ListConstIterator<T> position);
    void splice(List<T>& other);

    Iterator removeAfter(ListConstIterator<T> it);
    Iterator remove(ListConstIterator<T> it);

    void push_front(const T& val);
    void push_back(const T& val);
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
    Node<T>* m_first;
    Node<T>* m_last;
    size_t m_size;

    void copy(const List<T>& other)
    {
        for (ListConstIterator<T> it = other.cbegin(); it != other.cend(); ++it)
        {
            push_back(*it);
        }
    }

    void swap(List& other)
    {
        std::swap(m_first, other.m_first);
        std::swap(m_last, other.m_last);
        std::swap(m_size, other.m_size);
    }

};

#include "List.inl"
#include "ListIterator.hpp"