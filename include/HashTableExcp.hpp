#pragma once

#include <exception>

namespace htexcp {
    template <typename T>
    class ItemExists : public std::exception {
    private:
        T *_item;
        
    public:
        ItemExists(T *item) : _item(item) { }

        T *get_item() const { return this->_item; }

        const char* what() const throw() {
            return "Item exists in the list";
        }
    };
}