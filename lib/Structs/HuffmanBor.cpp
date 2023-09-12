#include <algorithm>
#include "HuffmanBor.h"
#include "PriorityQueue.h"

void HuffmanBor::AddCharacter(const std::vector<bool>& character) {
    CharachterPos pos = characters_map_[character];
    if (pos == 0) {
        characters_map_[character] = num_of_characters_;
        pos = num_of_characters_;
        num_of_characters_++;
        Character new_character = {character, 0};
        characters_.push_back(new_character);
    }
    characters_[pos].entry_num++;
}

std::vector<bool> HuffmanBor::GetRepresentation(const std::vector<bool>& character) {
    return characters_[characters_map_[character]].huffman_code;
}

std::vector<bool> HuffmanBor::GetCharacter(const std::vector<bool>& representation) {
    return characters_[canonical_map_[representation]].character_bits;
}

class HuffmanBor::Comparator {
public:
    bool operator()(BorNode& a, BorNode& b) {
        return a.priority < b.priority;
    }
};

void HuffmanBor::Dearchive(BorNode* a) {
    static RepresentationBits bits;
    if (a->is_leaf) {
        CharachterPos pos = characters_map_[a->character->character_bits];
        Character& character = characters_[pos];
        character.huffman_code = bits;
        return;
    }
    bits.push_back(0);
    Dearchive(a->L);
    bits.pop_back();
    bits.push_back(1);
    Dearchive(a->R);
    bits.pop_back();
}

void HuffmanBor::AddOne(RepresentationBits& bits) {
    size_t pos;
    for (size_t i = 0; i < bits.size(); ++i) {
        pos = bits.size() - i - 1;
        if (bits[pos] == 0) {
            bits[pos] = 1;
            return;
        } else {
            bits[pos] = 0;
        }
    }
    bits.push_back(0);
    bits[0] = 1;
}

void HuffmanBor::Rebuild() {
    PriorityQueue<BorNode, Comparator> queue;
    BorNode another_node;
    for (size_t i = 1; i < characters_.size(); i++) {
        Character& character = characters_[i];
        another_node = {true, &character, character.entry_num, nullptr, nullptr};
        queue.Push(another_node);
    }
    BorNode node_a, node_b;
    std::vector<BorNode> nodes;
    nodes.reserve(queue.Size() * 2);
    while (queue.Size() > 1) {
        node_a = queue.Top();
        queue.Pop();
        nodes.push_back(node_a);
        node_b = queue.Top();
        queue.Pop();
        nodes.push_back(node_b);
        another_node = {false, nullptr, node_a.priority + node_b.priority, &nodes[nodes.size() - 2],
                        &nodes[nodes.size() - 1]};
        queue.Push(another_node);
    }
    node_a = queue.Top();
    Dearchive(&node_a);
    std::sort(characters_.begin(), characters_.end(), [](Character& a, Character& b) {
        if (a.huffman_code.size() == b.huffman_code.size()) {
            for (size_t i = 0; i < a.huffman_code.size(); ++i) {
                if (a.huffman_code[i] != b.huffman_code[i]) {
                    return a.huffman_code[i] < b.huffman_code[i];
                }
            }
        }
        return a.huffman_code.size() < b.huffman_code.size();
    });
    RepresentationBits canonical({0});
    canonical_map_.clear();
    characters_map_.clear();
    for (size_t i = 1; i < characters_.size(); ++i) {
        while (canonical.size() < characters_[i].huffman_code.size()) {
            canonical.push_back(0);
        }
        characters_[i].huffman_code = canonical;
        canonical_map_[characters_[i].huffman_code] = i;
        characters_map_[characters_[i].character_bits] = i;
        AddOne(canonical);
    }
}

HuffmanBor::HuffmanBor() {
    characters_.push_back({});
    num_of_characters_ = 1;
}

HuffmanBor::CharacterBits HuffmanBor::GetBitsFromChar(char ch) {
    std::vector<bool> bits(8);
    for (size_t i = 0; i < 8; ++i) {
        if (ch & (1 << i)) {
            bits[7 - i] = 1;
        }
    }
    return bits;
}
void HuffmanBor::AddCharacter(char ch) {
    AddCharacter(GetBitsFromChar(ch));
}
size_t HuffmanBor::GetAlphabetSize() {
    return characters_.size() - 1;
}
std::vector<HuffmanBor::Character> HuffmanBor::GetAlphabet() {
    std::vector<HuffmanBor::Character> ans = std::vector<Character>(characters_.begin() + 1, characters_.end());
    return ans;
}
void HuffmanBor::Rebuild(std::vector<std::vector<bool> > characters, std::vector<size_t> num_with_length) {
    characters_.clear();
    characters_.push_back({});
    std::vector<size_t> characters_codes_length;
    for (size_t i = 0; i < num_with_length.size(); ++i) {
        for (size_t j = 0; j < num_with_length[i]; ++j) {
            characters_codes_length.push_back(i + 1);
        }
    }
    for (size_t i = 0; i < characters.size(); ++i) {
        characters_.push_back({characters[i], characters_codes_length[i]});
    }
    RepresentationBits canonical({0});
    canonical_map_.clear();
    characters_map_.clear();
    for (size_t i = 1; i < characters_.size(); ++i) {
        while (canonical.size() < characters_[i].entry_num) {
            canonical.push_back(0);
        }
        characters_[i].huffman_code = canonical;
        canonical_map_[characters_[i].huffman_code] = i;
        characters_map_[characters_[i].character_bits] = i;
        AddOne(canonical);
    }
}
HuffmanBor::RepresentationBits HuffmanBor::GetRepresentation(char character) {
    return GetRepresentation(GetBitsFromChar(character));
}
