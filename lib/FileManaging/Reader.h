#pragma once

#include <fstream>
#include <bitset>
#include <vector>

class Reader {
public:
    Reader(std::ifstream&& input, size_t byte_size = 8);
    std::vector<bool> Read(size_t bits_to_read);
    bool GetOneBit();
    ~Reader() {
        input_.close();
    }
    void MoveToBeginning();
    size_t PositionInFile();
    size_t FileSize();

private:
    std::vector<bool> NoBufferRead(size_t bits_to_read);
    void ExtendBuffer(size_t bits_needed);
    std::ifstream input_;
    size_t byte_size_;
    int32_t buffer_;
    int8_t buffer_size_;
    size_t bit_number_;
    size_t file_size_;
};
