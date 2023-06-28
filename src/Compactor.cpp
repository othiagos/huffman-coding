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

void Compactor::in_order(LinkedList<table> &table_char, std::string &bits, unsigned int &bytes_size, TreeNodeChar *tree) {
    if(tree->get_right() != nullptr) {
        bits.push_back('0');
        in_order(table_char, bits,bytes_size, tree->get_right());
        bits.pop_back();
    }

    if(tree->get_left() != nullptr) {
        bits.push_back('1');
        in_order(table_char, bits, bytes_size, tree->get_left());
        bits.pop_back();
    }

    if (tree->get_left() == tree->get_right()) {
        table_char.push_back({tree->get_chars(), bits});
        bytes_size += tree->get_chars().length();
    }
}

void Compactor::reverse_byte(std::string &str) {
    if (str.size() % 8 != 0)
        throw "invalid string";

    for (int i = 0; i < (int) str.size() / 8; i++) {
        for (int j = 0; j < 4; j++) {
            if (str[i * 8 + j] != str[i * 8 + 7 - j]) {
                char aux = str[i * 8 + j];
                str[i * 8 + j] = str[i * 8 + 7 - j];
                str[i * 8 +7 - j] = aux;
            }
        }
    }
}

void Compactor::reverse_str(std::string &str) {
    int size = str.size();
    for (int i = 0; i < size / 2; i++) {
        if (str[i] != str[size - 1 - i]) {
            char aux = str[size - 1 - i];
            str[size - 1 - i] = str[i];
            str[i] = aux;
        }
    }
}

void Compactor::write_file_compress(std::ifstream *file, TreeNodeChar *tree, LinkedList<TreeNodeChar> &list, std::string file_path) {
    std::string bits = "";
    LinkedList<table> table_char;
    unsigned int bytes_size = list.size();
    unsigned int max_item = list[list.size() - 1].get_count();
    unsigned int len;

    file->seekg(0, file->end);
    len = file->tellg();
    file->seekg(0, file->beg);

    u_char buffer[len];
    file->read((char *)buffer, len);
    
    in_order(table_char, bits, bytes_size, tree);

    char size = 0;

    std::string filename = file_path.substr(0, file_path.find("."));

    std::ofstream new_file(filename + ".tzip", std::ios::out | std::ios::binary);
    size = file_path.size(); 
    new_file.write((char*) &size, sizeof(char));
    new_file.write(file_path.c_str(), size * sizeof(char));

    if (max_item >> LEN_1BYTE == 0) size = 1;
    else if (max_item >> LEN_2BYTE == 0) size = 2;
    else if (max_item >> LEN_3BYTE == 0) size = 3;
    else if (max_item >> LEN_4BYTE == 0) size = 4;

    new_file.write((char *) &size, sizeof(char));
    new_file.write((char *) &bytes_size, sizeof(unsigned int));

    int i;
    for (TreeNodeChar node : list) {
        i = node.get_count();
        new_file.write((char *) node.get_chars().c_str(), node.get_chars().size());
        new_file.write((char *) &i, size);
    }

    std::string bit_string = "";
    for (i = 0; i < (int) len; i++) {
        for (table s : table_char) {

            if (buffer[i] >> DISCARD_7BIT == UTF8_ENCODING_1BYTE && s.chars.size() == 1) {
                char str[2] = {(char) buffer[i], '\0'};
                std::string code_point(str);

                if (code_point == s.chars) {
                    std::string aux = s.encoding;
                    reverse_str(aux);
                    bit_string = aux + bit_string;
                    std::cout << s.encoding;
                }

            }
            else if (buffer[i] >> DISCARD_5BIT == UTF8_ENCODING_2BYTE && s.chars.size() == 2) {
                char str[3] = {(char) buffer[i], (char) buffer[i + 1], '\0'};
                std::string code_point(str);
                
                if (code_point == s.chars) {
                    std::string aux = s.encoding;
                    reverse_str(aux);
                    bit_string = aux + bit_string;
                    std::cout << s.encoding;
                    i += 1;
                }

            }
            else if (buffer[i] >> DISCARD_4BIT == UTF8_ENCODING_3BYTE && s.chars.size() == 3) {
                char str[4] = {(char) buffer[i],(char) buffer[i + 1], (char) buffer[i + 2], '\0'};
                std::string code_point(str);
                
                if (code_point == s.chars) {
                    std::string aux = s.encoding;
                    reverse_str(aux);
                    bit_string = aux + bit_string;
                    std::cout << s.encoding;
                    i += 2;
                }

            }
            else if (buffer[i] >> DISCARD_3BIT == UTF8_ENCODING_4BYTE && s.chars.size() == 4) {
                char str[5] = {(char) buffer[i],(char) buffer[i + 1],(char) buffer[i + 2],(char) buffer[i + 3], '\0'};
                std::string code_point(str);
                
                if (code_point == s.chars) {
                    std::string aux = s.encoding;
                    reverse_str(aux);
                    bit_string = aux + bit_string;
                    std::cout << s.encoding;
                }
                i += 3;
            }

            if (bit_string.size() > 32) {
                std::string bit_s = bit_string.substr(bit_string.size() - 32, 32);
                reverse_byte(bit_s);

                std::bitset<sizeof(bit_s)> b(bit_s);
                unsigned long c = b.to_ulong();

                new_file.write((char *) &c, bit_s.size() / 8);

                bit_string = bit_string.substr(0, bit_string.size() - 32);
            }
        }

    }

    if (bit_string.size() > 0) {

        while (bit_string.size() % 8 != 0)
            bit_string = "0" + bit_string;
        
        reverse_byte(bit_string);
        std::bitset<sizeof(bit_string)> b(bit_string);
        unsigned long c = b.to_ulong();

        new_file.write((char *) &c, bit_string.size() / 8);
    }
}

void Compactor::compress(std::string file_path) {
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
    write_file_compress(&file, &tree[0], list, file_path);

    file.close();
}

void Compactor::decompress(std::string file_path) {
}