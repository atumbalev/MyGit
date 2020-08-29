#pragma once

#include <cstddef>
#include <initializer_list>
#include "ListNode.hpp"
#include "ListIterator.hpp"
#include "ListConstIterator.hpp"


///
/// @class Forward Singly Linked List Implementation
/// Supports constant and normal iterators
/// compatible with stl range-based for loops
template <typename T>
class List
{
public:
    using Iterator = ListIterator<T>;
    using ConstIterator = ListConstIterator<T>;

public:
    List() : m_size(0), m_sentinel() {}
    List(std::initializer_list<T> list);
    List(const List<T>& other);
    List(List&& other);
    List& operator=(const List& other);
    List& operator=(List&& other);
    ~List();

    ConstIterator cBeforeBegin() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    Iterator beforeBegin();
    Iterator begin();
    Iterator end();

    Iterator insert_after(ListConstIterator<T> position, const T& value);

    /// Same function as above, but utilizes variadic arguments to be able to insert variable amount of elements
    template<typename Head, typename ...Tail>
    Iterator insert_after(ListConstIterator<T> position, Head head, Tail... tail);

    /// Steals all of @other's elements and inserts them @position
    void splice(List<T>& other, ListConstIterator<T> position);
    void splice(List<T>& other);

    /// Removes the element after @it, keeps list order
    Iterator removeAfter(ListConstIterator<T> it);

    /// Removes whichever @it by swapping it with the first element of the list and popping front
    /// Does NOT keep list order
    /// Makes use as BucketIterator for the hash map easier
    Iterator remove(ListConstIterator<T> it);

    void push_front(const T& val);
    void pop_front();

    size_t size() const;
    bool empty() const;

    void clear();

private:
    Node<T> m_sentinel;
    size_t m_size;

    void copy(const List<T>& other);
    void swap(List& other);

};

#include "List.inl"
#include "ListIterator.hpp"