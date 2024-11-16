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
    // constructors
    explicit HuffmanTree(const std::string& source); // used when creating an instance from which to compress
    HuffmanTree() = default; // default constructor used when creating an empty object from which to decompress

    // main functions, they are in order of the program loop
    void generate(std::ifstream& input, const std::string& source);
    void compress() const;
    void decompress(const std::string& source);
    void instantiate();

private:
    // instantiated data members
    HuffmanNode* huffmanTreeRoot{nullptr};
    FileInformation fileInformation{"", "", 0, ""};

    // used for compressing, empty when decompressed
    std::unordered_map<std::optional<char>, std::string> encodingTable{};

    // data members which are written and read to file
    HuffmanHeader huffmanHeader{0, 0, 0};
    std::string huffmanFileInfoCode{};
    std::string huffmanTreeRepresentation{};
    std::string huffmanCode{};
};


#endif // HUFFMAN_TREE_H
