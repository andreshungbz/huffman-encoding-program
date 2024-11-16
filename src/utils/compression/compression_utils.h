#ifndef COMPRESSION_UTILS_H
#define COMPRESSION_UTILS_H


#include <cstdint>
#include <fstream>

#include "huffman_tree/components/FileInformation.h"
#include "huffman_tree/HuffmanNode.h"

// compress helper function
void writeSection(std::ofstream& output, const std::string& section);
// decompress helper functions
void readSection(std::ifstream& input, std::string& section, uint32_t size);
void instantiateHuffmanFileInformation(FileInformation& information, const std::string& infoEncoding);
HuffmanNode* instantiateHuffmanTree(const std::string& representation, int& position);
void writeDecompressedFile(const std::string& path, HuffmanNode* root, const std::string& encodingString);


#endif // COMPRESSION_UTILS_H
