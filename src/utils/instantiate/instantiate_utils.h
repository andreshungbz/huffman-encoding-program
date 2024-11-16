#ifndef INSTANTIATE_UTILS_H
#define INSTANTIATE_UTILS_H


#include "huffman_tree/HuffmanNode.h"
#include "huffman_tree/components/FileInformation.h"

void instantiateFileInformation(FileInformation& information, const std::string& infoEncoding);
HuffmanNode* instantiateHuffmanTree(const std::string& representation, int& position);


#endif // INSTANTIATE_UTILS_H
