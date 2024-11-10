// Huffman Tree Header

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H



#include <string>
#include <unordered_map>

#include "HuffmanNode.h"

class HuffmanTree {
public:
    explicit HuffmanTree(HuffmanNode* tree);
    void generateEncodingTable();
private:
    HuffmanNode* root{nullptr};
    std::unordered_map<std::optional<char>, std::string> huffmanEncodingTable{};
    std::string huffmanEncodingString{};

    // helper functions
    void generateEncodingTableHelper(HuffmanNode* root, const std::string& code);
};



#endif // HUFFMAN_TREE_H
