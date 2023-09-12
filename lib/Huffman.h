#pragma once

#include <string_view>
#include "FileManaging/FileManagers.h"
#include "exceptions.h"

struct OperationReport {
    size_t num_decoded_bits;
    size_t num_encoded_bits;
    time_t duration;
    void operator+=(OperationReport a) {
        num_encoded_bits += a.num_encoded_bits;
        num_decoded_bits += a.num_decoded_bits;
    }
};

class Huffman {
public:
    const std::vector<bool> FILENAME_END = {1, 0, 0, 0, 0, 0, 0, 0, 0};
    const std::vector<bool> ONE_MORE_FILE = {1, 0, 0, 0, 0, 0, 0, 0, 1};
    const std::vector<bool> ARCHIVE_END = {1, 0, 0, 0, 0, 0, 0, 1, 0};
    OperationReport Decode(char* archive_name);
    OperationReport Encode(char* archive_name, char* file_names[], size_t file_num);

private:
    OperationReport Encode(char* file_name, Writer& writer, Hasher& hasher, bool last_file);
    bool IsHashOK(Reader& input);
    std::vector<bool> Converter(int64_t code, size_t bits);
    int64_t Converter(std::vector<bool>& bits);
};