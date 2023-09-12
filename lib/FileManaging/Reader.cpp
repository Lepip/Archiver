#include <cassert>
#include <iostream>
#include "Reader.h"
#include "../exceptions.h"

Reader::Reader(std::ifstream&& input, size_t byte_size) {
    this->input_ = std::move(input);
    if (!this->input_.is_open()) {
        throw COULD_NOT_READ();
    }
    input_.clear();
    input.seekg(0);
    std::streampos beginning = input.tellg();
    input_.seekg(0, input_.end);
    file_size_ = input_.tellg() - beginning;
    input_.clear();
    input_.seekg(0);
    this->byte_size_ = byte_size;
    buffer_ = 0;
    buffer_size_ = 0;
    bit_number_ = 0;
    if (byte_size == 0 || byte_size > 32) {
        std::cerr << "Да ты чо";
    }
}

std::vector<bool> Reader::Read(size_t bits_to_read) {
    if (bits_to_read == 0) {
        return {};
    }
    if (bits_to_read > 16) {
        return NoBufferRead(bits_to_read);
    }
    bit_number_ += bits_to_read;
    if (buffer_size_ < bits_to_read) {
        ExtendBuffer(bits_to_read);
    }
    std::vector<bool> bits;
    bool bit;
    for (size_t i = 0; i < bits_to_read; ++i) {
        bit = (buffer_ & (1 << (buffer_size_ - i - 1))) >> (buffer_size_ - i - 1);
        bits.push_back(bit);
    }
    buffer_size_ -= bits_to_read;
    return bits;
}

bool Reader::GetOneBit() {
    if (buffer_size_ == 0) {
        ExtendBuffer(1);
    }
    bool bit = (buffer_ & (1 << (buffer_size_ - 1))) > 0;
    --buffer_size_;
    return bit;
}

void Reader::ExtendBuffer(size_t bits_needed) {
    unsigned char byte;
    while (buffer_size_ < bits_needed) {
        byte = input_.get();
        if (input_.gcount() == 0) {
            throw COULD_NOT_READ();
        }
        buffer_ <<= byte_size_;
        buffer_ |= byte;
        buffer_size_ += byte_size_;
    }
}
std::vector<bool> Reader::NoBufferRead(size_t bits_to_read) {
    std::vector<bool> bits;
    bits.reserve(bits_to_read);
    unsigned char byte;
    for (size_t bit = 0; bit < bits_to_read; ++bit) {
        if (buffer_size_ == 0) {
            byte = input_.get();
            if (input_.gcount() == 0) {
                throw COULD_NOT_READ();
            }
            buffer_ <<= byte_size_;
            buffer_ |= byte;
            buffer_size_ += byte_size_;
        }
        bits.push_back(GetOneBit());
    }
    bit_number_ += bits_to_read;
    return bits;
}
void Reader::MoveToBeginning() {
    input_.clear();
    input_.seekg(0);
    bit_number_ = 0;
    buffer_ = 0;
    buffer_size_ = 0;
}
size_t Reader::PositionInFile() {
    return bit_number_;
}
size_t Reader::FileSize() {
    return file_size_;
}
