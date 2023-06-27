#include <unistd.h>

#include "Compactor.hpp"


int main(int argc, char *argv[]) {

    int c;
    while ((c = getopt(argc, argv, "c:d:")) != -1) {
        switch (c) {
        case 'c':
            Compactor::compress(optarg);
            break;

        case 'd':
            Compactor::decompress(optarg);
            break;
        
        default:
            break;
        }
    }
    return 0;
}
