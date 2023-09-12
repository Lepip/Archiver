#pragma once

#include <fstream>
#include <vector>
#include <bitset>

class Writer {
public:
    Writer(std::ofstream&& output, size_t byte_size = 8);
    void Write(const std::vector<bool>& bits_to_write);
    void Write(bool bit);
    void ExtrenalTermination();
    void CompleteLastByte();
    ~Writer();

private:
    size_t byte_size_;
    unsigned char to_write_;
    size_t to_write_size_;
    std::ofstream output_;
    void WriteByte();
};
