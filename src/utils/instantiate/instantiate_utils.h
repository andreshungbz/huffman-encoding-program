// Instantiate Utilities Header

// This module consists of functions used to recreate or instantiate the File Information data member as well as the
// original Huffman Tree. This is done after the compressed file is read in the decompress HuffmanTree function.
// Once these data members are instantiated, the original file can then be recreated.

// The instantiateFileInformation function reads byte by byte, appending each character to the file name until a
// period is reached, from then the file extension will be appended to till the end.

// The instantiateHuffmanTree function is a recursive function that reads the Tree Representation so that the original
// Huffman Tree can be recreated. The tree is built top-down in a preorder traversal fashion. When a 1 is read, a
// new Node is created with character key empty and the left and right are assigned with the incremented position.
// When a 0 is read, a substantiated node with a character key is created after decoding the resulting byte. The
// position variable is moved accordingly and the node is recursively assigned to its appropriate parent. Weights are
// not store and are not needed at this point.

#ifndef INSTANTIATE_UTILS_H
#define INSTANTIATE_UTILS_H


#include "huffman_tree/HuffmanNode.h"
#include "huffman_tree/components/FileInformation.h"

void instantiateFileInformation(FileInformation& information, const std::string& infoEncoding);
HuffmanNode* instantiateHuffmanTree(const std::string& representation, int& position);


#endif // INSTANTIATE_UTILS_H
