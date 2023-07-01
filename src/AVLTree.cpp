#include "AVLTree.hpp"

TreeNode *AVLTree::create_node(std::string data) {
    TreeNode *node = new TreeNode(data);
    return node;
}

int AVLTree::max(int v1, int v2) {
    if (v1 > v2)
        return v1;
    else
        return v2;
}

int AVLTree::height(TreeNode *node) {
    if (node == nullptr)
        return 0;

    return 1 + max(height(node->get_left()), height(node->get_right()));
}

int AVLTree::get_balance(TreeNode *node) {
    return height(node->get_left()) - height(node->get_right());
}

TreeNode *AVLTree::right_rotate(TreeNode *node) {
    TreeNode *t = node->get_left();
    TreeNode *u = t->get_right();
    t->set_right(node);
    node->set_left(u);
    return t;
}

TreeNode *AVLTree::left_rotate(TreeNode *node) {
    TreeNode *t = node->get_right();
    TreeNode *u = t->get_left();
    t->set_left(node);
    node->set_right(u);
    return t;
}

TreeNode *AVLTree::min_value(TreeNode *node) {
    if (node->get_left() == nullptr)
        return node;
    return min_value(node->get_left());
}

TreeNode *AVLTree::insert_node(TreeNode *node, std::string item) {
    if (node == nullptr) {
        return create_node(item);
    }
    if (node->get_chars() == item) {
        node->set_count(node->get_count() + 1);
    } else if (node->get_chars().compare(item) > 0) {
        node->set_left(insert_node(node->get_left(), item));
    } else {
        node->set_right(insert_node(node->get_right(), item));
    }

    int b = get_balance(node);
    if (b > 1) {
        if (get_balance(node->get_left()) < 0) {
            node->set_left(left_rotate(node->get_left())); // LR case
        }
        return right_rotate(node); // LL Case
    } else if (b < -1) {
        if (get_balance(node->get_right()) > 0) {
            node->set_right(right_rotate(node->get_right())); // RL Case
        }
        return left_rotate(node); // RR Case
    }
    return node;
}

TreeNode *AVLTree::delete_node(TreeNode *node, std::string element) {
    if (node == nullptr) {
        return node;
    }
    if (node->get_chars().compare(element) < 0) {
        node->set_left(delete_node(node->get_left(), element));
    } else if (node->get_chars().compare(element) > 0) {
        node->set_right(delete_node(node->get_right(), element));

    } else {
        if (node->get_count() > 1) {
            node->set_count(node->get_count() - 1);
        } else {
            if (!node->get_left() || !node->get_right()) {
                TreeNode *temp = !node->get_right() ? node->get_left() : node->get_right();
                delete node;
                return temp;
            }

            TreeNode *temp = min_value(node->get_right());
            node->set_chars(temp->get_chars());
            node->set_right(delete_node(node->get_right(), temp->get_chars()));
        }
    }
    return node;
}

void AVLTree::delete_all_nodes(TreeNode *node) {
    if (node) {
        delete_all_nodes(node->get_left());
        delete_all_nodes(node->get_right());
        delete node;
    }
}

AVLTree::~AVLTree() {
    if (this->_root != nullptr)
        delete _root;
}

void AVLTree::insert(std::string item) {
    _root = insert_node(_root, item);
}

void AVLTree::remove(std::string item) {
    _root = delete_node(_root, item);
}

TreeNode *AVLTree::get_root() {
    return this->_root;
}