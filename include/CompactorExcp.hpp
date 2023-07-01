#pragma once

#include <exception>

namespace compexcp {
    class BufferOverflow : public std::exception {
    private:
        int _overflow_size;
        
    public:
        BufferOverflow(int overflow_size) : _overflow_size(overflow_size) { }

        int get_overflow_size() const { return this->_overflow_size; }

        const char* what() const throw() {
            return "Buffer reached the end";
        }
    };

    class DecompressBufferOverflow : public std::exception {
    private:
        int _overflow_size;
        int _char_size;
        int _number_size;
        
    public:
        DecompressBufferOverflow(int overflow_size, int char_size, int number_size)
             : _overflow_size(overflow_size), _char_size(char_size), _number_size(number_size) { }

        int get_overflow_size() const { return this->_overflow_size; }

        int get_char_size() const { return this->_char_size; }
        
        int get_number_size() const { return this->_number_size; }

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