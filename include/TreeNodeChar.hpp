#pragma once

#include <cmath>
#include <string>

#define PRIME_CONST 137

class TreeNodeChar {
private:
    std::string _chars;
    unsigned int _count;
    TreeNodeChar *_right = nullptr;
    TreeNodeChar *_left = nullptr;

public:
    TreeNodeChar();
    ~TreeNodeChar();
    TreeNodeChar(const TreeNodeChar &o);
    TreeNodeChar(std::string _chars);
    TreeNodeChar(unsigned int count, TreeNodeChar right, TreeNodeChar left);

    int hash();
    TreeNodeChar operator++(int);
    bool operator==(const TreeNodeChar& o);
    TreeNodeChar& operator=(const TreeNodeChar& o);
    std::string get_chars() const;
    void set_chars(std::string value);
    unsigned int get_count() const;
    void set_count(unsigned int value);
    TreeNodeChar *get_right();
    TreeNodeChar *get_left();
};
