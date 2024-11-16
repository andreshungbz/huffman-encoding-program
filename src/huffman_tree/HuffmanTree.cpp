#include "huffman_tree/HuffmanTree.h"

#include <fstream>
#include <iostream>

#include "priority_queue/PriorityQueue.h"

#include "utils/file/file_utils.h"
#include "utils/generate/generate_utils.h"
#include "utils/compression/compression_utils.h"

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
    std::ofstream output{compressedFilePath,std::ios::out | std::ios::binary};

    if (!output) {
        std::cout << "File Write Error\n";
        return;
    }

    // read on little-endian format later
    // file named input.txt with content "happy_hip_hop" gives the following output in raw bits for the header
    // 01001000 00000000 00000000 00000000 for 72
    // 01000101 00000000 00000000 00000000 for 69
    // 00100010 00000000 00000000 00000000 for 34

    // write Huffman File Header (always 12 bytes)
    output.write(reinterpret_cast<const char*>(&fileHeader), sizeof(HuffmanHeader));

    // write Huffman File Information (always in byte (8-bit) chunks)
    writeSection(output, huffmanFileInfoCode);

    // write Huffman Tree Representation (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose tree representation is 69 bits, padding count is 3
    writeSection(output, huffmanTreeRepresentation);

    // write Huffman Encoding (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose encoding string is 34 bits, padding count is 6
    writeSection(output, huffmanCode);

    // close file
    output.close();
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

    // read and instantiate Huffman File Info Encoding along with fileInformation.fileName and fileInformation.fileExtension
    readSection(input, huffmanFileInfoCode, fileHeader.infoLength);
    instantiateHuffmanFileInformation(fileInformation, huffmanFileInfoCode);

    // read and instantiate Huffman Tree Representation and build huffmanTreeRoot
    readSection(input, huffmanTreeRepresentation, fileHeader.treeLength);
    int position{0};
    huffmanTreeRoot = instantiateHuffmanTree(huffmanTreeRepresentation, position);

    // read and instantiate Huffman Encoding String
    readSection(input, huffmanCode, fileHeader.encodingLength);

    // construct original file path
    std::string directory = getDirectory(source);
    std::string decompressedFilePath = directory + '/' + fileInformation.fileName + "-decompressed" + fileInformation.fileExtension;

    // write decompressed file
    writeDecompressedFile(decompressedFilePath, huffmanTreeRoot, huffmanCode);

    input.close();
}
