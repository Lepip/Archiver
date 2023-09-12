#include <sstream>
#include <iostream>
#include "../../contrib/catch/catch.hpp"
#include "lib/FileManaging/FileManagers.h"
#include "lib/Structs/PriorityQueue.h"
#include "lib/Structs/HuffmanBor.h"
#include "lib/Huffman.h"

TEST_CASE("Test FileManaging") {
    auto started =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    auto ended =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    std::ofstream ss("test.txt");
    Writer w(std::move(ss));
    std::vector<bool> test = {0, 1, 1, 0, 1, 0, 0, 0, 1, 0};
    w.Write(test);
    w.ExtrenalTermination();
    std::ifstream sas("test.txt");
    Reader r(std::move(sas));
    std::vector<bool> v = r.Read(test.size());
    REQUIRE(v == test);
    test.clear();
    for (size_t i = 0; i < 1000009; i++) {
        test.push_back(rand() % 2);
    }
    started = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                  .count();
    std::ofstream ss2("test2.txt");
    Writer w2(std::move(ss2));
    w2.Write(test);
    w2.ExtrenalTermination();
    ended = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                .count();
    std::cout << ended - started << " ms" << std::endl;
    started = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                  .count();
    std::ifstream sas2("test2.txt");
    Reader r2(std::move(sas2));
    v = r2.Read(test.size());
    REQUIRE(v == test);
    ended = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                .count();
    std::cout << ended - started << " ms" << std::endl;

    std::ofstream ss3("test3.txt");
    Writer w3(std::move(ss3));
    test = {0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    w3.Write(std::vector(test.begin(), test.begin() + 5));
    w3.Write(std::vector(test.begin() + 5, test.begin() + 10));
    w3.Write(std::vector(test.begin() + 10, test.begin() + 13));
    w3.ExtrenalTermination();
    std::ifstream in3("test3.txt");
    Reader r3(std::move(in3));
    REQUIRE(r3.FileSize() == 3);
    REQUIRE(r3.Read(3) == std::vector(test.begin(), test.begin() + 3));
    REQUIRE(r3.Read(5) == std::vector(test.begin() + 3, test.begin() + 8));
    REQUIRE(r3.Read(5) == std::vector(test.begin() + 8, test.begin() + 13));
}

TEST_CASE("Test Queue") {
    PriorityQueue<int> pq;
    pq.Push(5);
    pq.Push(5);
    pq.Push(5);
    pq.Push(5);
    pq.Push(5);
    pq.Push(5);
    pq.Push(5);
    pq.Push(4);
    pq.Push(2);
    pq.Push(5);
    REQUIRE(pq.Size() == 10);
    REQUIRE(pq.Top() == 2);
    std::vector<int> v;
    while (!pq.Empty()) {
        v.push_back(pq.Top());
        pq.Pop();
    }
    REQUIRE(v == std::vector({2, 4, 5, 5, 5, 5, 5, 5, 5, 5}));
    pq.Push(1);
    REQUIRE(pq.Top() == 1);
}

TEST_CASE("Test Bor") {
    HuffmanBor bor;
    std::vector<bool> bits = bor.GetBitsFromChar('a');
    bor.AddCharacter(bits);
    bor.AddCharacter(bits);
    bor.AddCharacter(bits);
    bor.AddCharacter(bits);
    bits = bor.GetBitsFromChar('d');
    bor.AddCharacter(bits);
    bor.AddCharacter(bits);
    bits = bor.GetBitsFromChar('c');
    bor.AddCharacter(bits);
    bits = bor.GetBitsFromChar('z');
    bor.AddCharacter(bits);
    bor.AddCharacter(bits);
    bor.Rebuild();
    REQUIRE(bor.GetRepresentation(bor.GetBitsFromChar('d')) == std::vector<bool>({1, 0}));
    REQUIRE(bor.GetRepresentation(bor.GetBitsFromChar('c')) == std::vector<bool>({1, 1, 0}));
    REQUIRE(bor.GetRepresentation(bor.GetBitsFromChar('z')) == std::vector<bool>({1, 1, 1}));
    REQUIRE(bor.GetRepresentation(bor.GetBitsFromChar('a')) == std::vector<bool>({0}));
}

TEST_CASE("Test Huffman") {
    std::ofstream test1("output1.txt");
    std::ofstream test2("output2");
    std::ofstream test3("output3.txt");
    std::ofstream test4("output4.pdf");
    std::string test1_, test2_, test3_, test4_;
    test1_ = "abacaba";
    for (size_t i = 0; i < 1000000; i++) {
        test2_.push_back(1);
    }
    for (size_t i = 0; i < 1000000; i++) {
        test3_.push_back(rand()%256);
    }
    test4_ = "azwsxerdcftvgybhunjmk,l.;/-32284978790";
    Huffman huff;
    test1 << test1_;
    test2 << test2_;
    test3 << test3_;
    test4 << test4_;
    test1.close();
    test2.close();
    test3.close();
    test4.close();
    char** a = nullptr;
    std::string arch_0 = "arch";
    char* arch = arch_0.data();
    std::string a_0 = "output1.txt";
    std::string a_1 = "output2";
    std::string a_2 = "output3.txt";
    std::string a_3 = "output4.pdf";
    a = static_cast<char**>(malloc(a_0.size() + a_1.size() + a_2.size() + a_3.size() + 10));
    a[0] = a_0.data();
    a[1] = a_1.data();
    a[2] = a_2.data();
    a[3] = a_3.data();
    huff.Encode(arch, a, 4);
    huff.Decode(arch);
    std::ifstream test1_i("output1.txt");
    std::ifstream test2_i("output2");
    std::ifstream test3_i("output3.txt");
    std::ifstream test4_i("output4.pdf");
    std::string test1_i_;
    char ch = test1_i.get();
    while(test1_i.gcount() == 1) {
        test1_i_.push_back(ch);
        ch = test1_i.get();
    }
    REQUIRE(test1_i_ == test1_);
    std::string test2_i_;
    ch = test2_i.get();
    while(test2_i.gcount() == 1) {
        test2_i_.push_back(ch);
        ch = test2_i.get();
    }
    REQUIRE(test2_i_ == test2_);
    std::string test3_i_;
    ch = test3_i.get();
    while(test3_i.gcount() == 1) {
        test3_i_.push_back(ch);
        ch = test3_i.get();
    }
    REQUIRE(test3_i_ == test3_);
    std::string test4_i_;
    ch = test4_i.get();
    while(test4_i.gcount() == 1) {
        test4_i_.push_back(ch);
        ch = test4_i.get();
    }
    REQUIRE(test4_i_ == test4_);

}