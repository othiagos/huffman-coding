#include "TreeNode.hpp"

/**
 * @class AVLTree
 * @brief Represents an AVL tree data structure.
 */
class AVLTree {
private:

    // Pointer to the root node of the AVL tree.
    TreeNode *_root = nullptr;

    /**
     * @brief Creates a new tree node with the given data.
     * @param data The data to be stored in the new node.
     * @return Pointer to the newly created node.
     */
    TreeNode *create_node(std::string data);

    /**
     * @brief Returns the maximum of two values.
     * @param v1 First value.
     * @param v2 Second value.
     * @return The maximum value between v1 and v2.
     */
    int max(int v1, int v2);

    /**
     * @brief Returns the height of the given node.
     * @param node Pointer to the node.
     * @return The height of the node.
     */
    int height(TreeNode *node);

    /**
     * @brief Returns the balance factor of the given node.
     * @param node Pointer to the node.
     * @return The balance factor of the node.
     */
    int get_balance(TreeNode *node);

    /**
     * @brief Performs a right rotation on the given node.
     * @param node Pointer to the node.
     * @return Pointer to the new root after the rotation.
     */
    TreeNode *right_rotate(TreeNode *node);

    /**
     * @brief Performs a left rotation on the given node.
     * @param root Pointer to the root node.
     * @return Pointer to the new root after the rotation.
     */
    TreeNode *left_rotate(TreeNode *root);

    /**
     * @brief Finds the node with the minimum value in the given subtree.
     * @param root Pointer to the root of the subtree.
     * @return Pointer to the node with the minimum value.
     */
    TreeNode *min_value(TreeNode *root);

    /**
     * @brief Inserts a new node with the given item into the AVL tree.
     * @param node Pointer to the current node.
     * @param item The item to be inserted.
     * @return Pointer to the root of the modified subtree.
     */
    TreeNode *insert_node(TreeNode *node, std::string item);

    /**
     * @brief Removes the node with the given item from the AVL tree.
     * @param node Pointer to the current node.
     * @param item The item to be removed.
     * @return Pointer to the root of the modified subtree.
     */
    TreeNode *remove_node(TreeNode *node, std::string item);

public:

    /**
     * @brief Destructor for the AVLTree class.
     */
    ~AVLTree();

    /**
     * @brief Inserts a new item into the AVL tree.
     * @param item The item to be inserted.
     */
    void insert(std::string item);

    /**
     * @brief Removes the item from the AVL tree.
     * @param item The item to be removed.
     */
    void remove(std::string item);

    /**
     * @brief Returns a pointer to the root node of the AVL tree.
     * @return Pointer to the root node.
     */
    TreeNode *get_root();
};
