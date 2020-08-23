#include "HashMapIterator.hpp"

template<typename K, typename T, typename HashFunction>
HashMapIterator<K, T, HashFunction>::HashMapIterator(HashMapIterator::HashTable& tableRef): table(&tableRef) {
    bucket = table->begin();
    element = bucket->begin();
    findNextValid();
}

template<typename K, typename T, typename HashFunction>
HashMapIterator<K, T, HashFunction>::HashMapIterator(HashMapIterator::HashTable& table, HashMapIterator::BucketIterator bucket,
                                             HashMapIterator::ElementIterator element) : table(&table), bucket(bucket), element(element) {}

template<typename K, typename T, typename HashFunction>
typename HashMapIterator<K, T, HashFunction>::ConstPair& HashMapIterator<K, T, HashFunction>::operator*()
{
    return reinterpret_cast<ConstPair &>(*element);
}

template<typename K, typename T, typename HashFunction>
typename HashMapIterator<K, T, HashFunction>::ConstPair* HashMapIterator<K, T, HashFunction>::operator->()
{
    return &(operator*());
}

template<typename K, typename T, typename HashFunction>
HashMapIterator<K, T, HashFunction> HashMapIterator<K, T, HashFunction>::operator++(int)
{
    HashMapIterator copy(*this);
    advance();
    return copy;
}

template<typename K, typename T, typename HashFunction>
HashMapIterator<K, T, HashFunction>& HashMapIterator<K, T, HashFunction>::operator++()
{
    advance();
    return *this;
}

template<typename K, typename T, typename HashFunction>
bool HashMapIterator<K, T, HashFunction>::operator==(const HashMapIterator& other) const
{
    return table == other.table && bucket == other.bucket && element == other.element;
}

template<typename K, typename T, typename HashFunction>
bool HashMapIterator<K, T, HashFunction>::operator!=(const HashMapIterator& other) const
{
    return !(*this == other);
}

template<typename K, typename T, typename HashFunction>
void HashMapIterator<K, T, HashFunction>::advance()
{
    ++element;
    findNextValid();
}

template<typename K, typename T, typename HashFunction>
void HashMapIterator<K, T, HashFunction>::findNextValid()
{
    if (element != bucket->end())
    {
        return;
    }

    do
    {
        ++bucket;
    } while (bucket != table->end() && bucket->empty());

    if (bucket != table->end())
    {
        element = bucket->begin();
    }
    else
    {
        element = table->back().end();
    }
}
