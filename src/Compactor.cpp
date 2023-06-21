#include "Compactor.hpp"

void Compactor::count_char(std::ifstream *file, HashTable<TreeNodeChar> *result) {
    int size;

    file->seekg(0, file->end);
    size = file->tellg();
    file->seekg(0, file->beg);

    u_char buffer[size];
    file->read((char *)buffer, size);

    for (int i = 0; i < size; i++) {
        try {
            if (buffer[i] >> DISCARD_7BIT == UTF8_ENCODING_1BYTE) {
                char str[2] = {(char) buffer[i], '\0'};
                std::string code_point(str);
                TreeNodeChar node = TreeNodeChar(code_point);
                result->insert(node);
            }
            else if (buffer[i] >> DISCARD_5BIT == UTF8_ENCODING_2BYTE) {
                char str[3] = {(char) buffer[i], (char) buffer[i + 1], '\0'};
                std::string code_point(str);
                TreeNodeChar node = TreeNodeChar(code_point);
                result->insert(node);
                i++;
            }
            else if (buffer[i] >> DISCARD_4BIT == UTF8_ENCODING_3BYTE) {
                char str[4] = {(char) buffer[i],(char) buffer[i + 1], (char) buffer[i + 2], '\0'};
                std::string code_point(str);
                TreeNodeChar node = TreeNodeChar(code_point);
                result->insert(node);
                i += 2;
            }
            else if (buffer[i] >> DISCARD_3BIT == UTF8_ENCODING_4BYTE) {
                char str[5] = {(char) buffer[i],(char) buffer[i + 1], (char) buffer[i + 2], (char) buffer[i + 3], '\0'};
                std::string code_point(str);
                TreeNodeChar node = TreeNodeChar(code_point);
                result->insert(node);
                i += 3;
            }
        } catch (htexcp::ItemExists<TreeNodeChar> &e) {
            TreeNodeChar *item = e.get_item();
            (*item)++;
        }
    }
}

void Compactor::compress(char *file_path) {
    std::ifstream file;
    HashTable<TreeNodeChar> table;

    file.open(file_path);
    if (!file.is_open())
        throw "Could not open the file!";

    count_char(&file, &table);
    LinkedList<TreeNodeChar> list;

    table.get_list(list);
    QuickSort::sort(list);

    // huffalgorithms
    // writefilecompress

    file.close();
}

void Compactor::uncompress(char *file_path) {
}