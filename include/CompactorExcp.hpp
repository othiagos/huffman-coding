#pragma once

#include <exception>

namespace compexcp {
    class BufferOverflow : public std::exception {
    private:
        uint8_t _overflow_size;
        
    public:
        BufferOverflow(uint8_t overflow_size) : _overflow_size(overflow_size) { }

        uint8_t get_overflow_size() const { return this->_overflow_size; }

        const char* what() const throw() {
            return "Buffer reached the end";
        }
    };

    class DecompressBufferOverflow : public std::exception {
    private:
        uint8_t _overflow_size;
        uint8_t _char_size;
        uint8_t _number_size;
        
    public:
        DecompressBufferOverflow(uint8_t overflow_size, uint8_t char_size, uint8_t number_size)
             : _overflow_size(overflow_size), _char_size(char_size), _number_size(number_size) { }

        uint8_t get_overflow_size() const { return this->_overflow_size; }

        uint8_t get_char_size() const { return this->_char_size; }
        
        uint8_t get_number_size() const { return this->_number_size; }

        const char* what() const throw() {
            return "Buffer reached the end";
        }
    };

    class CouldNotOpenFile : public std::exception {
        
    public:
        CouldNotOpenFile() { }

        const char* what() const throw() {
            return "Could not open the file";
        }
    };

    class WithoutArguments : public std::exception {
    private:
        uint8_t _arg_count;
        
    public:
        WithoutArguments(uint8_t arg_count) : _arg_count(arg_count) { }

        uint8_t get_arg_count() const { return this->_arg_count; }

        const char* what() const throw() {
            return "Arguments passed are invalid";
        }
    };
}