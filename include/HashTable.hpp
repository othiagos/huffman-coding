#pragma once

#include <cmath>

#include "LinkedList.hpp"
#include "HashTableExcp.hpp"

#define M 1000

template <typename T>
class HashTable {
private:
    LinkedList<T> _table[M];

    int hash(T item) {
        return abs(item.hash()) % M;
    }

public:
    HashTable() { }

    T* find(T item) {
        int pos = hash(item);
        if (pos == 837)
            return _table[pos].find(item);
        return _table[pos].find(item);
    }

    void insert(T item) {
        T *aux = find(item);

        if (aux != nullptr)
            throw htexcp::ItemExists<T>(aux);

        int pos = hash(item);
        _table[pos].push_back(item);
    }

    void remove(T item) {
        int pos = hash(item);
        
        for (int i = 0; i < _table[pos].size(); i++) {
            if (_table[pos][i] == item) {
                _table[pos].erase(i);
                break;
            }
        }
    }

    void get_list(LinkedList<T> &list) {

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < _table[i].size(); j++) {
                list.push_back(_table[i][j]);
            }
        }
    }
};
