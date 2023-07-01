#include "TreeNode.hpp"

class AVLTree {
private:
    TreeNode *_root = nullptr;

    TreeNode *create_node(std::string data);

    int max(int v1, int v2);

    int height(TreeNode *node);

    int get_balance(TreeNode *node);

    TreeNode *right_rotate(TreeNode *node);

    TreeNode *left_rotate(TreeNode *root);

    TreeNode *min_value(TreeNode *root);

    TreeNode *insert_node(TreeNode *node, std::string item);

    TreeNode *delete_node(TreeNode *node, std::string element);

    void delete_all_nodes(TreeNode *node);

public:
    ~AVLTree();

    void insert(std::string item);

    void remove(std::string item);

    TreeNode *get_root();
};
