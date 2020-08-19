#include "List.hpp"

template<typename T>
List<T>::List(std::initializer_list<T> list) : m_size(0)
{
    for (auto elem : list)
    {
        push_front(std::move(elem));
    }
}

template<typename T>
ListIterator<T> List<T>::begin()
{
    return ListIterator(first);
}

template<typename T>
ListIterator<T> List<T>::end()
{
    return ListIterator<T>(nullptr);
}

template<typename T>
ListConstIterator<T> List<T>::cbegin() const
{
    return ListConstIterator<T>(first);
}

template<typename T>
ListConstIterator<T> List<T>::cend() const
{
    return ListConstIterator<T>(nullptr);
}

template<typename T>
void List<T>::push_front(const T& val)
{
    Node<T>* newNode = new Node(val);

    if (empty())
    {
        first = last = newNode;
    }
    else
    {
        newNode->next = first;
        first = newNode;
    }

    ++m_size;
}

template<typename T>
void List<T>::pop_front()
{
    Node<T>* del = first;
    first = first->next;
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
