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

    // file-related data members
    FileInformation fileInformation{"", "", 0, ""};

    // compress helper function
    static void writeSection(std::ofstream& output, const std::string& section);

    // decompress helper functions
    static void readSection(std::ifstream& input, std::string& section, uint32_t size);
    void instantiateHuffmanFileInformation();
    static HuffmanNode* instantiateHuffmanTree(const std::string& representation, int& position);

    void writeDecompressedFile(const std::string& path);
};


#endif // HUFFMAN_TREE_H
