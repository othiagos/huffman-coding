#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "AVLTree.hpp"
#include "TreeNode.hpp"
#include "QuickSort.hpp"
#include "CompactorExcp.hpp"

#define BUFFER_SIZE 16384

#define UTF8_ENCODING_1BYTE 0x00
#define UTF8_ENCODING_2BYTE 0x06
#define UTF8_ENCODING_3BYTE 0x0E
#define UTF8_ENCODING_4BYTE 0x1E
#define UTF8_ENCODING_XBYTE 0x02

#define DISCARD_7BIT 7
#define DISCARD_5BIT 5
#define DISCARD_6BIT 6
#define DISCARD_4BIT 4
#define DISCARD_3BIT 3

#define LEN_1BYTE 7
#define LEN_2BYTE 15
#define LEN_3BYTE 23
#define LEN_4BYTE 31

using std::string;

/**
 * @brief Class responsible for file compression and decompression.
 */
class Compactor {
private:
    /**
     * @brief Counts the frequency of each character in a file.
     * @param file_path The path of the file.
     * @param result AVLTree to store the characters and their frequencies.
     * @throw compexcp::CouldNotOpenFile If the file cannot be opened.
     * @throw compexcp::FileNotUTF8 If the file is not encoded in UTF-8.
     */
    static void count_char(string file_path, AVLTree &result);

    /**
     * @brief Converts a tree into a linked list.
     * @param tree The tree.
     * @param list The linked list to store the tree nodes.
     */
    static void tree2list(TreeNode *tree, LinkedList<TreeNode*> &list);

    /**
     * @brief Implements the Huffman algorithm to build the Huffman tree.
     * @param list The linked list of nodes to be used in the tree construction.
     */
    static void huffman_algorithm(LinkedList<TreeNode*> &list);

    /**
     * @brief Performs binary search in a character Table.
     * @param vec The array of character tables.
     * @param size The size of the array.
     * @param str The string to search for.
     * @return The index of the string in the Table, or -1 if not found.
     */
    static int32_t binary_search_table(table *vec, uint32_t size, string str);

    /**
     * @brief Converts a string to a numeric value represented in a byte.
     * @param str The string to convert.
     * @return The numeric value represented in a byte.
     */
    static uint8_t str2byte(string &str);

    /**
     * @brief Performs an in-order traversal on the Huffman tree and generates the character Table.
     * @param table_char The linked list to store the character Table.
     * @param bits The string with the Huffman encoding bits.
     * @param bit_len The length of the bit sequence.
     * @param bytes_size The file size in bytes.
     * @param tree The Huffman tree.
     */
    static void in_order(LinkedList<table> &table_char, string &bits, uint64_t &bit_len,
        uint32_t &bytes_size, TreeNode *tree);

    /**
     * @brief Writes the compressed file based on the Huffman tree.
     * @param input_path The path of the input file.
     * @param output_path The path of the compressed output file.
     * @param tree The Huffman tree.
     * @param list The linked list of nodes used in the tree construction.
     * @throw compexcp::CouldNotOpenFile If the file cannot be opened.
     */
    static void write_file_compress(string input_path, string output_path, TreeNode *tree,
        LinkedList<TreeNode*> &list);

public:
    /**
     * @brief Compresses a file.
     * @param input_path The path of the input file.
     * @param output_path The path of the compressed output file.
     * @throw compexcp::CouldNotOpenFile If the file cannot be opened.
     * @throw compexcp::FileNotUTF8 If the file is not encoded in UTF-8.
     */
    static void compress(string input_path, string output_path);
    
    /**
     * @brief Decompresses a compressed file.
     * @param input_path The path of the compressed input file.
     * @param output_path The path of the decompressed output file.
     * @throw compexcp::CouldNotOpenFile If the file cannot be opened.
     */
    static void decompress(string input_path, string output_path);
};