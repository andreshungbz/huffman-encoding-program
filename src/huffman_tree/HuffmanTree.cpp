#include <fstream>
#include <iostream>

#include "huffman_tree/HuffmanTree.h"
#include "FileUtils.h"
#include "priority_queue/PriorityQueue.h"
#include "GenerateUtils.h"
#include "CompressionUtils.h"

HuffmanTree::HuffmanTree(const std::string& path) {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{path, std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "File Open Error\n";
        return;
    }

    generate(input, path);

    input.close();
}

void HuffmanTree::generate(std::ifstream& input, const std::string& path) {
    // get information about file and store in fileInformation
    std::string directory = getDirectory(path);
    std::string fileName = getFileName(path);
    std::string fileExtension = getFileExtension(path);
    std::size_t fileSize = getFileSize(path);
    fileInformation = FileInformation{fileName, fileExtension, fileSize, directory};

    // create frequency hash map with specified number of buckets
    FrequencyHashMap hashMap{input, 10};

    // create priority queue and populate with every key in hash map
    PriorityQueue priorityQueue{hashMap};

    // assign constructed Huffman Tree to this object's root
    huffmanTreeRoot = priorityQueue.getHuffmanTree();

    // generate other structures
    generateEncodingTable(huffmanEncodingTable, huffmanTreeRoot);
    generateEncodingString(input, huffmanEncodingTable, huffmanEncodingString);
    generateHuffmanTreeRepresentation(huffmanTreeRepresentation, huffmanTreeRoot);
    generateFileInfoEncoding(fileInformation, huffmanFileInfoEncoding);
    generateHuffmanFileHeader(huffmanFileHeader, huffmanFileInfoEncoding.length(), huffmanTreeRepresentation.length(), huffmanEncodingString.length());
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
    output.write(reinterpret_cast<const char*>(&huffmanFileHeader), sizeof(HuffmanFileHeader));

    // write Huffman File Information (always in byte (8-bit) chunks)
    writeSection(output, huffmanFileInfoEncoding);

    // write Huffman Tree Representation (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose tree representation is 69 bits, padding count is 3
    writeSection(output, huffmanTreeRepresentation);

    // write Huffman Encoding (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose encoding string is 34 bits, padding count is 6
    writeSection(output, huffmanEncodingString);

    // close file
    output.close();
}

void HuffmanTree::decompress(const std::string& path) {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{path, std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "Compressed File Open Error\n";
        return;
    }

    // read and instantiate Huffman File Header
    input.read(reinterpret_cast<char*>(&huffmanFileHeader), sizeof(HuffmanFileHeader));

    // read and instantiate Huffman File Info Encoding along with fileInformation.fileName and fileInformation.fileExtension
    readSection(input, huffmanFileInfoEncoding, huffmanFileHeader.infoLength);
    instantiateHuffmanFileInformation(fileInformation, huffmanFileInfoEncoding);

    // read and instantiate Huffman Tree Representation and build huffmanTreeRoot
    readSection(input, huffmanTreeRepresentation, huffmanFileHeader.treeLength);
    int position{0};
    huffmanTreeRoot = instantiateHuffmanTree(huffmanTreeRepresentation, position);

    // read and instantiate Huffman Encoding String
    readSection(input, huffmanEncodingString, huffmanFileHeader.encodingLength);

    // construct original file path
    std::string directory = getDirectory(path);
    std::string decompressedFilePath = directory + '/' + fileInformation.fileName + "-decompressed" + fileInformation.fileExtension;

    // write decompressed file
    writeDecompressedFile(decompressedFilePath, huffmanTreeRoot, huffmanEncodingString);

    input.close();
}
