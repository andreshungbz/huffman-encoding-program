// Huffman File Header Implementation

#ifndef HUFFMAN_FILE_HEADER_H
#define HUFFMAN_FILE_HEADER_H


#include <cstdint>

class HuffmanFileHeader {
public:
    // constructor
    HuffmanFileHeader(uint32_t iLength, uint32_t tLength, uint32_t eLength)
        : infoLength(iLength), treeLength(tLength), encodingLength(eLength) {}

    // data members
    uint32_t infoLength{};
    uint32_t treeLength{};
    uint32_t encodingLength{};
};


#endif // HUFFMAN_FILE_HEADER_H
