#pragma once

#include "List.hpp"
#include "ListIterator.hpp"

template <typename T>
class List;

template <typename T>
class ListIterator;

template <typename T>
class ListConstIterator
{
    using iterator_category = std::forward_iterator_tag;
    friend class List<T>;
public:
    ListConstIterator(nullptr_t);
    ListConstIterator(Node<T>* ptr);
    ListConstIterator(ListIterator<T> it) : m_ptr(it.m_ptr) {}

    ListConstIterator() = default;
    ListConstIterator(const ListConstIterator& other) : m_ptr(other.m_ptr) {}
    ListConstIterator& operator=(const ListConstIterator& other) = default;

    ~ListConstIterator() = default;

    const T& operator*() const;

    const T* operator-> () const;

    bool operator==(const ListConstIterator& rhs) const;

    bool operator!=(const ListConstIterator& rhs) const;

    ListConstIterator& operator++();
    ListConstIterator operator++(int);
private:
    const Node<T>* m_ptr;
};

#include "ListConstIterator.inl"