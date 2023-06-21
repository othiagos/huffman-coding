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

    T* find(int hash, T item) {
        return _table[hash].find(item);
    }

    void insert(T item) {
        int pos = hash(item);
        T *aux = find(pos, item);

        if (aux != nullptr)
            throw htexcp::ItemExists<T>(aux);

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
