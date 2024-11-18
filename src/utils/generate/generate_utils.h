// Generate Utilities Header

// This module contains a number of functions used when data members are being generated when compression is
// occurring. In each main generate function, the corresponding value is cleared or overwritten first. Additionally,
// helper functions maybe be used where recursion is used.

// The generateEncodingTable function traverses the Huffman binary tree and populates the encoding table with the
// character to Huffman Code mapping. This is done when a leaf node is reached, otherwise traversal to the left adds 0
// to the code, and traversal to the right adds 1.

// The generateFileInfoCode function encodes the file name and extension inclusive of the period. To create the
// ASCII byte representation, each character byte is read from the std::string, and looped for each bit using the
// right shift (>>) and bitwise AND (&) operators.

// The generateHuffmanTreeRepresentation function encodes the Huffman Tree by using preorder traversal, and noting
// each node. For every non-leaf node, 1 is recorded; for every leaf node with a value, 0 is recorded and then the
// 8-bit representation of the character using the right shift and bitwise AND operators.

// The generateHuffmanCode function uses the previously generated encoding table to take the original file and
// create the complete Huffman Code.

// The generateHuffmanHeader function simply assigns the header values, type cast with the correct uint32_t type.

#ifndef GENERATE_UTILS_H
#define GENERATE_UTILS_H


#include <fstream>
#include <string>
#include <unordered_map>

#include "huffman_tree/components/FileInformation.h"
#include "huffman_tree/components/HuffmanHeader.h"
#include "huffman_tree/HuffmanNode.h"

// https://www.learncpp.com/cpp-tutorial/typedefs-and-type-aliases/
typedef std::unordered_map<std::optional<char>, std::string> EncodingTable; // simple alias for the hash map type

// generate encoding table
void generateEncodingTable(EncodingTable& encodingTable, const HuffmanNode* root);
void generateEncodingTableHelper(EncodingTable& encodingTable, const HuffmanNode* root, const std::string& code);

// generate file information code
void generateFileInfoCode(FileInformation& information, std::string& infoEncoding);

// generate tree representation
void generateHuffmanTreeRepresentation(std::string& representation, const HuffmanNode* root);
void generateHuffmanTreeRepresentationHelper(std::string& representation, const HuffmanNode* root);

// generate huffman code
void generateHuffmanCode(std::ifstream& input, const EncodingTable& encodingTable, std::string& encodingString);

// generate huffman header
void generateHuffmanHeader(HuffmanHeader& header, std::size_t iLength, std::size_t tLength, std::size_t eLength);


#endif // GENERATE_UTILS_H
