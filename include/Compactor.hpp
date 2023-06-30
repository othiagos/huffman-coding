#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "HashTable.hpp"
#include "TreeNodeChar.hpp"
#include "QuickSort.hpp"
#include "CompactorExcp.hpp"

#define BUFFER_SIZE 16384

#define UTF8_ENCODING_1BYTE 0x00
#define UTF8_ENCODING_2BYTE 0x06
#define UTF8_ENCODING_3BYTE 0x0E
#define UTF8_ENCODING_4BYTE 0x1E

#define DISCARD_7BIT 7
#define DISCARD_5BIT 5
#define DISCARD_4BIT 4
#define DISCARD_3BIT 3

#define LEN_1BYTE 7
#define LEN_2BYTE 15
#define LEN_3BYTE 23
#define LEN_4BYTE 31

using std::string;

class Compactor {
private:
    static void count_char(string file_path, HashTable<TreeNodeChar> *result);
    static void huffman_algorithm(LinkedList<TreeNodeChar> &list);
    static void in_order(LinkedList<table> &table_char, std::string &bits, uint64_t &bit_len, unsigned int &bytes_size, TreeNodeChar *tree);
    static void write_file_compress(string file_path, TreeNodeChar *tree, LinkedList<TreeNodeChar> &list);
    static uint8_t str2byte(string &str);
    static int32_t binary_search_table(table *vec, uint32_t size, string str);

public:
    static void compress(std::string file_path);
    static void decompress(std::string file_path);
};
;