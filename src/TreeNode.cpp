#include "TreeNode.hpp"

TreeNode::TreeNode() {
    this->_chars = "";
    this->_count = 0;
    this->_right = nullptr;
    this->_left = nullptr;
}

TreeNode::~TreeNode() {
    if (this->_right != nullptr)
        delete _right;
    if (this->_left != nullptr)
        delete _left;
}

TreeNode::TreeNode(std::string chars) {
    this->_chars = chars;
    this->_count = 1;
    this->_right = nullptr;
    this->_left = nullptr;
}

TreeNode::TreeNode(const TreeNode &o) {
    this->_chars = o.get_chars();
    this->_count = o.get_count();

    this->_right = (o._right != nullptr) ? new TreeNode(*o._right) : nullptr;
    this->_left = (o._left != nullptr) ? new TreeNode(*o._left) : nullptr;
}

TreeNode::TreeNode(unsigned int count, TreeNode right, TreeNode left) {
    this->_chars = "";
    this->_count = count;
    this->_right = new TreeNode(right);
    this->_left = new TreeNode(left);
}

bool TreeNode::operator==(const TreeNode& o) {
    return this->_chars == o._chars;
}

TreeNode& TreeNode::operator=(const TreeNode& o) {
    this->_chars = o.get_chars();
    this->_count = o.get_count();

    if (this->_right != nullptr) delete _right;
    if (this->_left != nullptr) delete _left;

    this->_right = (o._right != nullptr) ? new TreeNode(*o._right) : nullptr;
    this->_left = (o._left != nullptr) ? new TreeNode(*o._left) : nullptr;
    return *this;
}

std::string TreeNode::get_chars() const {
    return this->_chars;
}

void TreeNode::set_chars(std::string value) {
    this->_chars = value;
}

unsigned int TreeNode::get_count() const {
    return this->_count;
}

void TreeNode::set_count(unsigned int value) {
    this->_count = value;
}

TreeNode *TreeNode::get_right() {
    return this->_right;
}

void TreeNode::set_right(TreeNode *node) {
    this->_right = node;
}

TreeNode *TreeNode::get_left() {
    return this->_left;
}

void TreeNode::set_left(TreeNode *node) {
    this->_left = node;
}
