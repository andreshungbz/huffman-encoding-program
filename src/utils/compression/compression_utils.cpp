#include "compression_utils.h"

#include <iostream>

// compress helper function

void writeSection(std::ofstream& output, const std::string& section) {
    std::string string{section}; // copy over section string
    // the calculation in parentheses gives us number of 0s to pad; extra % 8 ensures that
    // if the string is already divisible by 8, then the padding becomes 0
    std::size_t paddingCount{(8 - (section.length() % 8)) % 8};
    string.append(paddingCount, '0'); // pad as necessary

    // follow previous loop to write byte chunks
    for (std::size_t i{0}; i < string.length(); i += 8) {
        // construct the 8-bit byte chunk by setting the appropriate bit
        // this time use the left shift operator (<<) to set the appropriate bit which gets set to byte
        // via the bitwise OR operator (|)
        int byte{0};
        for (int j{0}; j < 8; ++j) {
            if (string[i + j] == '1') {
                // integer 1 represented as 00000001
                byte |= (1 << (7 - j));
            }
        }

        // write byte chunk into file
        char charByte{static_cast<char>(byte)};
        output.write(&charByte, 1);
    }
}

// decompress helper functions

void readSection(std::ifstream& input, std::string& section, uint32_t size) {
    section.clear();

    // read the section and instantiate the appropriate data member
    int bytesCount{(static_cast<int>(size) + 7) / 8};
    for (int i{0}; i < bytesCount; ++i) { // this ensures file pointer ends after the padding
        char byte{};
        input.read(&byte, 1);

        for (int j{0}; j < 8; ++j) {
            if (i * 8 + j < size) { // don't count padding bits
                section += (byte & (1 << 7 - j)) ? '1' : '0';
            }
        }
    }
}

void instantiateHuffmanFileInformation(FileInformation& information, const std::string& infoEncoding) {
    // use huffmanFileInfoEncoding to instantiate fileInformation's fileName and fileExtension

    bool isFileName{true};
    for (size_t i = 0; i < infoEncoding.length(); i += 8) {
        // read byte
        int byte{0};
        for (size_t j = 0; j < 8; j++) {
            if (infoEncoding[i + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }

        // check character
        char character{static_cast<char>(byte)};

        // when period is reached, change condition status so that fileExtension gets appended instead
        if (character == '.') {
            isFileName = false;
        }

        // append to fileName and fileExtension accordingly
        if (isFileName) {
            information.fileName += character;
        } else {
            information.fileExtension += character;
        }
    }
}

HuffmanNode* instantiateHuffmanTree(const std::string& representation, int& position) {
    if (position >= representation.length()) {
        return nullptr;
    }

    if (representation[position] == '0') {
        // leaf node - next 8 bits represent the character
        ++position;
        int byte{0};
        for (int i{0}; i < 8; ++i) {
            if (representation[position + i] == '1') {
                byte |= (1 << (7 - i));
            }
        }
        position += 8;
        return new HuffmanNode(static_cast<char>(byte), 0);  // Weight doesn't matter for decompression
    }

    // internal node
    ++position;
    auto* node = new HuffmanNode(0);  // Create internal node
    node->left = instantiateHuffmanTree(representation, position);
    node->right = instantiateHuffmanTree(representation, position);

    return node;
}

void writeDecompressedFile(const std::string& path, HuffmanNode* root, const std::string& encodingString) {
    std::ofstream output(path, std::ios::out | std::ios::binary);

    if (!output) {
        std::cout << "Write Decompressed File Error\n";
        return;
    }

    // traverse through Huffman Encoding and where a leaf node is reached, insert character
    HuffmanNode* currentPtr{root};
    for (char bit : encodingString) {
        if (bit == '0') {
            currentPtr = currentPtr->left;
        } else if (bit == '1') {
            currentPtr = currentPtr->right;
        }

        // when a leaf node is reached, write to file and reset to root
        if (!currentPtr->left && !currentPtr->right) {
            output.put(currentPtr->key.value());
            currentPtr = root;
        }
    }

    output.close();
}
