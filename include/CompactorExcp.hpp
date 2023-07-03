#pragma once

#include <exception>

namespace compexcp {
    /**
     * @class BufferOverflow
     * @brief Exception class for buffer overflow.
     */
    class BufferOverflow : public std::exception {
    private:
        uint8_t _overflow_size;
        
    public:
        /**
         * @brief Constructor for the BufferOverflow class.
         * @param overflow_size The size of the buffer overflow.
         */
        BufferOverflow(uint8_t overflow_size) : _overflow_size(overflow_size) { }

        /**
         * @brief Returns the size of the buffer overflow.
         * @return The size of the buffer overflow.
         */
        uint8_t get_overflow_size() const { return this->_overflow_size; }

        /**
         * @brief Returns a string describing the exception.
         * @return string describing the exception.
         */
        const char* what() const throw() {
            return "Buffer reached the end";
        }
    };

    /**
     * @class DecompressBufferOverflow
     * @brief Exception class for buffer overflow during decompression.
     */
    class DecompressBufferOverflow : public std::exception {
    private:
        uint8_t _overflow_size;
        uint8_t _char_size;
        uint8_t _number_size;
        
    public:

        /**
         * @brief Constructor for the DecompressBufferOverflow class.
         * @param overflow_size The size of the buffer overflow.
         * @param char_size The size of the character.
         * @param number_size The size of the number.
         */
        DecompressBufferOverflow(uint8_t overflow_size, uint8_t char_size, uint8_t number_size)
             : _overflow_size(overflow_size), _char_size(char_size), _number_size(number_size) { }

        /**
         * @brief Returns the size of the buffer overflow.
         * @return The size of the buffer overflow.
         */
        uint8_t get_overflow_size() const { return this->_overflow_size; }

        /**
         * @brief Returns the size of the character.
         * @return The size of the character.
         */
        uint8_t get_char_size() const { return this->_char_size; }
        
        /**
         * @brief Returns the size of the number.
         * @return The size of the number.
         */
        uint8_t get_number_size() const { return this->_number_size; }

        /**
         * @brief Returns a string describing the exception.
         * @return string describing the exception.
         */
        const char* what() const throw() {
            return "Buffer reached the end";
        }
    };

    /**
     * @class CouldNotOpenFile
     * @brief Exception class for a file that could not be opened.
     */
    class CouldNotOpenFile : public std::exception { 
    public:
        /**
         * @brief Constructor for the CouldNotOpenFile class.
         */
        CouldNotOpenFile() { }

        /**
         * @brief Returns a string describing the exception.
         * @return string describing the exception.
         */
        const char* what() const throw() {
            return "Could not open the file";
        }
    };

    /**
     * @class FileNotUTF8
     * @brief Exception class for a text file that does not have UTF-8 encoding.
     */
    class FileNotUTF8 : public std::exception { 
    public:
        /**
         * @brief Constructor for the FileNotUTF8 class.
         */
        FileNotUTF8() { }

        /**
         * @brief Returns a string describing the exception.
         * @return string describing the exception.
         */
        const char* what() const throw() {
            return "Text file does not have UTF-8 encoding";
        }
    };

    /**
     * @class WithoutArguments
     * @brief Exception class for an incorrect number of arguments.
     */
    class WithoutArguments : public std::exception {
    private:
        uint8_t _arg_count;
        
    public:
        /**
         * @brief Constructor for the WithoutArguments class.
         * @param arg_count The number of arguments.
         */
        WithoutArguments(uint8_t arg_count) : _arg_count(arg_count) { }

        /**
         * @brief Returns the number of arguments.
         * @return The number of arguments.
         */
        uint8_t get_arg_count() const { return this->_arg_count; }

        /**
         * @brief Returns a string describing the exception.
         * @return string describing the exception.
         */
        const char* what() const throw() {
            return "Arguments passed are invalid";
        }
    };
}