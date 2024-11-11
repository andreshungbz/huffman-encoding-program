// #include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "huffman_tree/HuffmanTree.h"

// POSIX functions

// function to get file size
std::size_t getFileSize(const std::string& path) {
    struct stat statBuf{};
    if (stat(path.c_str(), &statBuf) != 0) {
        return 0;
    }
    return statBuf.st_size;
}

// function to get directory from path
std::string getDirectory(const std::string& path) {
    char absPath[PATH_MAX];
    if (realpath(path.c_str(), absPath) == nullptr) {
        return "";
    }
    std::string fullPath(absPath);
    size_t pos = fullPath.find_last_of("/\\");
    return (std::string::npos == pos) ? "" : fullPath.substr(0, pos);
}

// function to get file name without extension
std::string getFileName(const std::string& path) {
    size_t start = path.find_last_of("/\\") + 1;
    size_t end = path.find_last_of('.');
    return path.substr(start, end - start);
}

// function to get file extension
std::string getFileExtension(const std::string& path) {
    size_t pos = path.find_last_of('.');
    return (std::string::npos == pos) ? "" : path.substr(pos);
}

HuffmanTree::HuffmanTree(const std::string& path) : fileInformation("", "", 0) {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{path, std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "File Open Error\n";
        return;
    }

    // populate file-related information using filesystem
    // std::filesystem::path filePath{path};
    // std::string directory{canonical(filePath.parent_path()).string()};
    // std::string fileName{filePath.stem().string()};
    // std::string fileExtension{filePath.extension().string()};
    // std::size_t fileSize{std::filesystem::file_size(filePath)};

    // populate file-related information using POSIX functions
    std::string directory = getDirectory(path);
    std::string fileName = getFileName(path);
    std::string fileExtension = getFileExtension(path);
    std::size_t fileSize = getFileSize(path);

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
    generateEncodingString(input);

    // close file
    input.close();
}

void HuffmanTree::generateEncodingTable() {
    // clear encoding table
    huffmanEncodingTable.clear();

    generateEncodingTableHelper(root, "");
}

void HuffmanTree::generateEncodingString(std::ifstream& input) {
    // clear string and move file pointer back to beginning
    huffmanEncodingString.clear();
    input.clear(); // check if error state
    input.seekg(0, std::ios::beg);

    char character;
    while(input.get(character)) {
        insertEncodedCharacter(character);
    }
}

void HuffmanTree::generateHuffmanTreeRepresentation() {

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

void HuffmanTree::insertEncodedCharacter(const char character) {
    auto encoding{huffmanEncodingTable.find(character)};

    if (encoding != huffmanEncodingTable.end()) {
        huffmanEncodingString += encoding->second;
    } else {
        std::cout << "Character not found in encoding table.\n";
    }
}
