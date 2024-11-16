#ifndef COMPRESSION_UTILS_H
#define COMPRESSION_UTILS_H


#include <cstdint>
#include <fstream>

#include "huffman_tree/HuffmanNode.h"
#include "huffman_tree/components/HuffmanHeader.h"

// compress helper functions
void writeSection(std::ofstream& output, const std::string& section);
void writeCompressedFile(const std::string& destination, const HuffmanHeader& header, const std::string& information, const std::string& representation, const std::string& encoding);

// decompress helper functions
void readSection(std::ifstream& input, std::string& section, uint32_t size);
void writeDecompressedFile(const std::string& destination, HuffmanNode* root, const std::string& encodingString);


#endif // COMPRESSION_UTILS_H
