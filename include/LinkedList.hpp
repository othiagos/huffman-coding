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

/**
 * @class LinkedList
 * @brief A doubly linked list implementation.
 * @tparam T The type of elements stored in the list.
 */
template <typename T>
class LinkedList {
  private:
    NodeLinkedList<T> *_first;
    NodeLinkedList<T> *_last;
    int _size;

    /**
     * @brief Returns the node at the given position in the list.
     * @param pos The position of the node to retrieve.
     * @return Pointer to the node at the given position.
     * @throws llexcp::InvalidPosition if the position is out of range.
     */
    NodeLinkedList<T> *position(int pos) const {
        NodeLinkedList<T> *p = _first;

        if (pos > _size || pos < 0 || p == nullptr)
            throw llexcp::InvalidPosition(pos);

        for (int i = 0; i < pos; i++) {
            p = p->next;
        }
        return p;
    }

    /**
     * @brief Returns the node before the given position in the list.
     * @param pos The position of the node after the returned node.
     * @return Pointer to the node before the given position.
     * @throws llexcp::InvalidPosition if the position is out of range.
     */
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
    /**
     * @brief Default constructor for the LinkedList class.
     */
    LinkedList() {
        _first = nullptr;
        _last = nullptr;
        _size = 0;
    }

    /**
     * @brief Constructor for the LinkedList class that takes an initializer list.
     * @param args The initializer list of elements to insert into the list.
     */
    LinkedList(std::initializer_list<T> args) {
        _first = nullptr;
        _last = nullptr;
        _size = 0;

        for (T item : args)
            this->push_back(item);
    }

    /**
     * @brief Copy constructor for the LinkedList class.
     * @param o The LinkedList object to copy.
     */
    LinkedList(const LinkedList<T> &o) {
        _first = nullptr;
        _last = nullptr;
        _size = 0;

        for (int i = 0; i < o.size(); i++) {
            T item = o.get_item(i);
            this->push_back(T(item));
        }
    }

    /**
     * @brief Destructor for the LinkedList class.
     * Clears all nodes in the list.
     */
    ~LinkedList() {
        clear();
    }

    /**
     * @brief Returns an iterator pointing to the first element in the list.
     * @return Iterator pointing to the first element.
     */
    Iterator<T> begin() {
        return Iterator<T>(_first);
    }

    /**
     * @brief Returns an iterator pointing to the end element in the list.
     * @return Iterator pointing to the end element.
     */
    Iterator<T> end() {
        return Iterator<T>(_last);
    }

    /**
     * @brief Returns the current size of the list.
     * @return The size of the list.
     */
    int size() const {
        return this->_size;
    }

    /**
     * @brief Returns the item at the given position in the list.
     * @param pos The position of the item to retrieve.
     * @return The item at the given position.
     * @throws llexcp::InvalidPosition if the position is out of range.
     */
    T get_item(int pos) const {
        NodeLinkedList<T> *p;
        p = position(pos);
        return p->item;
    }

    /**
     * @brief Returns a reference to the item at the given position in the list.
     * @param pos The position of the item to retrieve.
     * @return Reference to the item at the given position.
     * @throws llexcp::InvalidPosition if the position is out of range.
     */
    T &operator[](int pos) {
        NodeLinkedList<T> *p;
        p = position(pos);
        return p->item;
    }

    /**
     * @brief Sets the item at the given position in the list.
     * @param item The new value to set.
     * @param pos The position of the item to set.
     * @throws llexcp::InvalidPosition if the position is out of range.
     */
    void set_item(T item, int pos) {
        NodeLinkedList<T> *p;

        p = position(pos);
        p->item = item;
    }

    /**
     * @brief Inserts an item at the front of the list.
     * @param item The item to insert.
     */
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

    /**
     * @brief Inserts an item at the end of the list.
     * @param item The item to insert.
     */
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

    /**
     * @brief Inserts an item at the specified position in the list.
     * @param item The item to insert.
     * @param pos The position at which to insert the item.
     * @throws llexcp::InvalidPosition if the position is out of range.
     */
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

    /**
     * @brief Removes and returns the item at the front of the list.
     * @return The item at the front of the list.
     * @throws llexcp::EmptyList if the list is empty.
     */
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

    /**
     * @brief Removes and returns the item at the end of the list.
     * @return The item at the end of the list.
     * @throws llexcp::EmptyList if the list is empty.
     */
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

    /**
     * @brief Removes and returns the item at the specified position in the list.
     * @param pos The position of the item to remove.
     * @return The item at the specified position.
     * @throws llexcp::EmptyList if the list is empty.
     * @throws llexcp::InvalidPosition if the position is out of range.
     */
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

    /**
     * @brief Searches for the first occurrence of the given item in the list.
     * @param item The item to search for.
     * @return Pointer to the first occurrence of the item, or nullptr if not found.
     */
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

    /**
     * @brief Removes all elements from the list.
     * Deletes all nodes in the list and resets the size to zero.
     */
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