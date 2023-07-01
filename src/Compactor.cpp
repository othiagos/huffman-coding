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

uint8_t Compactor::str2byte(string &str) {
    string substr = str.substr(0, 8);
    
    if (str.size() > 7)
        str = str.substr(8, str.size() - 1);
    else
        str = "";

    uint8_t byte = 0;
    for (uint8_t i = 0; i < substr.size(); i++) {
        if (substr[i] == '1')
            byte += 1 << (7 - i);
    }

    return byte;
}

int32_t Compactor::binary_search_table(table *vec, uint32_t size, string str) {
    uint64_t left = 0, right = size;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (vec[mid].chars.compare(str) == 0) {
            return mid;
        }
        else if (vec[mid].chars.compare(str) < 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

void Compactor::write_file_compress(string input_path, string output_path, TreeNodeChar *tree, LinkedList<TreeNodeChar> &list) {
    std::string bits = "";
    LinkedList<table> table_char;
    unsigned int bytes_size = 0;
    unsigned int max_item = list[list.size() - 1].get_count();
    uint64_t bit_len = 0;
    
    in_order(table_char, bits, bit_len, bytes_size, tree);

    QuickSort::sort(table_char);
    uint32_t table_size = table_char.size();
    table table_char_vec[table_size];

    uint64_t i = 0;
    for (table s : table_char) {
        table_char_vec[i] = s;
        i++;
    }

    std::ofstream new_file(output_path, std::ios::out | std::ios::binary);
    if (!new_file.is_open())
        throw compexcp::CouldntOpenFile();

    char size = 0;
    if (max_item >> LEN_1BYTE == 0) size = 1;
    else if (max_item >> LEN_2BYTE == 0) size = 2;
    else if (max_item >> LEN_3BYTE == 0) size = 3;
    else if (max_item >> LEN_4BYTE == 0) size = 4;
    
    bytes_size += list.size() * size;
    new_file.write((char *) &size, sizeof(char));
    new_file.write((char *) &bytes_size, sizeof(unsigned int));

    for (TreeNodeChar node : list) {
        i = node.get_count();
        new_file.write((char *) node.get_chars().c_str(), node.get_chars().size());
        new_file.write((char *) &i, size);
    }

    new_file.write((char *) &bit_len, sizeof(uint64_t));

    std::ifstream file(input_path, std::ios::in);
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

                int32_t index = binary_search_table(table_char_vec, table_size, str);
                if (index >= 0) {
                    table s = table_char_vec[index];
                    bit_string += s.encoding;
                    br_index++;
                    i++;

                    if (bit_string.size() >= 8) {
                        buffer_write[bw_index] = str2byte(bit_string);
                        bw_index++;
                    }
                }

            }
            else if (buffer_read[br_index] >> DISCARD_5BIT == UTF8_ENCODING_2BYTE) {
                if (br_index + 1 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((br_index + 2) % BUFFER_SIZE);

                string str({(char) buffer_read[br_index], (char) buffer_read[br_index + 1]});
                
                int32_t index = binary_search_table(table_char_vec, table_size, str);

                if (index >= 0) {
                    bit_string += table_char_vec[index].encoding;
                    br_index += 2;
                    i += 2;

                    if (bit_string.size() >= 8) {
                        buffer_write[bw_index] = str2byte(bit_string);
                        bw_index++;
                    }
                }
            }
            else if (buffer_read[br_index] >> DISCARD_4BIT == UTF8_ENCODING_3BYTE) {
                if (br_index + 2 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((br_index + 3) % BUFFER_SIZE);

                string str({(char) buffer_read[br_index], (char) buffer_read[br_index + 1],
                    (char) buffer_read[br_index + 2]});
                
                int32_t index = binary_search_table(table_char_vec, table_size, str);

                if (index >= 0) {
                    bit_string += table_char_vec[index].encoding;
                    br_index += 3;
                    i += 3;

                    if (bit_string.size() >= 8) {
                        buffer_write[bw_index] = str2byte(bit_string);
                        bw_index++;
                    }
                }
            }
            else if (buffer_read[br_index] >> DISCARD_3BIT == UTF8_ENCODING_4BYTE) {
                if (br_index + 3 >= BUFFER_SIZE)
                    throw compexcp::BufferEnd((br_index + 4) % BUFFER_SIZE);
                
                string str({(char) buffer_read[br_index],(char) buffer_read[br_index+ 1],
                    (char) buffer_read[br_index + 2],(char) buffer_read[br_index + 3]});
                
                
                int32_t index = binary_search_table(table_char_vec, table_size, str);

                if (index >= 0) {
                    bit_string += table_char_vec[index].encoding;
                    br_index += 4;
                    i += 4;

                    if (bit_string.size() >= 8) {
                        buffer_write[bw_index] = str2byte(bit_string);
                        bw_index++;
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
            while ((int) tb < e.get_overflow_size()) {
                str.push_back(temp_buffer[tb]);
                tb++;
                ti++;
            }

            int32_t index = binary_search_table(table_char_vec, table_size, str);
            
            if (index >= 0) {
                bit_string += table_char_vec[index].encoding;
                br_index = tb;
                i = ti;

                if (bit_string.size() >= 8) {
                    buffer_write[bw_index] = str2byte(bit_string);
                    bw_index++;
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

    if (bw_index > 0 || bit_string.size() > 0) {
        while (bit_string.size() > 0) {
            buffer_write[bw_index] = str2byte(bit_string);
            bw_index++;
        }

        new_file.write((char *) buffer_write, bw_index);
    }

    delete[] buffer_read;
    delete[] buffer_write;
}

void Compactor::compress(string input_path, string output_path) {
    HashTable<TreeNodeChar> table;
    count_char(input_path, &table);

    LinkedList<TreeNodeChar> list;
    table.get_list(list);

    QuickSort::sort(list);
    LinkedList<TreeNodeChar> tree = LinkedList<TreeNodeChar>(list);

    huffman_algorithm(tree);
    write_file_compress(input_path, output_path, &tree[0], list);
}

void Compactor::decompress(string input_path, string output_path) {
    std::ifstream file(input_path, std::ios::in | std::ios::binary);
    if (!file.is_open())
        throw compexcp::CouldntOpenFile();

    uint8_t size = 0;
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
                if (b_index + size >= BUFFER_SIZE)
                    throw compexcp::DecompressBufferEnd(
                        (b_index + size + 1) % BUFFER_SIZE, 1, size);

                t.set_chars({(char) buffer[b_index]});

                uint64_t number = 0;
                for (uint8_t j = 0; j < size; j++) {
                    number += buffer[b_index + 1 + j] << 8 * j;
                }
                
                t.set_count(number);

                list.push_back(t);
                i += size;
                b_index += 1 + size;
            }
            else if (buffer[b_index] >> DISCARD_5BIT == UTF8_ENCODING_2BYTE) {
                if (b_index + size + 1 >= BUFFER_SIZE)
                    throw compexcp::DecompressBufferEnd(
                        (b_index + size + 2) % BUFFER_SIZE, 2, size);

                t.set_chars({(char) buffer[b_index], (char) buffer[b_index + 1]});
                
                uint64_t number = 0;
                for (uint8_t j = 0; j < size; j++) {
                    number += buffer[b_index + 2 + j] << 8 * j;
                }
                
                t.set_count(number);

                list.push_back(t);
                i += 1 + size;
                b_index += 2 + size;
            }
            else if (buffer[b_index] >> DISCARD_4BIT == UTF8_ENCODING_3BYTE) {
                if (b_index + size + 2 >= BUFFER_SIZE)
                    throw compexcp::DecompressBufferEnd(
                        (b_index + size + 3) % BUFFER_SIZE, 3, size);

                t.set_chars({(char) buffer[b_index],(char) buffer[b_index + 1],
                    (char) buffer[b_index + 2]});
                
                uint64_t number = 0;
                for (uint8_t j = 0; j < size; j++) {
                    number += buffer[b_index + 3 + j] << 8 * j;
                }
                
                t.set_count(number);

                list.push_back(t);
                i += 2 + size;
                b_index += 3 + size;
            }
            else if (buffer[b_index] >> DISCARD_3BIT == UTF8_ENCODING_4BYTE) {
                if (b_index + size + 3 >= BUFFER_SIZE)
                    throw compexcp::DecompressBufferEnd(
                        (b_index + size + 4) % BUFFER_SIZE, 4, size);

                t.set_chars({(char) buffer[b_index],(char) buffer[b_index + 1], 
                    (char) buffer[b_index + 2], (char) buffer[b_index + 3]});
                
                uint64_t number = 0;
                for (uint8_t j = 0; j < size; j++) {
                    number += buffer[b_index + 4 + j] << 8 * j;
                }
                
                t.set_count(number);

                list.push_back(t);
                i += 3 + size;
                b_index += 4 + size;
            }
        } catch (const compexcp::DecompressBufferEnd& e) {
            u_char *temp_buffer = new u_char[BUFFER_SIZE];
            if (len - 1 - i >= BUFFER_SIZE)
                file.read((char*) temp_buffer, BUFFER_SIZE);
            else
                file.read((char*) temp_buffer, (len - 1 - i) % BUFFER_SIZE);

            string str;
            uint8_t k = 0;
            bool new_buffer = false;
            while (k < e.get_char_size()) {
                if (b_index < BUFFER_SIZE) {
                    str.push_back(buffer[b_index]);
                    i++;
                    b_index++;
                }
                else {
                    if (!new_buffer) {
                        new_buffer = true;
                        b_index = 0;
                    }

                    str.push_back(temp_buffer[b_index]);
                    i++;
                    b_index++;
                }
                k++;
            }

            if (b_index >= BUFFER_SIZE) {
                b_index = 0;
                new_buffer = true;
            }

            k = 0;
            uint64_t number = 0;
            for (uint8_t j = 0; j < e.get_number_size(); j++) {
                if (b_index >= BUFFER_SIZE) {
                    new_buffer = true;
                    b_index = 0;
                }
                if (new_buffer) {
                    number += temp_buffer[b_index] << 8 * j;
                    i++;
                    b_index++;
                }
                else {
                    number += buffer[b_index] << 8 * j;
                    i++;
                    b_index++;
                }
            }
            
            t.set_chars(str);
            t.set_count(number);

            list.push_back(t);
            i -= 1;

            delete[] buffer;
            buffer = temp_buffer;
        }

        if (b_index >= BUFFER_SIZE) {
            if (len - 1 - i >= BUFFER_SIZE)
                file.read((char*) buffer, BUFFER_SIZE);
            else
                file.read((char*) buffer, (len - 1 - i) % BUFFER_SIZE);
            b_index = 0;
        }
    }

    delete[] buffer;

    huffman_algorithm(list);

    std::ofstream new_file(output_path, std::ios::out | std::ios::binary);
    if (!new_file.is_open())
        throw compexcp::CouldntOpenFile();

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

    delete[] buffer;
    delete[] buffer_write;

    new_file.close();
    file.close();
}