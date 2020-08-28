#include <cassert>

#include "List.hpp"

template<typename T>
List<T>::List(std::initializer_list<T> list) : m_size(0)
{
    Iterator pos = beforeBegin();
    for (auto elem : list)
    {
        pos = insert_after(pos, elem);
    }
}

template<typename T>
List<T>::List(const List<T>& other) : m_size(0)
{
    copy(other);
}

template<typename T>
List<T>::List(List&& other) : m_size(0)
{
    swap(other);
}

template<typename T>
List<T>& List<T>::operator=(const List& other)
{
    if (this != &other)
    {
        clear();
        copy(other);
    }

    return (*this);
}

template<typename T>
List<T>& List<T>::operator=(List&& other)
{
    if (this != &other)
    {
        clear();
        swap(other);
    }

    return (*this);
}

template<typename T>
List<T>::~List()
{
    clear();
}

template<typename T>
typename List<T>::ConstIterator List<T>::cBeforeBegin() const
{
    return ConstIterator(&m_sentinel);
}

template<typename T>
ListIterator<T> List<T>::begin()
{
    return ListIterator(m_sentinel.next);
}

template<typename T>
ListIterator<T> List<T>::end()
{
    return ListIterator<T>(nullptr);
}

template<typename T>
typename List<T>::Iterator List<T>::beforeBegin()
{
    return Iterator(&m_sentinel);
}

template<typename T>
ListConstIterator<T> List<T>::cbegin() const
{
    return ListConstIterator<T>(m_sentinel.next);
}

template<typename T>
ListConstIterator<T> List<T>::cend() const
{
    return ListConstIterator<T>(nullptr);
}

template<typename T>
void List<T>::push_front(const T& val)
{
    insert_after(cBeforeBegin(), val);
}

template<typename T>
void List<T>::pop_front()
{
    assert(!empty());
    Node<T>* del = m_sentinel.next;
    m_sentinel.next = del->next;
    delete del;
    --m_size;
}

template<typename T>
size_t List<T>::size() const
{
    return m_size;
}

template<typename T>
bool List<T>::empty() const
{
    return m_size == 0;
}

template<typename T>
typename List<T>::Iterator List<T>::insert_after(ListConstIterator<T> position, const T& value)
{
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


    Node<T>* beforeInserted = const_cast<Node<T>*>(position.m_ptr);
    Node<T>* afterInserted = beforeInserted->next;
    beforeInserted->next = newNode;
    newNode->next = afterInserted;


    ++m_size;
    return Iterator(newNode);
}

template<typename T>
template<typename Head, typename... Tail>
typename List<T>::Iterator List<T>::insert_after(ListConstIterator<T> position, Head head, Tail... tail)
{
    insert_after(position, std::move(head));
    return insert_after(position, std::move(tail)...);
}

template<typename T>
void List<T>::splice(List<T>& other, ListConstIterator<T> position)
{
    Node<T>* beforeInserted = const_cast<Node<T>*>(position.m_ptr);
    Node<T>* afterInserted = beforeInserted->next;

    beforeInserted->next = other.m_first;
    other.m_last->next = afterInserted;
    m_size += other.m_size;

    other.m_first = other.m_last = nullptr;
    other.m_size = 0;
}

template<typename T>
void List<T>::splice(List<T>& other)
{
    splice(other, cbegin());
}

template<typename T>
typename List<T>::Iterator List<T>::removeAfter(ListConstIterator<T> it)
{
    Node<T>* beforeRemoved = const_cast<Node<T>*>(it.m_ptr);
    Node<T>* removed = beforeRemoved->next;
    Node<T>* afterRemoved = removed->next;

    beforeRemoved->next = afterRemoved;

    delete removed;

    --m_size;

    return Iterator(afterRemoved);
}

template<typename T>
typename List<T>::Iterator List<T>::remove(ListConstIterator<T> it)
{
    assert(!empty());
    Node<T>* node = const_cast<Node<T>*>(it.m_ptr);
    std::swap(node->data, m_sentinel.next->data);
    pop_front();
    return Iterator(node->next);
}

template<typename T>
void List<T>::clear()
{
    while (!empty())
    {
        pop_front();
    }
}

template<typename T>
void List<T>::copy(const List<T>& other)
{
    ListIterator pos = beforeBegin();
    for (ListConstIterator<T> it = other.cbegin(); it != other.cend(); ++it)
    {
        pos = insert_after(pos, *it);
    }
}

template<typename T>
void List<T>::swap(List& other)
{
    std::swap(m_sentinel, other.m_sentinel);
    std::swap(m_size, other.m_size);
}
