// Huffman Header Implementation

// This class contains the true byte count of the sections written to file, namely the File Information Code,
// Tree Representation, and Huffman Code. They are stored and read as 32-bit unsigned integers, which covers the size
// of most text files. The header written to file will always be 12 bytes.

// Not relevant to this project, but further reading about big-endian and little-endian systems could be interesting.
// https://library.mosse-institute.com/articles/2022/04/endian-systems-explained-little-endian-vs-big-endian/endian-systems-explained-little-endian-vs-big-endian.html

#ifndef HUFFMAN_HEADER_H
#define HUFFMAN_HEADER_H


#include <cstdint>

class HuffmanHeader {
public:
    // constructor
    HuffmanHeader(uint32_t iLength, uint32_t tLength, uint32_t eLength)
        : infoLength(iLength), treeLength(tLength), encodingLength(eLength) {}

    // data members
    uint32_t infoLength{};
    uint32_t treeLength{};
    uint32_t encodingLength{};
};


#endif // HUFFMAN_HEADER_H
