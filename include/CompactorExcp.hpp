#pragma once

#include <exception>

namespace compexcp {
    class BufferEnd : public std::exception {
    private:
        int _overflow_size;
        
    public:
        BufferEnd(int overflow_size) : _overflow_size(overflow_size) { }

        int get_overflow_size() const { return this->_overflow_size; }

        const char* what() const throw() {
            return "Buffer reached the end";
        }
    };

    class CouldntOpenFile : public std::exception {
    private:
        int _overflow_size;
        
    public:
        CouldntOpenFile() { }

        const char* what() const throw() {
            return "Could not open the file";
        }
    };
}