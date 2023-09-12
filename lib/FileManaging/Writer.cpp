#include <iostream>
#include "Writer.h"

Writer::Writer(std::ofstream&& output, size_t byte_size) {
    this->byte_size_ = byte_size;
    this->output_ = std::move(output);
    to_write_size_ = 0;
    to_write_ = 0;
}
void Writer::Write(const std::vector<bool>& bits_to_write) {
    for (bool bit : bits_to_write) {
        Write(bit);
    }
}

void Writer::WriteByte() {
    output_.put(*reinterpret_cast<char*>(&to_write_));
    to_write_size_ = 0;
}

void Writer::Write(bool bit) {
    to_write_ <<= 1;
    to_write_ |= bit;
    ++to_write_size_;
    if (to_write_size_ == byte_size_) {
        WriteByte();
    }
}

Writer::~Writer() {
    if (to_write_size_ > 0) {
        to_write_ <<= byte_size_ - to_write_size_;
        WriteByte();
    }
    output_.close();
}
void Writer::ExtrenalTermination() {
    if (to_write_size_ > 0) {
        to_write_ <<= byte_size_ - to_write_size_;
        WriteByte();
    }
    output_.close();
}
void Writer::CompleteLastByte() {
    if (to_write_size_ > 0) {
        to_write_ <<= byte_size_ - to_write_size_;
        WriteByte();
    }
}
