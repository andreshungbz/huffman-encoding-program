// Huffman Tree Header

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include <string>
#include <unordered_map>

#include "HuffmanNode.h"
#include "hash_map/FrequencyHashMap.h"
#include "priority_queue/PriorityQueue.h"
#include "FileInformation.h"

class HuffmanTree {
public:
    explicit HuffmanTree(const std::string& path);
    void generateEncodingTable();
    void generateEncodingString(std::ifstream& input);
    void generateHuffmanTreeRepresentation();
    void generateFileInfoEncoding();

private:
    HuffmanNode* huffmanTreeRoot{nullptr};
    std::unordered_map<std::optional<char>, std::string> huffmanEncodingTable{};
    std::string huffmanEncodingString{};
    std::string huffmanTreeRepresentation{};
    std::string huffmanFileInfoEncoding{};

    // file-related data members
    FileInformation fileInformation;

    // helper functions
    static void traverseBST(PriorityQueue& queue, const FrequencyHashNode* root);
    static void populateQueue(PriorityQueue& queue, const FrequencyHashMap& hashMap);
    void generateEncodingTableHelper(const HuffmanNode* root, const std::string& code);
    void insertEncodedCharacter(char character);
    void generateHuffmanTreeRepresentationHelper(const HuffmanNode* root);
};


#endif // HUFFMAN_TREE_H
