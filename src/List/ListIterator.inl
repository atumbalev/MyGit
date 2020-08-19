#include "ListIterator.hpp"

template<typename T>
ListIterator<T>::ListIterator(nullptr_t) : m_ptr(nullptr) {}

template<typename T>
ListIterator<T>::ListIterator(Node<T>* ptr) : m_ptr(ptr) {}

template<typename T>
T& ListIterator<T>::operator*()
{
    return m_ptr->data;
}

template<typename T>
const T& ListIterator<T>::operator*() const
{
    return m_ptr->data;
}

template<typename T>
T* ListIterator<T>::operator->()
{
    return &(operator*());
}

template<typename T>
const T* ListIterator<T>::operator->() const
{
    return &(operator*());
}

template<typename T>
ListIterator<T>& ListIterator<T>::operator++()
{
    m_ptr = m_ptr->next;
    return (*this);
}

template<typename T>
ListIterator<T> ListIterator<T>::operator++(int)
{
    ListIterator<T> tmp(*this);
    ++(*this);
    return tmp;
}

template<typename T>
bool ListIterator<T>::operator==(const ListIterator& rhs) const
{
    return m_ptr == rhs.m_ptr;
}

template<typename T>
bool ListIterator<T>::operator!=(const ListIterator& rhs) const
{
    return !(rhs == *this);
}
