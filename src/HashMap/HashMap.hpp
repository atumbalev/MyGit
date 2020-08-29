#pragma once

#include <vector>
#include "../List/List.hpp"
#include "HashMapIterator.hpp"
#include "HashMapConstIterator.hpp"

template <typename K, typename T, typename HashFunction>
class HashMapIterator;

template <typename K, typename T, typename HashFunction>
class HashMapConstIterator;

///
/// \tparam K key type
/// \tparam T element type
/// \tparam HashFunction hash function, that must work on K
/// HashMap implementation that uses @vector as a bucket container and @List as an element container
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

    ConstIterator cbegin() const;
    ConstIterator cend() const;

    Iterator find(const K& key);
    ConstIterator find(const K& key) const;

    Iterator insert(const K &key, const T &value);

    /// operator[] suggests element access with constant complexity, so I implemented it
    T& operator[](const K &key);

    Iterator erase(Iterator it);
    Iterator erase(const K &key);

    int size() const;
    bool empty() const;

private:
    HashTable table;
    int count;
    HashFunction hashFunction;

    int index(const K &key) const;
    BucketIterator getBucket(const K& key);
    BucketConstIterator getBucket(const K& key) const;

    void copy(const HashMap<K, T, HashFunction>& other);
    bool shouldResize();
    void resize();

};

#include "HashMap.inl"
