// Huffman Tree Header

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include <string>
#include <unordered_map>

#include "HuffmanNode.h"
#include "hash_map/FrequencyHashMap.h"
#include "FileInformation.h"
#include "HuffmanFileHeader.h"

class HuffmanTree {
public:
    explicit HuffmanTree(const std::string& path);
    HuffmanTree() = default;

    void generate(std::ifstream& input, const std::string& path);
    void compress() const;
    void decompress(const std::string& path);

private:
    HuffmanNode* huffmanTreeRoot{nullptr};
    std::unordered_map<std::optional<char>, std::string> huffmanEncodingTable{};
    std::string huffmanEncodingString{};
    std::string huffmanTreeRepresentation{};
    std::string huffmanFileInfoEncoding{};
    HuffmanFileHeader huffmanFileHeader{0, 0, 0};
    FileInformation fileInformation{"", "", 0, ""};
};


#endif // HUFFMAN_TREE_H
