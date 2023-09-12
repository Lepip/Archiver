#include <iostream>
#include "Hasher.h"

void Hasher::AddBits(const std::vector<bool>& bits) {
    for (bool bit : bits) {
        if (bit) {
            hash_ *= osn_;
            hash_ %= mod_;
            hash_ += bit_number_ % mod_;
        }
        ++bit_number_;
    }
}

std::vector<bool> Hasher::GetHash() {
    std::vector<bool> hash;
    for (size_t i = 0; i < 32; i++) {
        hash.push_back(hash_ & (1 << (31 - i)));
    }
    return hash;
}

Hasher::Hasher(int64_t mod, int64_t osn) {
    hash_ = 1;
    mod_ = mod;
    osn_ = osn;
    bit_number_ = 1;
}
