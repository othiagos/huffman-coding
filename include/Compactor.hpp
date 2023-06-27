#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <bitset>

#include "HashTable.hpp"
#include "TreeNodeChar.hpp"
#include "QuickSort.hpp"

#define UTF8_ENCODING_1BYTE 0x00
#define UTF8_ENCODING_2BYTE 0x06
#define UTF8_ENCODING_3BYTE 0x0E
#define UTF8_ENCODING_4BYTE 0x1E

#define DISCARD_7BIT 7
#define DISCARD_5BIT 5
#define DISCARD_4BIT 4
#define DISCARD_3BIT 3

struct table {
    std::string chars;
    std::string encoding;
};

class Compactor {
private:
    static void count_char(std::ifstream *file, HashTable<TreeNodeChar> *result);
    static void huffman_algorithm(LinkedList<TreeNodeChar> &list);
    static void in_order(LinkedList<table> &table_char, std::string &bits, TreeNodeChar *tree);
    static void write_file_compress(std::ifstream *file, TreeNodeChar *tree, LinkedList<TreeNodeChar> *list);

public:
    static void compress(char *file_path);
    static void decompress(std::string file_path);
};
