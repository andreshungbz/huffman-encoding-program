// Huffman Tree Implementation

#include "huffman_tree/HuffmanTree.h"

#include <fstream>
#include <unordered_map>

#include "priority_queue/PriorityQueue.h"

#include "utils/generate/generate_utils.h"
#include "utils/compression/compression_utils.h"
#include "utils/instantiate/instantiate_utils.h"

HuffmanTree::HuffmanTree(std::ifstream& input, const std::string& name, const std::string& extension,
                         const std::string& destination) {
    // create fileInformation
    fileInformation = FileInformation{name, extension};

    // build Huffman Tree
    FrequencyHashMap hashMap{input, 10}; // hash map of frequencies of each character
    PriorityQueue priorityQueue{hashMap}; // min-heap priority queue where the lowest weight is accessed first
    huffmanTreeRoot = priorityQueue.getHuffmanTree(); // pass the constructed Huffman Tree in the priority queue

    // construct all data members
    compress(input, destination);
}

void HuffmanTree::compress(std::ifstream& input, const std::string& destination) {
    // generate data members
    generate(input);

#if defined(_WIN32)
    char slash = '\\';
#else
    char slash = '/';
#endif

    // write the compressed file
    std::string compressedFilePath{destination + slash + fileInformation.fileName + ".hzip"};
    writeCompressedFile(compressedFilePath, huffmanHeader, huffmanFileInfoCode, huffmanTreeRepresentation, huffmanCode);
}

void HuffmanTree::generate(std::ifstream& input) {
    // generate encoding table
    std::unordered_map<std::optional<char>, std::string> encodingTable{};
    generateEncodingTable(encodingTable, huffmanTreeRoot);

    // generate each section
    generateFileInfoCode(fileInformation, huffmanFileInfoCode);
    generateHuffmanTreeRepresentation(huffmanTreeRepresentation, huffmanTreeRoot);
    generateHuffmanCode(input, encodingTable, huffmanCode);

    // generate the header from each section
    generateHuffmanHeader(huffmanHeader, huffmanFileInfoCode.length(), huffmanTreeRepresentation.length(),
                          huffmanCode.length());
}

void HuffmanTree::decompress(std::ifstream& input, const std::string& destination) {
    // read and instantiate huffmanHeader, huffmanFileInfoCode, huffmanTreeRepresentation, and huffmanCode.
    readCompressedFile(input, huffmanHeader, huffmanFileInfoCode, huffmanTreeRepresentation, huffmanCode);

    // reconstruct fileInformation and huffmanTreeRoot
    instantiate();

#if defined(_WIN32)
    char slash = '\\';
#else
    char slash = '/';
#endif

    // write the original file
    std::string decompressedFilePath = destination + slash + fileInformation.fileName + "-decompressed" +
        fileInformation.fileExtension;
    // write decompressed file
    writeDecompressedFile(decompressedFilePath, huffmanTreeRoot, huffmanCode);
}

void HuffmanTree::instantiate() {
    // post-condition: fileInformation has fileName and fileExtension
    instantiateFileInformation(fileInformation, huffmanFileInfoCode);
    // post-condition: huffmanTreeRoot has the original Huffman Tree
    int position{0};
    huffmanTreeRoot = instantiateHuffmanTree(huffmanTreeRepresentation, position);
}
