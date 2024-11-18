// Compression Utilities Header

// This module encapsulates a number of functions used by the HuffmanTree compress and decompress functions. Both
// writeCompressedFile and readCompressedFile functions consolidate a number of writeSection and readSection calls,
// respectively. writeDecompressedFile is used to write the original file by traversing the Huffman Tree.

// When writing each section using writeSection, bits from the std::string representation are accumulated in byte
// chunks. Each byte is manually constructed using the left shift (<<) and bitwise OR (|) operators. Padding of 0s is
// accounted for in the Tree Representation and Huffman Code sections.

// Reading each section using readSection works in a similar manner, accounting for the possible padded bits using
// the size from the Huffman Header as an additional condition when reconstructing the data members.

// The writeDecompressedFile iterates over the Huffman Code and uses the Huffman Tree in order to write the original
// file. For every leaf node reached based on the traversal, a character is written to file.

#ifndef COMPRESSION_UTILS_H
#define COMPRESSION_UTILS_H


#include <cstdint>
#include <fstream>

#include "huffman_tree/HuffmanNode.h"
#include "huffman_tree/components/HuffmanHeader.h"

// compress helper functions
void writeSection(std::ofstream& output, const std::string& section);
void writeCompressedFile(const std::string& destination, const HuffmanHeader& header, const std::string& information,
                         const std::string& representation, const std::string& encoding);

// decompress helper functions
void readSection(std::ifstream& input, std::string& section, uint32_t size);
void readCompressedFile(std::ifstream& input, HuffmanHeader& header, std::string& information,
                        std::string& representation, std::string& encoding);
void writeDecompressedFile(const std::string& destination, HuffmanNode* root, const std::string& encodingString);


#endif // COMPRESSION_UTILS_H
