#include "ListConstIterator.hpp"
template<typename T>
ListConstIterator<T>::ListConstIterator(nullptr_t) : m_ptr(nullptr) {}

template<typename T>
ListConstIterator<T>::ListConstIterator(Node<T>* ptr) : m_ptr(ptr) {}

template<typename T>
const T& ListConstIterator<T>::operator*() const
{
    return m_ptr->data;
}

template<typename T>
const T* ListConstIterator<T>::operator->() const
{
    return &(operator*());
}

template<typename T>
ListConstIterator<T>& ListConstIterator<T>::operator++()
{
    m_ptr = m_ptr->next;
    return (*this);
}

template<typename T>
ListConstIterator<T> ListConstIterator<T>::operator++(int)
{
    ListConstIterator tmp(*this);
    m_ptr = m_ptr->next;
    return tmp;
}

template<typename T>
bool ListConstIterator<T>::operator==(const ListConstIterator& rhs) const
{
    return m_ptr == rhs.m_ptr;
}

template<typename T>
bool ListConstIterator<T>::operator!=(const ListConstIterator& rhs) const
{
    return !(rhs == *this);
}
