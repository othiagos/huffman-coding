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

void Compactor::huffman_algorithm(LinkedList<TreeNodeChar> &list) {
   TreeNodeChar x;
   TreeNodeChar y;
   TreeNodeChar z;

    int i;
    while (list.size() != 1) {
        x = list.pop_front();
        y = list.pop_front();
        z = TreeNodeChar(x.get_count() + y.get_count(), x, y);

        for (i = 0; i < list.size(); i++) {
            if (list[i].get_count() >= z.get_count()) {
                list.insert(z, i);
                break;
            }
            else if (i + 1 == list.size()) {
                list.push_back(z);
                break;
            }
        }

        if (list.size() == 0)
            list.push_back(z);
    }
}

#include <iostream>

void Compactor::in_order(LinkedList<table> &table_char, std::string &bits, TreeNodeChar *tree) {
    if(tree->get_right() != nullptr) {
        bits.push_back('0');
        in_order(table_char, bits, tree->get_right());
        bits.pop_back();
    }

    if(tree->get_left() != nullptr) {
        bits.push_back('1');
        in_order(table_char, bits, tree->get_left());
        bits.pop_back();
    }

    if (tree->get_left() == tree->get_right())
        table_char.push_back({tree->get_chars(), bits});
}

void Compactor::write_file_compress(std::ifstream *file, TreeNodeChar *tree, LinkedList<TreeNodeChar> *list) {
    std::string bits = "";
    LinkedList<table> table_char;
    
    in_order(table_char, bits, tree);

    for (table chars : table_char) {
        if (chars.chars == "\n")
            std::cout << "\\n" << ": " << chars.encoding<< bits << std::endl;
        else
            std::cout << chars.chars << ": " << chars.encoding << std::endl;
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

    LinkedList<TreeNodeChar> tree = LinkedList<TreeNodeChar>(list);
    huffman_algorithm(tree);
    write_file_compress(&file, &tree[0], &list);

    file.close();
}

void Compactor::decompress(std::string file_path) {
    // std::string filename = file_path.substr(0, file_path.find("."));
    // std::ofstream new_file(filename + ".tzip", std::ios::out | std::ios::binary);

    //  char a = 0;
    //  char b = 1;
    // std::bitset<sizeof(a)> bits(a);
    // unsigned long binary_value = bits.to_ulong();

    // bool b[2] = {0,1};

    // new_file.write((char *) &a, sizeof(char));
    // new_file.write((char *) &b, sizeof(char));

    // new_file.close();

}