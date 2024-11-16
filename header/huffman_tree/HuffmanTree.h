// Huffman Tree Header

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include <string>
#include <unordered_map>

#include "HuffmanNode.h"
#include "hash_map/FrequencyHashMap.h"
#include "priority_queue/PriorityQueue.h"
#include "FileInformation.h"
#include "HuffmanFileHeader.h"

class HuffmanTree {
public:
    explicit HuffmanTree(const std::string& path);
    HuffmanTree() = default;
    void generateEncodingTable();
    void generateEncodingString(std::ifstream& input);
    void generateHuffmanTreeRepresentation();
    void generateFileInfoEncoding();
    void generateHuffmanFileHeader();

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

    // helper functions
    static void traverseBST(PriorityQueue& queue, const FrequencyHashNode* root);
    static void populateQueue(PriorityQueue& queue, const FrequencyHashMap& hashMap);
    void generateEncodingTableHelper(const HuffmanNode* root, const std::string& code);
    void insertEncodedCharacter(char character);
    void generateHuffmanTreeRepresentationHelper(const HuffmanNode* root);

    // compress helper function
    static void writeSection(std::ofstream& output, const std::string& section);

    // decompress helper functions
    void readHuffmanFileInfo(std::ifstream& input);
    void readHuffmanTreeRepresentation(std::ifstream& input);
    void readHuffmanEncoding(std::ifstream& input);
    static HuffmanNode* reconstructHuffmanTree(const std::string& representation, int& position);
    void writeDecompressedFile(const std::string& path);
};


#endif // HUFFMAN_TREE_H
