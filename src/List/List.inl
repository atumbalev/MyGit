#include <cassert>

#include "List.hpp"

template<typename T>
List<T>::List(std::initializer_list<T> list) : m_size(0)
{
    for (auto elem : list)
    {
        push_back(std::move(elem));
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
    if (this != other)
    {
        clear();
        copy(other);
    }

    return (*this);
}

template<typename T>
List<T>& List<T>::operator=(List&& other)
{
    if (this != other)
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
ListIterator<T> List<T>::begin()
{
    return ListIterator(m_first);
}

template<typename T>
ListIterator<T> List<T>::end()
{
    return ListIterator<T>(nullptr);
}

template<typename T>
ListIterator<T> List<T>::last()
{
    return Iterator(m_last);
}

template<typename T>
ListConstIterator<T> List<T>::cbegin() const
{
    return ListConstIterator<T>(m_first);
}

template<typename T>
ListConstIterator<T> List<T>::cend() const
{
    return ListConstIterator<T>(nullptr);
}

template<typename T>
ListConstIterator<T> List<T>::clast() const
{
    return ConstIterator(m_last);
}

template<typename T>
void List<T>::push_front(const T& val)
{
    Node<T>* newNode = new Node(val);

    if (empty())
    {
        m_first = m_last = newNode;
    }
    else
    {
        newNode->next = m_first;
        m_first = newNode;
    }

    ++m_size;
}

template<typename T>
void List<T>::pop_front()
{
    Node<T>* del = m_first;
    m_first = m_first->next;
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

    if (empty())
    {
        m_first = m_last = newNode;
    }
    else
    {
        Node<T>* beforeInserted = const_cast<Node<T>*>(position.m_ptr);
        Node<T>* afterInserted = beforeInserted->next;
        beforeInserted->next = newNode;
        newNode->next = afterInserted;
    }

    ++m_size;
    return Iterator(newNode);
}

template<typename T>
template<typename Head, typename... Tail>
void List<T>::insert_after(ListConstIterator<T> position, Head head, Tail... tail)
{
    insert_after(position, std::move(head));
    insert_after(position, std::move(tail)...);
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
    splice(other, ListConstIterator<T>(m_last));
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
    std::swap(node->data, m_first->data);
    pop_front();
    return Iterator(node->next);
}

template<typename T>
void List<T>::push_back(const T& val)
{
    Node<T>* newNode = new Node<T>(val);
    if (empty())
    {
        m_first = m_last = newNode;
    }
    else
    {
        m_last->next = newNode;
        m_last = newNode;
    }

    ++m_size;
}
