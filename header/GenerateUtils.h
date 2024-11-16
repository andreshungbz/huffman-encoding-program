#ifndef GENERATE_UTILS_H
#define GENERATE_UTILS_H


#include <fstream>
#include <string>
#include <unordered_map>

#include "FileInformation.h"
#include "HuffmanFileHeader.h"
#include "huffman_tree/HuffmanNode.h"

void generateEncodingTable(std::unordered_map<std::optional<char>, std::string>& encodingTable, const HuffmanNode* root);
void generateEncodingTableHelper(std::unordered_map<std::optional<char>, std::string>& encodingTable, const HuffmanNode* root, const std::string& code);

void generateEncodingString(std::ifstream& input, const std::unordered_map<std::optional<char>, std::string>& encodingTable, std::string& encodingString);
void insertEncodedCharacter(const std::unordered_map<std::optional<char>, std::string>& encodingTable, std::string& encodingString, char character);

void generateHuffmanTreeRepresentation(std::string& representation, const HuffmanNode* root);
void generateHuffmanTreeRepresentationHelper(std::string& representation, const HuffmanNode* root);

void generateFileInfoEncoding(FileInformation& information, std::string& infoEncoding);

void generateHuffmanFileHeader(HuffmanFileHeader& header, std::size_t iLength, std::size_t tLength, std::size_t eLength);


#endif // GENERATE_UTILS_H
