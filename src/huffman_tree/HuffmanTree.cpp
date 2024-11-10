#include <filesystem>
#include <fstream>
#include <iostream>

#include "huffman_tree/HuffmanTree.h"

HuffmanTree::HuffmanTree(const std::string& path) : fileInformation("", "", 0) {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{path, std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "File Open Error\n";
        return;
    }

    // populate file-related information
    std::filesystem::path filePath{path};
    std::string directory{canonical(filePath.parent_path()).string()};
    std::string fileName{filePath.stem().string()};
    std::string fileExtension{filePath.extension().string()};
    std::size_t fileSize{std::filesystem::file_size(filePath)};
    // initialize members
    fileDirectory = directory;
    fileInformation = FileInformation(fileName, fileExtension, fileSize);

    // create frequency hash map with specified number of buckets
    FrequencyHashMap hashMap{10};
    // read every character and insert into frequency hash map
    char character;
    while (input.get(character)) {
        // .get() evaluates false after reading invalid character
        hashMap.insertHashNode(character);
    }

    // create priority queue
    PriorityQueue queue{};
    // populate queue with every key in hash map
    populateQueue(queue, hashMap);

    // construct Huffman Tree
    queue.constructHuffmanTree();
    // assign constructed Huffman Tree to this object's root
    root = queue.getHuffmanTree();

    // generate other structures

    generateEncodingTable();
}

void HuffmanTree::generateEncodingTable() {
    // clear encoding table
    huffmanEncodingTable.clear();

    generateEncodingTableHelper(root, "");
}

// helper functions

void HuffmanTree::traverseBST(PriorityQueue& queue, const FrequencyHashNode* root) {
    // base case
    if (root == nullptr) {
        return;
    }

    // traverse inorder, going through every node and populating the queue
    queue.enqueue(root->key, root->frequency);
    traverseBST(queue, root->left);
    traverseBST(queue, root->right);
}

void HuffmanTree::populateQueue(PriorityQueue& queue, const FrequencyHashMap& hashMap) {
    for (FrequencyHashNode* tree : hashMap.buckets) {
        traverseBST(queue, tree);
    }
}

void HuffmanTree::generateEncodingTableHelper(const HuffmanNode* root, const std::string& code) {
    // base case: past leaf node nullptr
    if (root == nullptr) {
        return;
    }

    // add encoding for only leaf nodes
    if (root->left == nullptr && root->right == nullptr) {
        // special case: tree with only one node
        if (code.empty()) {
            huffmanEncodingTable[root->key] = "0";
        } else {
            huffmanEncodingTable[root->key] = code;
        }

        return; // short-circuit on successful addition
    }

    // for non-leaf nodes, recursively continue
    // here is where the Huffman Coding algorithm comes into play with 0 going left and 1 going right
    generateEncodingTableHelper(root->left, code + "0");
    generateEncodingTableHelper(root->right, code + "1");
}
