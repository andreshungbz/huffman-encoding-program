#include "huffman_tree/HuffmanTree.h"

#include <fstream>
#include <iostream>

#include "priority_queue/PriorityQueue.h"

#include "utils/file/file_utils.h"
#include "utils/generate/generate_utils.h"
#include "utils/compression/compression_utils.h"
#include "utils/instantiate/instantiate_utils.h"

HuffmanTree::HuffmanTree(const std::string& source) {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{source, std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "File Open Error\n";
        return;
    }

    generate(input, source);

    input.close();
}

void HuffmanTree::generate(std::ifstream& input, const std::string& source) {
    // get information about file and store in fileInformation
    std::string directory = getDirectory(source);
    std::string fileName = getFileName(source);
    std::string fileExtension = getFileExtension(source);
    std::size_t fileSize = getFileSize(source);
    fileInformation = FileInformation{fileName, fileExtension, fileSize, directory};

    // create frequency hash map with specified number of buckets
    FrequencyHashMap hashMap{input, 10};

    // create priority queue and populate with every key in hash map
    PriorityQueue priorityQueue{hashMap};

    // assign constructed Huffman Tree to this object's root
    huffmanTreeRoot = priorityQueue.getHuffmanTree();

    // generate other structures
    generateEncodingTable(encodingTable, huffmanTreeRoot);
    generateEncodingString(input, encodingTable, huffmanCode);
    generateHuffmanTreeRepresentation(huffmanTreeRepresentation, huffmanTreeRoot);
    generateFileInfoEncoding(fileInformation, huffmanFileInfoCode);
    generateHuffmanFileHeader(fileHeader, huffmanFileInfoCode.length(), huffmanTreeRepresentation.length(), huffmanCode.length());
}

void HuffmanTree::compress() const {
    std::string compressedFilePath{fileInformation.fileDirectory + "/" + fileInformation.fileName + ".hzip"};
    writeCompressedFile(compressedFilePath, fileHeader, huffmanFileInfoCode, huffmanTreeRepresentation, huffmanCode);
}

void HuffmanTree::decompress(const std::string& source) {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{source, std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "Compressed File Open Error\n";
        return;
    }

    // read and instantiate Huffman File Header
    input.read(reinterpret_cast<char*>(&fileHeader), sizeof(HuffmanHeader));

    // read each encoded section and instantiate respective data members
    readSection(input, huffmanFileInfoCode, fileHeader.infoLength);
    readSection(input, huffmanTreeRepresentation, fileHeader.treeLength);
    readSection(input, huffmanCode, fileHeader.encodingLength);
    // construct file name, file extension, and build Huffman Tree
    instantiate();

    // construct original file path
    std::string directory = getDirectory(source);
    std::string decompressedFilePath = directory + '/' + fileInformation.fileName + "-decompressed" + fileInformation.fileExtension;

    // write decompressed file
    writeDecompressedFile(decompressedFilePath, huffmanTreeRoot, huffmanCode);

    input.close();
}

void HuffmanTree::instantiate() {
    int position{0};
    instantiateFileInformation(fileInformation, huffmanFileInfoCode);
    huffmanTreeRoot = instantiateHuffmanTree(huffmanTreeRepresentation, position);
}
