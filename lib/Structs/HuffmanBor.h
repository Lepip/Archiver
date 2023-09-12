#pragma once

#include <vector>
#include <unordered_map>
class HuffmanBor {
public:
    HuffmanBor();
    using RepresentationBits = std::vector<bool>;
    using CharacterBits = std::vector<bool>;
    RepresentationBits GetRepresentation(const CharacterBits &character);
    RepresentationBits GetRepresentation(char character);
    void AddCharacter(const std::vector<bool> &character);
    void AddCharacter(char ch);
    CharacterBits GetBitsFromChar(char ch);
    CharacterBits GetCharacter(const RepresentationBits &representation);
    void Rebuild();
    void Rebuild(std::vector<std::vector<bool> > characters, std::vector<size_t> num_with_length);
    size_t GetAlphabetSize();
    using CharachterPos = size_t;
    struct Character {
        CharacterBits character_bits;
        size_t entry_num = 0;
        RepresentationBits huffman_code;
        bool operator<(const Character &a) const {
            return entry_num < a.entry_num;
        }
    };
    std::vector<Character> GetAlphabet();
    class Comparator;
    struct BorNode {
        bool is_leaf = false;
        Character *character = nullptr;
        size_t priority = 0;
        BorNode *L = nullptr;
        BorNode *R = nullptr;
    };

private:
    std::unordered_map<CharacterBits, CharachterPos> characters_map_;
    std::vector<Character> characters_;
    std::unordered_map<RepresentationBits, CharachterPos> canonical_map_;
    size_t num_of_characters_;
    void Dearchive(BorNode *a);
    void AddOne(RepresentationBits &bits);
};
