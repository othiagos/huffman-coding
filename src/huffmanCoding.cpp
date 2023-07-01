#include <unistd.h>

#include "Compactor.hpp"

int main(int argc, char *argv[]) {

    if (argc < 4)
        throw "Without arguments";

    string flag_1 = argv[1];
    string file_input_1 = argv[2];
    string file_output_1 = argv[3];

    string flag_2;
    string file_input_2;
    string file_output_2;

    if (argc > 6) {
        flag_2 = argv[4];
        file_input_2 = argv[5];
        file_output_2 = argv[6];
    }  

    if (flag_1 == "-c") {
        Compactor::compress(file_input_1, file_output_1);
    }
    else if (flag_1 == "-d") {
        Compactor::decompress(file_input_1, file_output_1);
    }

    if (argc > 6 && flag_2 == "-c") {
        Compactor::compress(file_input_2, file_output_2);
    }
    else if (argc > 6 && flag_2 == "-d") {
        Compactor::decompress(file_input_2, file_output_2);
    }

    return 0;
}
