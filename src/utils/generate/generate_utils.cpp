#include "generate_utils.h"

#include <iostream>

void generateEncodingTable(std::unordered_map<std::optional<char>, std::string>& encodingTable, const HuffmanNode* root) {
    // clear encoding table
    encodingTable.clear();

    generateEncodingTableHelper(encodingTable, root, "");
}

void generateEncodingTableHelper(std::unordered_map<std::optional<char>, std::string>& encodingTable, const HuffmanNode* root, const std::string& code) {
    // base case: past leaf node nullptr
    if (root == nullptr) {
        return;
    }

    // add encoding for only leaf nodes
    if (root->left == nullptr && root->right == nullptr) {
        // special case: tree with only one node
        if (code.empty()) {
            encodingTable[root->key] = "0";
        } else {
            encodingTable[root->key] = code;
        }

        return; // short-circuit on successful addition
    }

    // for non-leaf nodes, recursively continue
    // here is where the Huffman Coding algorithm comes into play with 0 going left and 1 going right
    generateEncodingTableHelper(encodingTable, root->left, code + "0");
    generateEncodingTableHelper(encodingTable, root->right, code + "1");
}

void generateEncodingString(std::ifstream& input, const std::unordered_map<std::optional<char>, std::string>& encodingTable, std::string& encodingString) {
    // clear string and move file pointer back to beginning
    encodingString.clear();
    input.clear(); // check if error state
    input.seekg(0, std::ios::beg);

    char character;
    while (input.get(character)) {
        insertEncodedCharacter(encodingTable, encodingString, character);
    }
}

void insertEncodedCharacter(const std::unordered_map<std::optional<char>, std::string>& encodingTable, std::string& encodingString, const char character) {
    auto encoding{encodingTable.find(character)};

    if (encoding != encodingTable.end()) {
        encodingString += encoding->second;
    } else {
        std::cout << "Character not found in encoding table.\n";
    }
}

void generateHuffmanTreeRepresentation(std::string& representation, const HuffmanNode* root) {
    representation.clear();
    generateHuffmanTreeRepresentationHelper(representation, root);
}

void generateHuffmanTreeRepresentationHelper(std::string& representation, const HuffmanNode* root) {
    // base case
    if (root == nullptr) return;

    // preorder traversal is used to record the tree representation

    // if the key has a value, encode 0 and then the 8-bit representation (9 bits total)
    if (root->key.has_value()) {
        representation += '0';

        // get the character value and loop through every bit from left to right.
        // for example, 'h' has the ASCII representation of '01101000'.
        // loop down from 7 to 0 inclusive, using the right-shift operator (>>) based on index
        // and using the bitwise AND operator (&) to evaluate the moved bit.

        // https://www.geeksforgeeks.org/cpp-bitwise-operators/

        char character{root->key.value()};
        for (int i{7}; i >= 0; --i) {
            bool result{static_cast<bool>((character >> i) & 1)};
            representation += result ? '1' : '0';
        }
    } else {
        // leaf node
        representation += '1';
    }

    // recursively continue
    generateHuffmanTreeRepresentationHelper(representation, root->left);
    generateHuffmanTreeRepresentationHelper(representation, root->right);
}

void generateFileInfoEncoding(FileInformation& information, std::string& infoEncoding) {
    infoEncoding.clear();

    for (char c : information.fileName) {
        for (int i{7}; i >= 0; --i) {
            bool result{static_cast<bool>((c >> i) & 1)};
            infoEncoding += result ? '1' : '0';
        }
    }

    for (char c : information.fileExtension) {
        for (int i{7}; i >= 0; --i) {
            bool result{static_cast<bool>((c >> i) & 1)};
            infoEncoding += result ? '1' : '0';
        }
    }
}

void generateHuffmanFileHeader(HuffmanHeader& header, std::size_t iLength, std::size_t tLength, std::size_t eLength) {
    header.infoLength = static_cast<uint32_t>(iLength);
    header.treeLength = static_cast<uint32_t>(tLength);
    header.encodingLength = static_cast<uint32_t>(eLength);
}
