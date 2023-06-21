#pragma once

#include <cmath>
#include <string>

#define PRIME_CONST 137

class TreeNodeChar {
private:
    std::string _chars;
    unsigned int _count;

public:
    TreeNodeChar();
    TreeNodeChar(std::string _chars);
    TreeNodeChar(unsigned int count);

    int hash();
    TreeNodeChar operator++(int);
    bool operator==(const TreeNodeChar& o);
    std::string get_chars();
    unsigned int get_count();
};
