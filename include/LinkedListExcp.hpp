#pragma once

#include <exception>

namespace llexcp {
    /**
     * @class InvalidPosition
     * @brief Exception class for an invalid position in a list.
     */
    class InvalidPosition : public std::exception {
    private:
        int _size;
        
    public:

        /**
         * @brief Constructor for the InvalidPosition class.
         * @param size The size of the list.
         */
        InvalidPosition(int size) : _size(size) { }

        /**
         * @brief Returns the size of the list.
         * @return The size of the list.
         */
        int get_size() { return this->_size; }

        /**
         * @brief Returns a string describing the exception.
         * @return string describing the exception.
         */
        const char* what() const throw() {
            return "Position is not in the list";
        }
    };

    /**
     * @class EmptyList
     * @brief Exception class for an empty list.
     */
    class EmptyList : public std::exception {
    private:
        int _size;
        
    public:
        /**
         * @brief Constructor for the EmptyList class.
         * @param size The size of the list.
         */
        EmptyList(int size) : _size(size) { }

        /**
         * @brief Returns the size of the list.
         * @return The size of the list.
         */
        int get_size() { return this->_size; }

        /**
         * @brief Returns a string describing the exception.
         * @return string describing the exception.
         */
        const char* what() const throw() {
            return "List is empty";
        }
    };
}