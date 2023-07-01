#pragma once

#include <cmath>
#include <string>

#define PRIME_CONST 137

struct table {
    std::string chars;
    std::string encoding;
};

class TreeNode {
private:
    std::string _chars;
    unsigned int _count;
    TreeNode *_right = nullptr;
    TreeNode *_left = nullptr;

public:
    TreeNode();
    ~TreeNode();
    TreeNode(const TreeNode &o);
    TreeNode(std::string _chars);
    TreeNode(unsigned int count, TreeNode right, TreeNode left);

    int hash();
    TreeNode operator++(int);
    bool operator==(const TreeNode& o);
    TreeNode& operator=(const TreeNode& o);
    std::string get_chars() const;
    void set_chars(std::string value);
    unsigned int get_count() const;
    void set_count(unsigned int value);
    TreeNode *get_right();
    TreeNode *get_left();
};
