#include <iostream>
#include <vector>
#include <forward_list>
#include <string>
#include <stdexcept>
using namespace std;

const int maxColision = 3;

template <typename TK, typename TV>
class HashTable;

template <typename TK, typename TV>
class HashIterator {
private:
    using Entry = typename HashTable<TK, TV>::Entry;
    vector<Entry*>* insertionOrder;
    size_t index;

public:
    HashIterator(vector<Entry*>* _vec, size_t _i) : insertionOrder(_vec), index(_i) {}

    HashIterator<TK, TV>& operator++() {
        index++;
        return *this;
    }

    bool operator!=(const HashIterator<TK, TV>& other) const {
        return index != other.index;
    }

    pair<TK, TV> operator*() {
        Entry* e = (*insertionOrder)[index];
        return {e->k, e->v};
    }

    friend class HashTable<TK, TV>;
};

template <typename TK, typename TV>
class HashTable {
public:
    struct Entry {
        TK k;
        TV v;
        Entry(TK key, TV val) : k(key), v(val) {}
    };

private:
    using Bucket = forward_list<Entry*>;
    Bucket* table;
    int capacity;
    int size;
    vector<Entry*> insertionOrder;

    int getIndex(TK key) const {
        return hash<TK>{}(key) % capacity;
    }

    void clearBuckets() {
        for (int i = 0; i < capacity; ++i) {
            for (Entry* e : table[i]) {
                delete e;
            }
        }
        delete[] table;
    }

    void rehashing() {
        int oldCap = capacity;
        capacity *= 2;
        Bucket* oldTable = table;
        vector<Entry*> oldOrder = insertionOrder;

        table = new Bucket[capacity];
        insertionOrder.clear();
        size = 0;

        for (Entry* e : oldOrder) {
            insert(e->k, e->v);
            delete e;
        }

        delete[] oldTable;
    }

public:
    using iterator = HashIterator<TK, TV>;

    HashTable(int _cap = 10) : capacity(_cap), size(0) {
        table = new Bucket[capacity];
    }

    ~HashTable() {
        clearBuckets();
    }

    void insert(TK key, TV value) {
        if (float(size) / capacity >= 0.7) rehashing();

        int idx = getIndex(key);
        for (Entry* e : table[idx]) {
            if (e->k == key) {
                e->v = value;
                return;
            }
        }

        Entry* newEntry = new Entry(key, value);
        table[idx].push_front(newEntry);
        insertionOrder.push_back(newEntry);
        size++;
    }

    void insert(pair<TK, TV> item) {
        insert(item.first, item.second);
    }

    TV& at(TK key) {
        int idx = getIndex(key);
        for (Entry* e : table[idx]) {
            if (e->k == key)
                return e->v;
        }
        throw out_of_range("Key not found");
    }

    TV& operator[](TK key) {
        int idx = getIndex(key);
        for (Entry* e : table[idx]) {
            if (e->k == key)
                return e->v;
        }
        insert(key, TV());
        return at(key);
    }

    bool find(TK key) {
        int idx = getIndex(key);
        for (Entry* e : table[idx]) {
            if (e->k == key)
                return true;
        }
        return false;
    }

    bool remove(TK key) {
        int idx = getIndex(key);
        auto& bucket = table[idx];
        auto it = bucket.before_begin();

        for (auto cur = bucket.begin(); cur != bucket.end(); ++cur) {
            if ((*cur)->k == key) {
                delete *cur;
                bucket.erase_after(it);

                for (auto i = insertionOrder.begin(); i != insertionOrder.end(); ++i) {
                    if ((*i)->k == key) {
                        insertionOrder.erase(i);
                        break;
                    }
                }

                size--;
                return true;
            }
            ++it;
        }
        return false;
    }

    int getSize() {
        return size;
    }

    vector<TK> getAllKeys() {
        vector<TK> keys;
        for (auto e : insertionOrder)
            keys.push_back(e->k);
        return keys;
    }

    vector<pair<TK, TV>> getAllElements() {
        vector<pair<TK, TV>> elems;
        for (auto e : insertionOrder)
            elems.emplace_back(e->k, e->v);
        return elems;
    }

    iterator begin() {
        return iterator(&insertionOrder, 0);
    }

    iterator end() {
        return iterator(&insertionOrder, insertionOrder.size());
    }
};
