#include "Compactor.hpp"

void Compactor::count_char(string file_path, HashTable<TreeNodeChar> *result) {

    std::ifstream file(file_path, std::ios::in);
    if (!file.is_open())
        throw compexcp::CouldntOpenFile();
    
    uint64_t file_size = 0;
    uint16_t b_index = 0;

    file.seekg(0, file.end);
    file_size = file.tellg();
    file.seekg(0, file.beg);

    u_char *buffer = new u_char[BUFFER_SIZE];
    file.read((char *)buffer, BUFFER_SIZE);

    for (uint64_t i = 0; i < file_size; i++) {
        try {
            if (buffer[b_index] >> DISCARD_7BIT == UTF8_ENCODING_1BYTE) {
                string str({(char) buffer[b_index]});

                b_index++;
                result->insert(TreeNodeChar(str));
            }
            else if (buffer[b_index] >> DISCARD_5BIT == UTF8_ENCODING_2BYTE) {
                if (b_index + 1 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((b_index + 2) % BUFFER_SIZE);

                string str({(char) buffer[b_index], (char) buffer[b_index + 1]});

                i++;
                b_index += 2;
                result->insert(TreeNodeChar(str));
            }
            else if (buffer[b_index] >> DISCARD_4BIT == UTF8_ENCODING_3BYTE) {
                if (b_index + 2 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((b_index + 3) % BUFFER_SIZE);

                string str({(char) buffer[b_index],(char) buffer[b_index + 1],
                    (char) buffer[b_index + 2]});

                i += 2;
                b_index += 3;
                result->insert(TreeNodeChar(str));
            }
            else if (buffer[b_index] >> DISCARD_3BIT == UTF8_ENCODING_4BYTE) {
                if (b_index + 3 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((b_index + 4) % BUFFER_SIZE);
                string str({(char) buffer[b_index],(char) buffer[b_index + 1],
                    (char) buffer[b_index + 2], (char) buffer[b_index + 3]});

                i += 3;
                b_index += 4;
                result->insert(TreeNodeChar(str));
            }
        } catch (htexcp::ItemExists<TreeNodeChar> &e) {
            TreeNodeChar *item = e.get_item();
            (*item)++;
        } catch (const compexcp::BufferEnd &e) {
            u_char *temp_buffer = new u_char[BUFFER_SIZE];
            file.read((char *)temp_buffer, BUFFER_SIZE);

            string str;
            while (b_index < BUFFER_SIZE) {
                str.push_back(buffer[b_index]);
                i++;
                b_index++;
            }
            
            b_index = 0;
            while (b_index < e.get_overflow_size()) {
                str.push_back(temp_buffer[b_index]);
                i++;
                b_index++;
            }

            i--;

            try {
                result->insert(TreeNodeChar(str));
            }
            catch(htexcp::ItemExists<TreeNodeChar> &e) {
                TreeNodeChar *item = e.get_item();
                (*item)++;
            }

            delete[] buffer;
            buffer = temp_buffer;
        }

        if (b_index >= BUFFER_SIZE) {
            file.read((char *)buffer, BUFFER_SIZE);
            b_index = 0;
        } 
    }
    file.close();
    delete[] buffer;
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

void Compactor::in_order(LinkedList<table> &table_char, std::string &bits, uint64_t &bit_len, unsigned int &bytes_size, TreeNodeChar *tree) {
    if(tree->get_right() != nullptr) {
        bits.push_back('0');
        in_order(table_char, bits, bit_len, bytes_size, tree->get_right());
        bits.pop_back();
    }

    if(tree->get_left() != nullptr) {
        bits.push_back('1');
        in_order(table_char, bits, bit_len, bytes_size, tree->get_left());
        bits.pop_back();
    }

    if (tree->get_left() == tree->get_right()) {
        table_char.push_back({tree->get_chars(), bits});
        bytes_size += tree->get_chars().length();
        bit_len += bits.size() * tree->get_count();
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

uint8_t Compactor::str2byte(string &str) {
    string substr = str.substr(0, 8);
    
    if (str.size() > 7)
        str = str.substr(8, str.size() - 1);
    else
        str = "";

    uint8_t byte = 0;
    for (uint8_t i = 0; i < substr.size(); i++) {
        if (substr[i] == '1')
            byte += pow(2, 7 - i);
    }

    return byte;
}

void Compactor::write_file_compress(string file_path, TreeNodeChar *tree, LinkedList<TreeNodeChar> &list) {
    std::string bits = "";
    LinkedList<table> table_char;
    unsigned int bytes_size = list.size();
    unsigned int max_item = list[list.size() - 1].get_count();
    uint64_t bit_len = 0;
    
    in_order(table_char, bits, bit_len, bytes_size, tree);

    char size = 0;

    std::string filename = file_path.substr(0, file_path.find("."));

    std::ofstream new_file(filename + ".tzip", std::ios::out | std::ios::binary);
    if (!new_file.is_open())
        throw compexcp::CouldntOpenFile();

    size = file_path.size(); 
    new_file.write((char*) &size, sizeof(char));
    new_file.write(file_path.c_str(), size * sizeof(char));

    if (max_item >> LEN_1BYTE == 0) size = 1;
    else if (max_item >> LEN_2BYTE == 0) size = 2;
    else if (max_item >> LEN_3BYTE == 0) size = 3;
    else if (max_item >> LEN_4BYTE == 0) size = 4;

    new_file.write((char *) &size, sizeof(char));
    new_file.write((char *) &bytes_size, sizeof(unsigned int));

    uint64_t i;
    for (TreeNodeChar node : list) {
        i = node.get_count();
        new_file.write((char *) node.get_chars().c_str(), node.get_chars().size());
        new_file.write((char *) &i, size);
    }

    new_file.write((char *) &bit_len, sizeof(uint64_t));

    std::ifstream file(file_path, std::ios::in);
    if (!file.is_open())
        throw compexcp::CouldntOpenFile();
    
    uint64_t file_size;
    u_char *buffer_read = new u_char[BUFFER_SIZE];
    uint16_t br_index = 0;

    file.seekg(0, file.end);
    file_size = file.tellg();
    file.seekg(0, file.beg);

    uint8_t *buffer_write = new uint8_t[BUFFER_SIZE];
    uint16_t bw_index = 0;

    i = 0;
    std::string bit_string = "";
    file.read((char *) buffer_read, BUFFER_SIZE);

    while (i < file_size) {
        try {
            if (buffer_read[br_index] >> DISCARD_7BIT == UTF8_ENCODING_1BYTE) {
                string str({(char) buffer_read[br_index]});

                for (table s : table_char) {
                    if (str == s.chars) {
                        bit_string += s.encoding;
                        br_index++;
                        i++;

                        if (bit_string.size() >= 8) {
                            buffer_write[bw_index] = str2byte(bit_string);
                            bw_index++;
                        }
                        break;
                    }
                }

            }
            else if (buffer_read[br_index] >> DISCARD_5BIT == UTF8_ENCODING_2BYTE) {
                if (br_index + 1 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((br_index + 2) % BUFFER_SIZE);

                string str({(char) buffer_read[br_index], (char) buffer_read[br_index + 1]});
                
                for (table s : table_char) {
                    if (str == s.chars) {
                        bit_string += s.encoding;
                        br_index += 2;
                        i += 2;

                        if (bit_string.size() >= 8) {
                            buffer_write[bw_index] = str2byte(bit_string);
                            bw_index++;
                        }
                        break;
                    }
                }
            }
            else if (buffer_read[br_index] >> DISCARD_4BIT == UTF8_ENCODING_3BYTE) {
                if (br_index + 2 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((br_index + 3) % BUFFER_SIZE);

                string str({(char) buffer_read[br_index], (char) buffer_read[br_index + 1],
                    (char) buffer_read[br_index + 2]});
                
                for (table s : table_char) {
                    if (str == s.chars) {
                        bit_string += s.encoding;
                        br_index += 3;
                        i += 3;

                        if (bit_string.size() >= 8) {
                            buffer_write[bw_index] = str2byte(bit_string);
                            bw_index++;
                        }
                        break;
                    }
                }
            }
            else if (buffer_read[br_index] >> DISCARD_3BIT == UTF8_ENCODING_4BYTE) {
                if (br_index + 3 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((br_index + 4) % BUFFER_SIZE);
                
                string str({(char) buffer_read[br_index],(char) buffer_read[br_index+ 1],
                    (char) buffer_read[br_index + 2],(char) buffer_read[br_index + 3]});
                
                for (table s : table_char) {
                    if (str == s.chars) {
                        bit_string += s.encoding;
                        br_index += 4;
                        i += 4;

                        if (bit_string.size() >= 8) {
                            buffer_write[bw_index] = str2byte(bit_string);
                            bw_index++;
                        }
                        break;
                    }
                }
            }
        } catch(const compexcp::BufferEnd& e) {
            u_char *temp_buffer = new u_char[BUFFER_SIZE];
            file.read((char *)temp_buffer, BUFFER_SIZE);

            uint64_t ti = i, tb = br_index;
            string str;
            while (tb < BUFFER_SIZE) {
                str.push_back(buffer_read[tb]);
                tb++;
                ti++;
            }
            
            tb = 0;
            while (tb < e.get_overflow_size()) {
                str.push_back(temp_buffer[tb]);
                tb++;
                ti++;
            }

            for (table s : table_char) {
                if (str == s.chars) {
                    bit_string += s.encoding;
                    br_index = tb;
                    i = ti;

                    if (bit_string.size() >= 8) {
                        buffer_write[bw_index] = str2byte(bit_string);
                        bw_index++;
                    }
                    break;
                }
            }
            delete[] buffer_read;
            buffer_read = temp_buffer;
        }

        if (bw_index >= BUFFER_SIZE) {
            new_file.write((char*) buffer_write, BUFFER_SIZE);
            bw_index = 0;
        }

        if (br_index >= BUFFER_SIZE) {
            file.read((char*) buffer_read, BUFFER_SIZE);
            br_index = 0;
        }
    }

    if (bit_string.size() > 0) {
        buffer_write[bw_index] = str2byte(bit_string);
        new_file.write((char *) buffer_write, bw_index + 1);
    }

    delete[] buffer_read;
    delete[] buffer_write;
}

void Compactor::compress(std::string file_path) {
    clock_t t;
    t = clock();
    HashTable<TreeNodeChar> table;
    t = clock() - t;
    std::cout << "Cria hashtable: " << ((float)t)/CLOCKS_PER_SEC << 's' << std::endl;

    t = clock();
    count_char(file_path, &table);
    t = clock() - t;
    std::cout << "Contar o caracteres: " << ((float)t)/CLOCKS_PER_SEC << 's' << std::endl;

    t = clock();
    LinkedList<TreeNodeChar> list;
    t = clock() - t;
    std::cout << "Cria lista: " << ((float)t)/CLOCKS_PER_SEC << 's' << std::endl;

    t = clock();
    table.get_list(list);
    t = clock() - t;
    std::cout << "Gerar list da hashtable: " << ((float)t)/CLOCKS_PER_SEC << 's' << std::endl;

    t = clock();
    QuickSort::sort(list);
    t = clock() - t;
    std::cout << "Ordenar Quicksort: " << ((float)t)/CLOCKS_PER_SEC << 's' << std::endl;

    t = clock();
    LinkedList<TreeNodeChar> tree = LinkedList<TreeNodeChar>(list);
    t = clock() - t;
    std::cout << "Copiar lista: " << ((float)t)/CLOCKS_PER_SEC << 's' << std::endl;

    t = clock();
    huffman_algorithm(tree);
    t = clock() - t;
    std::cout << "Algoritmo de huffman: " << ((float)t)/CLOCKS_PER_SEC << 's' << std::endl;

    t = clock();
    write_file_compress(file_path, &tree[0], list);
    t = clock() - t;
    std::cout << "Compactar arquivo: " << ((float)t)/CLOCKS_PER_SEC << 's' << std::endl;
}

void Compactor::decompress(std::string file_path) {
    std::ifstream file(file_path, std::ios::in | std::ios::binary);

    if (!file.is_open())
        throw "Could not open the file!";

    uint8_t size = 0;
    file.read((char *) &size, sizeof(uint8_t));

    char *filename = new char[size];
    file.read(filename, size);

    file.read((char *) &size, sizeof(uint8_t));
    uint32_t len;
    file.read((char *) &len, sizeof(uint32_t));

    LinkedList<TreeNodeChar> list;
    TreeNodeChar t;

    u_char *buffer = new u_char[BUFFER_SIZE];
    if (len >= BUFFER_SIZE)
        file.read((char *) buffer, BUFFER_SIZE);
    else 
        file.read((char *) buffer, len);
    uint16_t b_index = 0;
    
    for (uint32_t i = 0; i < len; i++) {
        try {
            if (buffer[b_index] >> DISCARD_7BIT == UTF8_ENCODING_1BYTE) {
                if (b_index + 1 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((b_index + 2) % BUFFER_SIZE);

                t.set_chars({(char) buffer[b_index]});
                t.set_count(buffer[b_index + 1]);

                list.push_back(t);
                i++;
                b_index += 2;
            }
            else if (buffer[b_index] >> DISCARD_5BIT == UTF8_ENCODING_2BYTE) {
                if (b_index + 2 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((b_index + 3) % BUFFER_SIZE);

                t.set_chars({(char) buffer[b_index], (char) buffer[b_index + 1]});
                t.set_count(buffer[b_index + 2]);

                list.push_back(t);
                i += 2;
                b_index += 3;
            }
            else if (buffer[b_index] >> DISCARD_4BIT == UTF8_ENCODING_3BYTE) {
                if (b_index + 3 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((b_index + 4) % BUFFER_SIZE);

                t.set_chars({(char) buffer[b_index],(char) buffer[b_index + 1],
                    (char) buffer[b_index + 2]});
                t.set_count(buffer[b_index + 3]);

                list.push_back(t);
                i += 3;
                b_index += 4;
            }
            else if (buffer[b_index] >> DISCARD_3BIT == UTF8_ENCODING_4BYTE) {
                if (b_index + 4 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((b_index + 5) % BUFFER_SIZE);

                t.set_chars({(char) buffer[b_index],(char) buffer[b_index + 1], 
                    (char) buffer[b_index + 2], (char) buffer[b_index + 3]});
                t.set_count(buffer[b_index + 4]);

                list.push_back(t);
                i += 4;
                b_index += 5;
            }
        } catch (const compexcp::BufferEnd& e) {
            u_char *temp_buffer = new u_char[BUFFER_SIZE];
            if (len - 1 - i >= BUFFER_SIZE)
                file.read((char*) temp_buffer, BUFFER_SIZE);
            else
                file.read((char*) temp_buffer, (len - 1 - i) % BUFFER_SIZE);

            string str;
            while (b_index < BUFFER_SIZE) {
                str.push_back(buffer[b_index]);
                i++;
                b_index++;
            }
            
            b_index = 0;
            while (b_index < e.get_overflow_size() - 1) {
                str.push_back(temp_buffer[b_index]);
                i++;
                b_index++;
            }
            
            t.set_chars(str);
            t.set_count(temp_buffer[b_index]);

            list.push_back(t);
            b_index++;

            delete[] buffer;
            buffer = temp_buffer;
        }

        if (b_index >= BUFFER_SIZE) {
            if (len - 1 - i >= BUFFER_SIZE)
                file.read((char*) buffer, BUFFER_SIZE);
            else
                file.read((char*) buffer, (len - i) % BUFFER_SIZE);
            b_index = 0;
        }
    }

    delete[] buffer;

    huffman_algorithm(list);

    std::ofstream new_file(std::string(filename) + "(1).txt", std::ios::out | std::ios::binary);
    // if (!new_file.is_open())
    //     throw compexcp::CouldntOpenFile();

    uint64_t sum_freq;
    file.read((char *)&sum_freq, sizeof(uint64_t));
    uint64_t bytes = ceil(sum_freq / 8.0);

    buffer = new u_char[BUFFER_SIZE];
    u_char *buffer_write = new u_char[BUFFER_SIZE];
    uint16_t bw_index = 0;
    
    file.read((char *)buffer, BUFFER_SIZE);

    TreeNodeChar *tree = &list[0];
    uint64_t count_bits = 0;

    for (b_index = 0; b_index < bytes; b_index++) {
        for (uint8_t j = 0; j < 8 && count_bits <= sum_freq; j++) {
            if (b_index >= BUFFER_SIZE) {
                file.read((char*) buffer, BUFFER_SIZE);
                b_index = 0;
            }

            if (tree->get_chars() != "") {
                string str = tree->get_chars();
                for (uint8_t i = 0; i < str.size(); i++) {
                    buffer_write[bw_index] = str[i];
                    bw_index++;

                    if (bw_index >= BUFFER_SIZE) {
                        new_file.write((char*) buffer_write, BUFFER_SIZE);
                        bw_index = 0;
                    }
                }
                tree = &list[0];
            }

            if (buffer[b_index] >> (7 - j) & 1) {
                if (tree->get_left() != nullptr)
                    tree = tree->get_left();
            }
            else {
                if (tree->get_right() != nullptr)
                    tree = tree->get_right();
            }
            count_bits++;

            if (bw_index >= BUFFER_SIZE) {
                new_file.write((char*) buffer_write, BUFFER_SIZE);
                bw_index = 0;
            }
        }
        if (count_bits > sum_freq) 
            break;
    }

    if (bw_index >= 0)
        new_file.write((char*) buffer_write, bw_index);

    delete[] filename;
    delete[] buffer;
    delete[] buffer_write;

    new_file.close();
    file.close();
}