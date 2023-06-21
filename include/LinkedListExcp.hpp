#pragma once

#include <exception>

namespace llexcp {
    class InvalidPosition : public std::exception {
    private:
        int _size;
        
    public:
        InvalidPosition(int size) : _size(size) { }

        int get_size() { return this->_size; }

        const char* what() const throw() {
            return "Position is not in the list";
        }
    };

    class EmptyList : public std::exception {
    private:
        int _size;
        
    public:
        EmptyList(int size) : _size(size) { }

        int get_size() { return this->_size; }

        const char* what() const throw() {
            return "List is empty";
        }
    };
}