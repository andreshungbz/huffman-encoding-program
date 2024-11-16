// Huffman Tree Header

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include <string>
#include <unordered_map>

#include "HuffmanNode.h"
#include "huffman_tree/components/FileInformation.h"
#include "huffman_tree/components/HuffmanHeader.h"

class HuffmanTree {
public:
    explicit HuffmanTree(const std::string& source);
    HuffmanTree() = default;

    void generate(std::ifstream& input, const std::string& source);
    void compress() const;
    void decompress(const std::string& source);
    void instantiate();

private:
    HuffmanNode* huffmanTreeRoot{nullptr};
    FileInformation fileInformation{"", "", 0, ""};
    std::unordered_map<std::optional<char>, std::string> encodingTable{};

    // data members which are written and read to file
    HuffmanHeader fileHeader{0, 0, 0};
    std::string huffmanFileInfoCode{};
    std::string huffmanTreeRepresentation{};
    std::string huffmanCode{};
};


#endif // HUFFMAN_TREE_H
