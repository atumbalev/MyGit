#pragma once
#include "HashMap.hpp"

template <typename K, typename T, typename HashFunction>
class HashMap;

template <typename K, typename T, typename HashFunction>
class HashMapConstIterator {
    friend class HashMap<K, T, HashFunction>;

    using iterator_tag = std::forward_iterator_tag;

    using Bucket = typename HashMap<K, T, HashFunction>::Bucket;
    using HashTable = typename HashMap<K, T, HashFunction>::HashTable;

    using ElementIterator = typename HashMap<K, T, HashFunction>::ElementIterator;
    using ElementConstIterator = typename HashMap<K, T, HashFunction>::ElementConstIterator;

    using BucketIterator = typename HashMap<K, T, HashFunction>::BucketIterator;
    using BucketConstIterator = typename HashMap<K, T, HashFunction>::BucketConstIterator;

public:
    HashMapConstIterator(const HashTable &tableRef);
    HashMapConstIterator(const HashTable &table, BucketConstIterator bucket, ElementConstIterator element);

    const std::pair<K, T> & operator*() const;
    const std::pair<K, T> * operator->() const;

    HashMapConstIterator operator++(int);
    HashMapConstIterator& operator++();

    bool operator==(const HashMapConstIterator &other) const;

    bool operator!=(const HashMapConstIterator &other) const;

private:
    const HashTable *table;
    BucketConstIterator bucket;
    ElementConstIterator element;

    void advance();
    void findNextValid();
};

#include "HashMapConstIterator.inl"
