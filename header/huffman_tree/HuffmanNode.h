// Huffman Node Header and Implementation

// The HuffmanNode object is used to store the keys and their respective weights. It is used for both building
// the priority queue and the Huffman Tree. To account for non-leaf nodes in the Huffman Tree, which do not
// hold valid key values, std::optional<char> type is used as it offers a convenient way of explicitly
// determining invalid values over '\0' or nullptr. A key with value std::nullopt indicates a non-leaf node.

// https://en.cppreference.com/w/cpp/utility/optional

#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H



#include <optional>

class HuffmanNode {
public:
    std::optional<char> key{std::nullopt};
    int weight{};
    HuffmanNode* left{nullptr};
    HuffmanNode* right{nullptr};
    // constructors
    HuffmanNode(char keyValue, int weightValue) : key(keyValue), weight(weightValue) {} // for priority queue
    explicit HuffmanNode(int weightValue) : weight(weightValue) {} // for building Huffman Tree
};



#endif // HUFFMAN_NODE_H
