// Huffman Node Header and Implementation

#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H



#include <optional>

class HuffmanNode {
public:
    std::optional<char> key{std::nullopt};
    int weight{};
    HuffmanNode* left{nullptr};
    HuffmanNode* right{nullptr};
    HuffmanNode(char keyValue, int weightValue) : key(keyValue), weight(weightValue) {}
    explicit HuffmanNode(int weightValue) : weight(weightValue) {}
};



#endif // HUFFMAN_NODE_H
