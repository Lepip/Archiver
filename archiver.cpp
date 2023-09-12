#include <iostream>
#include "lib/exceptions.h"
#include "lib/Huffman.h"

int main(int args_num, char* args[]) {
    try {
        Huffman huffman;
        if (args_num < 2) {
            throw TOO_FEW_ARGUMENTS();
        }
        if (strcmp(args[1], "-c") == 0) {
            if (args_num < 4) {
                throw TOO_FEW_ARGUMENTS();
            }
            OperationReport report = huffman.Encode(args[2], args + 3, args_num-3);
            std::cout << "Compressed from " << report.num_decoded_bits << " bits to \n                " << report.num_encoded_bits
                      << " bits" << std::endl;
            std::cout << "in " << report.duration << " ms" << std::endl;
            std::cout << "Compression complete!" << std::endl;
            return 0;
        }
        if (strcmp(args[1], "-d") == 0) {
            if (args_num > 3) {
                std::string user_ans;
                std::cout << "WARNING: Too many arguments. Continue? (compare/N)" << std::endl;
                std::cin >> user_ans;
                if (user_ans.size() != 1) {
                    throw WRONG_ARGUMENT();
                }
                if (std::tolower(user_ans[0]) == 'y') {
                } else {
                    if (std::tolower(user_ans[0]) == 'n') {
                        throw TERMINATED_BY_USER();
                    } else {
                        throw WRONG_ARGUMENT();
                    }
                }
            }
            OperationReport report = huffman.Decode(args[2]);
            std::cout << "Decompressed from " << report.num_encoded_bits << " bits to \n                  " << report.num_decoded_bits
                      << " bits" << std::endl;
            std::cout << "in " << report.duration << " ms" << std::endl;
            std::cout << "Decompressing complete!" << std::endl;
            return 0;
        }
        if (strcmp(args[1], "-h") == 0) {
            std::cout << "* `archiver -c archive_name file1 [file2 ...]` - compress files `fil1, file2, ...` and save the result into the file `archive_name`.\n"
                         "* `archiver -d archive_name` - decompress files from file `archive_name` and all of the files will maintain their paths.\n"
                         "* `archiver -h` - help." << std::endl;
            return 0;
        }
        throw WRONG_ARGUMENT();
    } catch (std::exception &e) {
        std::cerr << "STOPPED WITH ERROR: " << e.what() << std::endl;
        return 1;
    }
}
