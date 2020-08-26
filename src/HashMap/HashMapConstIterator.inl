#include "HashMapConstIterator.hpp"

template<typename K, typename T, typename HashFunction>
HashMapConstIterator<K, T, HashFunction>::HashMapConstIterator(const HashMapConstIterator::HashTable& tableRef): table(&tableRef) {
    bucket = table->cbegin();
    element = bucket->cbegin();
    findNextValid();
}

template<typename K, typename T, typename HashFunction>
HashMapConstIterator<K, T, HashFunction>::HashMapConstIterator(const HashMapConstIterator::HashTable& table, HashMapConstIterator::BucketConstIterator bucket,
                                                     HashMapConstIterator::ElementConstIterator element) : table(&table), bucket(bucket), element(element) {}

template<typename K, typename T, typename HashFunction>
const std::pair<K, T> & HashMapConstIterator<K, T, HashFunction>::operator*() const
{
    return (*element);
}

template<typename K, typename T, typename HashFunction>
const std::pair<K, T>* HashMapConstIterator<K, T, HashFunction>::operator->() const
{
    return &(operator*());
}

template<typename K, typename T, typename HashFunction>
HashMapConstIterator<K, T, HashFunction> HashMapConstIterator<K, T, HashFunction>::operator++(int)
{
    HashMapConstIterator copy(*this);
    advance();
    return copy;
}

template<typename K, typename T, typename HashFunction>
HashMapConstIterator<K, T, HashFunction>& HashMapConstIterator<K, T, HashFunction>::operator++()
{
    advance();
    return *this;
}

template<typename K, typename T, typename HashFunction>
bool HashMapConstIterator<K, T, HashFunction>::operator==(const HashMapConstIterator& other) const
{
    return table == other.table && bucket == other.bucket && element == other.element;
}

template<typename K, typename T, typename HashFunction>
bool HashMapConstIterator<K, T, HashFunction>::operator!=(const HashMapConstIterator& other) const
{
    return !(*this == other);
}

template<typename K, typename T, typename HashFunction>
void HashMapConstIterator<K, T, HashFunction>::advance()
{
    ++element;
    findNextValid();
}

template<typename K, typename T, typename HashFunction>
void HashMapConstIterator<K, T, HashFunction>::findNextValid()
{
    if (element != bucket->cend())
    {
        return;
    }

    do
    {
        ++bucket;
    } while (bucket != table->cend() && bucket->empty());

    if (bucket != table->cend())
    {
        element = bucket->cbegin();
    }
    else
    {
        element = table->back().cend();
    }
}
