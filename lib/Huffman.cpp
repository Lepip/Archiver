#include <chrono>
#include <map>
#include <iostream>

#include "Huffman.h"
#include "FileManaging/Hasher.h"
#include "Structs/HuffmanBor.h"

OperationReport Huffman::Decode(char* archive_name) {
    std::ifstream archive_stream(archive_name);
    Reader archive(std::move(archive_stream));
    if (!IsHashOK(archive)) {
        throw COULD_NOT_READ();
    }
    OperationReport report;
    auto started =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    report.num_encoded_bits = archive.FileSize() * 8;
    report.num_decoded_bits = 0;
    bool is_running = true;
    std::vector<bool> bits;
    size_t num_characters;
    size_t left;
    std::vector<std::vector<bool> > characters_bits;
    std::vector<size_t> num_representations_with_length;
    HuffmanBor bor;
    while (is_running) {
        characters_bits.clear();
        num_representations_with_length.clear();
        bits = archive.Read(9);
        num_characters = Converter(bits);
        for (size_t i = 0; i < num_characters; ++i) {
            characters_bits.push_back(archive.Read(9));
        }
        left = num_characters;
        while (left > 0) {
            bits = archive.Read(9);
            num_representations_with_length.push_back(Converter(bits));
            left -= num_representations_with_length.back();
        }
        bor.Rebuild(characters_bits, num_representations_with_length);
        std::string file_name;
        while (true) {
            bits.clear();
            while (bor.GetCharacter(bits).empty()) {
                bits.push_back(archive.GetOneBit());
            }
            bits = bor.GetCharacter(bits);
            if (bits != FILENAME_END) {
                file_name.push_back(Converter(bits));
            } else {
                break;
            }
        }
        std::ofstream file(file_name);
        if (!file.is_open()) {
            throw COULD_NOT_WRITE();
        }
        while (true) {
            bits.clear();
            while (bor.GetCharacter(bits).empty()) {
                bits.push_back(archive.GetOneBit());
            }
            bits = bor.GetCharacter(bits);
            if (bits[0] != 1) {
                file.put(Converter(bits));
                report.num_decoded_bits += 8;
            } else {
                file.close();
                if (bits == ARCHIVE_END) {
                    is_running = false;
                }
                break;
            }
        }
    }
    auto ended =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    report.duration = ended - started;
    return report;
}

OperationReport Huffman::Encode(char* archive_name, char** file_names, size_t file_num) {
    OperationReport report = {0, 0, 0};
    auto started =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    std::ofstream archive_stream(archive_name);
    if (!archive_stream.is_open()) {
        throw COULD_NOT_WRITE();
    }
    Writer writer(std::move(archive_stream));
    Hasher hasher;
    for (size_t i = 0; i < file_num; ++i) {
        report += Encode(file_names[i], writer, hasher, (i + 1 == file_num));
    }
    writer.CompleteLastByte();
    writer.Write(hasher.GetHash());
    auto ended =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    report.duration = ended - started;
    return report;
}

bool Huffman::IsHashOK(Reader& input) {
    Hasher hasher;
    input.MoveToBeginning();
    size_t file_size = input.FileSize();
    if (file_size < 6) {
        throw COULD_NOT_READ();
    }
    std::vector<bool> bits;
    for (size_t byte = 0; byte < file_size - 5; ++byte) {
        bits = input.Read(8);
        hasher.AddBits(bits);
    }
    bits = hasher.GetHash();
    if (bits != input.Read(32)) {
        input.MoveToBeginning();
        return false;
    }
    input.MoveToBeginning();
    return true;
}

OperationReport Huffman::Encode(char* file_name, Writer& writer, Hasher& hasher, bool last_file) {
    OperationReport report = {0, 0, 0};
    HuffmanBor bor;
    std::ifstream input_file_stream(file_name);

    if (!input_file_stream.is_open()) {
        throw COULD_NOT_READ("one of files to encode didn't open");
    }

    std::string file_name_str = file_name;
    for (char ch : file_name_str) {
        bor.AddCharacter(ch);
    }
    size_t filesize = 0;
    char ch;
    while (true) {
        ch = input_file_stream.get();
        if (input_file_stream.gcount() == 0) {
            break;
        }
        bor.AddCharacter(ch);
        ++filesize;
    }
    report.num_decoded_bits = filesize * 8;
    bor.AddCharacter(ONE_MORE_FILE);
    bor.AddCharacter(FILENAME_END);
    bor.AddCharacter(ARCHIVE_END);
    bor.Rebuild();
    input_file_stream.clear();
    input_file_stream.seekg(0);
    std::vector<bool> bits;
    bits = Converter(bor.GetAlphabetSize(), 9);
    hasher.AddBits(bits);
    writer.Write(bits);
    report.num_encoded_bits += 9;
    std::vector<HuffmanBor::Character> alphabet = bor.GetAlphabet();
    std::map<size_t, size_t> num_characters_of_length;
    for (HuffmanBor::Character& character : alphabet) {
        bits = Converter(0, 9 - character.character_bits.size());
        hasher.AddBits(bits);
        hasher.AddBits(character.character_bits);
        writer.Write(bits);
        writer.Write(character.character_bits);
        report.num_encoded_bits += 9;
        ++num_characters_of_length[character.huffman_code.size()];
    }
    size_t prev_length = 0;
    for (auto [length, num] : num_characters_of_length) {
        bits = Converter(0, 9);
        while (prev_length + 1 < length) {
            hasher.AddBits(bits);
            writer.Write(bits);
            report.num_encoded_bits += 9;
            ++prev_length;
        }
        bits = Converter(num, 9);
        hasher.AddBits(bits);
        writer.Write(bits);
        report.num_encoded_bits += 9;
        prev_length = length;
    }
    for (char ch : file_name_str) {
        bits = bor.GetRepresentation(ch);
        hasher.AddBits(bits);
        writer.Write(bits);
        report.num_encoded_bits += bits.size();
    }
    bits = bor.GetRepresentation(FILENAME_END);
    hasher.AddBits(bits);
    writer.Write(bits);
    report.num_encoded_bits += bits.size();
    while (true) {
        ch = input_file_stream.get();
        if (input_file_stream.gcount() == 0) {
            break;
        }
        bits = bor.GetRepresentation(ch);
        hasher.AddBits(bits);
        writer.Write(bits);
        report.num_encoded_bits += bits.size();
    }
    if (last_file) {
        bits = bor.GetRepresentation(ARCHIVE_END);
        hasher.AddBits(bits);
        writer.Write(bits);
        report.num_encoded_bits += bits.size();
    } else {
        bits = bor.GetRepresentation(ONE_MORE_FILE);
        hasher.AddBits(bits);
        writer.Write(bits);
        report.num_encoded_bits += bits.size();
    }
    return report;
}
std::vector<bool> Huffman::Converter(int64_t code, size_t bits_size) {
    std::vector<bool> bits(bits_size);
    for (size_t i = 0; i < bits_size; i++) {
        bits[bits_size - i - 1] = code & 1;
        code >>= 1;
    }
    return bits;
}
int64_t Huffman::Converter(std::vector<bool>& bits) {
    int64_t ans = 0;
    for (bool bit : bits) {
        ans <<= 1;
        ans |= bit;
    }
    return ans;
}
