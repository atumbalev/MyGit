#pragma once


#include <cstddef>
#include <iterator>
#include "ListNode.hpp"
#include "ListConstIterator.hpp"

template <typename T>
class ListConstIterator;

template <typename T>
class ListIterator
{
    friend class ListConstIterator<T>;
    using iterator_category = std::forward_iterator_tag;
public:
    ListIterator(nullptr_t);
    ListIterator(Node<T>* ptr);

    ListIterator(const ListIterator& other) = default;
    ListIterator& operator=(const ListIterator& other) = default;
    ~ListIterator() = default;

    bool operator==(const ListIterator& rhs) const;

    bool operator!=(const ListIterator& rhs) const;

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator-> () const;

    ListIterator& operator++();
    ListIterator operator++(int);
private:
    Node<T>* m_ptr;
};

#include "ListIterator.inl"

