#include "TreeNodeChar.hpp"

TreeNodeChar::TreeNodeChar() {
    this->_chars = "";
    this->_count = 0;
}

TreeNodeChar::TreeNodeChar(std::string chars) {
    this->_chars = chars;
    this->_count = 1;
}

TreeNodeChar::TreeNodeChar(unsigned int count) {
    this->_chars = "";
    this->_count = count;
}

int TreeNodeChar::hash() {
    int hashValue = 0;

    for (int i = 0; i < _chars.length() - 1; i++)
        hashValue += (unsigned char) _chars[i] * pow(PRIME_CONST, _chars.length() - 1 - i);

    return hashValue;
}

TreeNodeChar TreeNodeChar::operator++(int) {
    this->_count++;
    return *this;
}

bool TreeNodeChar::operator==(const TreeNodeChar& o) {
    return this->_chars == o._chars;
}

std::string TreeNodeChar::get_chars() {
    return this->_chars;
}

unsigned int TreeNodeChar::get_count() {
    return this->_count;
}
