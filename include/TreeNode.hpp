#pragma once

#include <cmath>
#include <string>

struct table {
    std::string chars;
    std::string encoding;
};

class TreeNode {
private:
    std::string _chars;
    unsigned int _count = 0;
    TreeNode *_right = nullptr;
    TreeNode *_left = nullptr;

public:
    TreeNode();
    ~TreeNode();
    
    TreeNode(const TreeNode &o);
    TreeNode(std::string _chars);
    TreeNode(unsigned int count);
    
    static void DeleteNode(TreeNode *node);

    bool operator==(const TreeNode& o);

    TreeNode& operator=(const TreeNode& o);

    std::string get_chars() const;

    void set_chars(std::string value);

    unsigned int get_count() const;

    void set_count(unsigned int value);

    TreeNode *get_right();

    void set_right(TreeNode *node);

    TreeNode *get_left();

    void set_left(TreeNode *node);
};
