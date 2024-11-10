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

private:
    HuffmanNode* root{nullptr};
    std::unordered_map<std::optional<char>, std::string> huffmanEncodingTable{};
    std::string huffmanEncodingString{};

    // file-related data members
    std::string fileDirectory{};
    FileInformation fileInformation;

    // helper functions
    static void traverseBST(PriorityQueue& queue, const FrequencyHashNode* root);
    static void populateQueue(PriorityQueue& queue, const FrequencyHashMap& hashMap);
    void generateEncodingTableHelper(const HuffmanNode* root, const std::string& code);
};


#endif // HUFFMAN_TREE_H
