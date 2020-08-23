#pragma once
#include "HashMap.hpp"

template <typename K, typename T, typename HashFunction>
class HashMap;

template <typename K, typename T, typename HashFunction>
class HashMapIterator {
    friend class HashMap<K, T, HashFunction>;

    using Bucket = typename HashMap<K, T, HashFunction>::Bucket;
    using HashTable = typename HashMap<K, T, HashFunction>::HashTable;

    using ElementIterator = typename HashMap<K, T, HashFunction>::ElementIterator;
    using BucketIterator = typename HashMap<K, T, HashFunction>::BucketIterator;

public:
    HashMapIterator(HashTable &tableRef);
    HashMapIterator(HashTable &table, BucketIterator bucket, ElementIterator element);

    // Const key so the user can't edit the key
    using ConstPair = std::pair<const K, T>;

    ConstPair & operator*();

    ConstPair * operator->();

    HashMapIterator operator++(int);
    HashMapIterator& operator++();

    bool operator==(const HashMapIterator &other) const;

    bool operator!=(const HashMapIterator &other) const;

private:
    HashTable *table;
    BucketIterator bucket;
    ElementIterator element;

    void advance();
    void findNextValid();
};

#include "HashMapIterator.inl"
