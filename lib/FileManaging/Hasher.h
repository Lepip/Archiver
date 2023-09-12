#pragma once

#include <vector>
#include <cstdint>
#include <cstdlib>
class Hasher {
public:
    Hasher(int64_t mod = 1000000007, int64_t osn = 180071);
    void AddBits(const std::vector<bool> &bits);
    std::vector<bool> GetHash();

private:
    u_int64_t hash_;
    u_int64_t mod_;
    u_int64_t osn_;
    size_t bit_number_;
};
