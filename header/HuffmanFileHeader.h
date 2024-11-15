// Huffman File Header Implementation

#ifndef HUFFMAN_FILE_HEADER_H
#define HUFFMAN_FILE_HEADER_H


#include <cstdint>

class HuffmanFileHeader {
public:
    // constructor
    HuffmanFileHeader(uint32_t infoLength, uint32_t treeLength, uint32_t encodingLength)
        : fileInfoLength(infoLength), treeRepresentationLength(treeLength), huffmanEncodingLength(encodingLength) {}

    // data members
    uint32_t fileInfoLength{};
    uint32_t treeRepresentationLength{};
    uint32_t huffmanEncodingLength{};
};


#endif // HUFFMAN_FILE_HEADER_H
