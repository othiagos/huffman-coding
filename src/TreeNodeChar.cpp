#include "TreeNodeChar.hpp"

TreeNodeChar::TreeNodeChar() {
    this->_chars = "";
    this->_count = 0;
    this->_right = nullptr;
    this->_left = nullptr;
}

TreeNodeChar::~TreeNodeChar() {
    if (this->_right != nullptr)
        delete _right;
    if (this->_left != nullptr)
        delete _left;
}

TreeNodeChar::TreeNodeChar(std::string chars) {
    this->_chars = chars;
    this->_count = 1;
    this->_right = nullptr;
    this->_left = nullptr;
}

TreeNodeChar::TreeNodeChar(const TreeNodeChar &o) {
    this->_chars = o.get_chars();
    this->_count = o.get_count();

    this->_right = (o._right != nullptr) ? new TreeNodeChar(*o._right) : nullptr;
    this->_left = (o._left != nullptr) ? new TreeNodeChar(*o._left) : nullptr;
}

TreeNodeChar::TreeNodeChar(unsigned int count, TreeNodeChar right, TreeNodeChar left) {
    this->_chars = "";
    this->_count = count;
    this->_right = new TreeNodeChar(right);
    this->_left = new TreeNodeChar(left);
}

int TreeNodeChar::hash() {
    int hashValue = 0;

    for (int i = 0; i < (int) _chars.length() - 1; i++)
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

TreeNodeChar& TreeNodeChar::operator=(const TreeNodeChar& o) {
    this->_chars = o.get_chars();
    this->_count = o.get_count();

    if (this->_right != nullptr) delete _right;
    if (this->_left != nullptr) delete _left;

    this->_right = (o._right != nullptr) ? new TreeNodeChar(*o._right) : nullptr;
    this->_left = (o._left != nullptr) ? new TreeNodeChar(*o._left) : nullptr;
    return *this;
}

std::string TreeNodeChar::get_chars() const {
    return this->_chars;
}

unsigned int TreeNodeChar::get_count() const {
    return this->_count;
}

TreeNodeChar *TreeNodeChar::get_right() {
    return this->_right;
}

TreeNodeChar *TreeNodeChar::get_left() {
    return this->_left;
}
