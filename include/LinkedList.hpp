#pragma once

#include "LinkedListExcp.hpp"

#include <initializer_list>

template <typename T>
struct NodeLinkedList {
    NodeLinkedList *next = nullptr;
    NodeLinkedList *prev = nullptr;
    T item;
};

template <typename T>
class Iterator {
private:
    NodeLinkedList<T> *_node;

public:
    Iterator(NodeLinkedList<T> *node) {
        this->_node = node;
    }

    T operator*() const {
        return _node->item;
    }

    void operator++() {
        _node = _node->next;
    }

    bool operator!=(const Iterator& o) const {
        return _node != o._node->next;
    }
};

template <typename T>
class LinkedList {
  private:
    NodeLinkedList<T> *_first;
    NodeLinkedList<T> *_last;
    int _size;

    NodeLinkedList<T> *position(int pos) const {
        NodeLinkedList<T> *p = _first;

        if (pos > _size || pos < 0 || p == nullptr)
            throw llexcp::InvalidPosition(pos);

        for (int i = 0; i < pos; i++) {
            p = p->next;
        }
        return p;
    }

    NodeLinkedList<T> *position_before(int pos) {
        NodeLinkedList<T> *p = _first;
        int i;

        if (pos > _size || pos <= 0 || p == nullptr)
            throw llexcp::InvalidPosition(pos);

        for (i = 1; i < pos; i++) {
            p = p->next;
        }
        return p;
    }

  public:
    LinkedList() {
        _first = nullptr;
        _last = nullptr;
        _size = 0;
    }

    LinkedList(std::initializer_list<T> args) {
        _first = nullptr;
        _last = nullptr;
        _size = 0;

        for (T item : args)
            this->push_back(item);
    }

    LinkedList(const LinkedList<T> &o) {
        _first = nullptr;
        _last = nullptr;
        _size = 0;

        for (int i = 0; i < o.size(); i++) {
            T item = o.get_item(i);
            this->push_back(T(item));
        }
    }

    ~LinkedList() {
        clear();
    }

    Iterator<T> begin() {
        return Iterator<T>(_first);
    }

    Iterator<T> end() {
        return Iterator<T>(_last);
    }

    int size() const {
        return this->_size;
    }

    T get_item(int pos) const {
        NodeLinkedList<T> *p;
        p = position(pos);
        return p->item;
    }

    T &operator[](int pos) {
        NodeLinkedList<T> *p;
        p = position(pos);
        return p->item;
    }

    void set_item(T item, int pos) {
        NodeLinkedList<T> *p;

        p = position(pos);
        p->item = item;
    }

    void push_front(T item) {
        NodeLinkedList<T> *node = new NodeLinkedList<T>();

        node->item = item;
        node->next = _first;
        if (_first != nullptr)
            _first->prev = node;
        _first = node;
        _size++;

        if (node->next == nullptr)
            _last = node;
    }

    void push_back(T item) {

        if (_size != 0) {
            NodeLinkedList<T> *node = new NodeLinkedList<T>();
            node->item = item;
            node->prev = _last;
            _last->next = node;
            _last = node;
            _size++;
        } else
            push_front(item);
    }

    void insert(T item, int pos) {
        if (pos != 0) {
            NodeLinkedList<T> *p, *node = new NodeLinkedList<T>();
            p = position_before(pos);

            node->item = item;
            node->next = p->next;
            if (p->next != nullptr)
                p->next->prev = node;
            p->next = node;
            node->prev = p;
            _size++;

            if (node->next == nullptr)
                _last = node;
        } else
            push_front(item);
    }

    T pop_front() {
        T aux;
        NodeLinkedList<T> *p;

        if (_size == 0)
            throw llexcp::EmptyList(_size);

        p = _first;
        _first = p->next;
        if (_first != nullptr)
            _first->prev = nullptr;
        _size--;

        if (_first != nullptr && _first->next == nullptr)
            _last = _first;

        if (_first == nullptr)
            _last = _first;

        aux = p->item;
        delete p;

        return aux;
    }

    T pop_back() {
        T aux;
        NodeLinkedList<T> *p;

        if (_size == 0)
            throw llexcp::EmptyList(_size);

        p = position(_size - 1);
        _last = p->prev;
        if (_last != nullptr)
            _last->next = nullptr;
        else
            _first = nullptr;
        _size--;

        aux = p->item;

        delete p;
        return aux;
    }

    T erase(int pos) {
        T aux;
        NodeLinkedList<T> *p;

        if (_size == 0)
            throw llexcp::EmptyList(_size);

        if (pos == 0)
            return pop_front();

        if (pos == _size - 1)
            return pop_back();

        p = position(pos);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        _size--;

        aux = p->item;
        delete p;
        return aux;
    }

    T* find(T item) {
        NodeLinkedList<T> *p = _first, *aux = nullptr;

        while (p != nullptr) {
            if (p->item == item)  {
                aux = p;
                break;
            }
            p = p->next;
        }

        if (aux != nullptr)
            return &aux->item;
        return nullptr;
    }

    void clear() {
        NodeLinkedList<T> *p;

        p = _first;
        while (p != nullptr) {
            _first = p->next;
            delete p;
            p = _first;
        }
        _last = nullptr;
        _size = 0;
    }
};