#pragma once

#include <vector>
#include "../List/List.hpp"
#include "HashMapIterator.hpp"

template <typename K, typename T, typename HashFunction>
class HashMapIterator;

template <typename K, typename T, typename HashFunction = std::hash<K>>
class HashMap {
    friend class HashMapIterator<K, T, HashFunction>;

    using Pair = std::pair<K, T>;


    using Bucket =  List<Pair>;
    using HashTable = std::vector<Bucket>;

    using ElementIterator =  typename Bucket::Iterator;
    using BucketIterator =  typename HashTable::iterator;
public:
    using Iterator = HashMapIterator<K, T, HashFunction>;
    HashMap();
    HashMap(size_t capacity) : table(HashTable(capacity)), count(0), hashFunction(HashFunction()) {}
    HashMap(const HashMap<K, T, HashFunction>& other);
    HashMap(HashMap<K, T, HashFunction>&& other);
    HashMap& operator=(const HashMap<K, T, HashFunction>& other);
    ~HashMap() = default;
    void clear();

    Iterator begin();
    Iterator end();

    Iterator find(const K &key);

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
    BucketIterator getBucket(const K &key);
    bool shouldResize();
    void resize();

};

#include "HashMap.inl"
