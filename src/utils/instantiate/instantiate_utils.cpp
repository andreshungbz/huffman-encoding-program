// Instantiate Utilities Implementation

#include "instantiate_utils.h"

void instantiateFileInformation(FileInformation& information, const std::string& infoEncoding) {
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
    // base case: position is past the boundary
    if (position >= representation.length()) {
        return nullptr;
    }

    // leaf node
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
        return new HuffmanNode(static_cast<char>(byte), 0);
    }

    // internal node
    ++position;
    auto* node = new HuffmanNode(0); // Create internal node
    node->left = instantiateHuffmanTree(representation, position);
    node->right = instantiateHuffmanTree(representation, position);

    return node;
}
