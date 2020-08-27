#pragma once

#include <vector>
#include "../List/List.hpp"
#include "HashMapIterator.hpp"
#include "HashMapConstIterator.hpp"

template <typename K, typename T, typename HashFunction>
class HashMapIterator;

template <typename K, typename T, typename HashFunction = std::hash<K>>
class HashMap {
    friend class HashMapIterator<K, T, HashFunction>;
    friend class HashMapConstIterator<K, T, HashFunction>;

    using Pair = std::pair<K, T>;


    using Bucket =  List<Pair>;
    using HashTable = std::vector<Bucket>;

    using ElementIterator =  typename Bucket::Iterator;
    using ElementConstIterator =  typename Bucket::ConstIterator;

    using BucketIterator =  typename HashTable::iterator;
    using BucketConstIterator = typename HashTable::const_iterator;
public:
    using Iterator = HashMapIterator<K, T, HashFunction>;
    using ConstIterator = HashMapConstIterator<K, T, HashFunction>;
    HashMap();
    HashMap(size_t capacity) : table(HashTable(capacity)), count(0), hashFunction(HashFunction()) {}
    HashMap(const HashMap<K, T, HashFunction>& other);
    HashMap(HashMap<K, T, HashFunction>&& other);
    HashMap& operator=(const HashMap<K, T, HashFunction>& other);
    ~HashMap() = default;
    void clear();

    Iterator begin();
    Iterator end();

    ConstIterator cbegin() const
    {
        return ConstIterator (table);
    }

    ConstIterator cend() const
    {
        return ConstIterator(table, table.cend(), table.back().cend());
    }

    Iterator find(const K& key);
    ConstIterator find(const K& key) const
    {
        BucketConstIterator bucket = getBucket(key);
        for (ElementConstIterator elIter = bucket->cbegin(); elIter != bucket->cend(); ++elIter)
        {
            if (elIter->first == key)
            {
                return ConstIterator(table, bucket, elIter);
            }
        }
        return cend();
    }

    Iterator insert(const K &key, const T &value);

    T& operator[](const K &key);

    Iterator erase(Iterator it);
    Iterator erase(const K &key);

    int size() const;

private:
    HashTable table;
    int count;
    HashFunction hashFunction;

    int index(const K &key) const;
    BucketIterator getBucket(const K& key);
    BucketConstIterator getBucket(const K& key) const
    {
        return table.cbegin() + index(key);
    }
    bool shouldResize();
    void resize();

};

#include "HashMap.inl"
