#pragma once

#include <cmath>
#include <string>


/**
 * @struct table
 * @brief Represents a table structure with characters and their encodings.
 */
struct table {
    std::string chars;
    std::string encoding;
};

/**
 * @class TreeNode
 * @brief Represents a node in a tree.
 */
class TreeNode {
private:
    std::string _chars;
    unsigned int _count = 0;
    TreeNode *_right = nullptr;
    TreeNode *_left = nullptr;

public:
    /**
     * @brief Default constructor for the TreeNode class.
     */
    TreeNode();

    /**
     * @brief Destructor for the TreeNode class.
     */
    ~TreeNode();

    /**
     * @brief Copy constructor for the TreeNode class.
     * @param o The TreeNode object to be copied.
     */
    TreeNode(const TreeNode &o);

    /**
     * @brief Constructor for the TreeNode class with characters.
     * @param _chars The characters to be stored in the node.
     */
    TreeNode(std::string _chars);

    /**
     * @brief Constructor for the TreeNode class with count.
     * @param count The count associated with the characters.
     */
    TreeNode(unsigned int count);

    /**
     * @brief Deletes the given node and its children.
     * @param node Pointer to the node to be deleted.
     */
    static void DeleteNode(TreeNode *node);

    /**
     * @brief Overloaded equality operator for the TreeNode class.
     * @param o The TreeNode object to be compared.
     * @return True if the nodes are equal, false otherwise.
     */
    bool operator==(const TreeNode& o);

    /**
     * @brief Overloaded assignment operator for the TreeNode class.
     * @param o The TreeNode object to be assigned.
     * @return Reference to the assigned TreeNode object.
     */
    TreeNode& operator=(const TreeNode& o);

    /**
     * @brief Returns the characters stored in the node.
     * @return The characters.
     */
    std::string get_chars() const;

    /**
     * @brief Sets the characters for the node.
     * @param value The characters to be set.
     */
    void set_chars(std::string value);

    /**
     * @brief Returns the count associated with the characters.
     * @return The count.
     */
    unsigned int get_count() const;

    /**
     * @brief Sets the count for the node.
     * @param value The count to be set.
     */
    void set_count(unsigned int value);

    /**
     * @brief Returns a pointer to the right child node.
     * @return Pointer to the right child node.
     */
    TreeNode *get_right();

    /**
     * @brief Sets the right child node.
     * @param node Pointer to the right child node.
     */
    void set_right(TreeNode *node);

    /**
     * @brief Returns a pointer to the left child node.
     * @return Pointer to the left child node.
     */
    TreeNode *get_left();

    /**
     * @brief Sets the left child node.
     * @param node Pointer to the left child node.
     */
    void set_left(TreeNode *node);
};
