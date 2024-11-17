#include "huffman_tree/HuffmanTree.h"

#include <fstream>
#include <iostream>
#include <unordered_map>

#include "priority_queue/PriorityQueue.h"

#include "utils/file/file_utils.h"
#include "utils/generate/generate_utils.h"
#include "utils/compression/compression_utils.h"
#include "utils/instantiate/instantiate_utils.h"

HuffmanTree::HuffmanTree(std::ifstream& input, const std::string& source) {
    // get information about file and store in fileInformation
    std::string fileName = getFileName(source);
    std::string fileExtension = getFileExtension(source);
    fileInformation = FileInformation{fileName, fileExtension};

    // construct all data members
    generate(input, source);

    input.close();
}

void HuffmanTree::generate(std::ifstream& input, const std::string& source) {
    // build Huffman Tree
    FrequencyHashMap hashMap{input, 10}; // hash map of frequencies of each character
    PriorityQueue priorityQueue{hashMap}; // min-heap priority queue where the lowest weight is accessed first
    huffmanTreeRoot = priorityQueue.getHuffmanTree(); // pass the constructed Huffman Tree in the priority queue

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

void HuffmanTree::compress(const std::string& destination) const {
    // construct file path such that the compressed file is in the same directory as the original file
    // uses the .hzip extension which is an unused extension

#if defined(_WIN32)
    char slash = '\\';
#else
    char slash = '/';
#endif

    std::string compressedFilePath{destination + slash + fileInformation.fileName + ".hzip"};

    // write compressed file
    writeCompressedFile(compressedFilePath, huffmanHeader, huffmanFileInfoCode, huffmanTreeRepresentation, huffmanCode);
}

void HuffmanTree::decompress(std::ifstream& input, const std::string& destination) {
    // read each section in the file and instantiate appropriate data members
    input.read(reinterpret_cast<char*>(&huffmanHeader), sizeof(HuffmanHeader)); // always 12 bytes
    readSection(input, huffmanFileInfoCode, huffmanHeader.infoLength); // always in byte chunks
    readSection(input, huffmanTreeRepresentation, huffmanHeader.treeLength); // may have padding at the end
    readSection(input, huffmanCode, huffmanHeader.encodingLength); // may have padding at the end

    input.close();

    // reconstruct file name, file extension, and build Huffman Tree
    instantiate();

    // construct file path such that the decompressed file is in the same directory as the compressed file
    // appends the file with "-decompressed" to avoid overwriting original file if in the same directory

#if defined(_WIN32)
    char slash = '\\';
#else
    char slash = '/';
#endif

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
