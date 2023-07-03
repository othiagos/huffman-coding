#include <unistd.h>

#include "Compactor.hpp"

int main(int argc, char *argv[]) {

    try {
        if (argc < 4)
            throw compexcp::WithoutArguments(argc);

        if (argc > 4 && argc < 7)
            throw compexcp::WithoutArguments(argc);

        if (argc > 7)
            throw compexcp::WithoutArguments(argc);

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
        } else if (flag_1 == "-d") {
            Compactor::decompress(file_input_1, file_output_1);
        }

        if (argc > 6 && flag_2 == "-c") {
            Compactor::compress(file_input_2, file_output_2);
        } else if (argc > 6 && flag_2 == "-d") {
            Compactor::decompress(file_input_2, file_output_2);
        }

    } catch (const compexcp::WithoutArguments &e) {
        std::cerr << e.what() << std::endl;
    } catch (const compexcp::CouldNotOpenFile &e) {
        std::cerr << e.what() << std::endl;
    } catch (const compexcp::FileNotUTF8 &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
