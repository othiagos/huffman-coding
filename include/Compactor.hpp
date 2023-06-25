#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

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

class Compactor {
private:
    static void count_char(std::ifstream *file, HashTable<TreeNodeChar> *result);
    static void huffman_algorithm(LinkedList<TreeNodeChar> &list);

public:
    static void compress(char *file_path);
    static void uncompress(char *file_path);
};
