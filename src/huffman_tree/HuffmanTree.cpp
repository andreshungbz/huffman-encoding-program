#include <fstream>
#include <iostream>

#include "huffman_tree/HuffmanTree.h"
#include "FileUtils.h"
#include "priority_queue/PriorityQueue.h"
#include "GenerateUtils.h"

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
    instantiateHuffmanFileInformation();

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
    writeDecompressedFile(decompressedFilePath);

    input.close();
}

// compress helper function

void HuffmanTree::writeSection(std::ofstream& output, const std::string& section) {
    std::string string{section}; // copy over section string
    // the calculation in parentheses gives us number of 0s to pad; extra % 8 ensures that
    // if the string is already divisible by 8, then the padding becomes 0
    std::size_t paddingCount{(8 - (section.length() % 8)) % 8};
    string.append(paddingCount, '0'); // pad as necessary

    // follow previous loop to write byte chunks
    for (std::size_t i{0}; i < string.length(); i += 8) {
        // construct the 8-bit byte chunk by setting the appropriate bit
        // this time use the left shift operator (<<) to set the appropriate bit which gets set to byte
        // via the bitwise OR operator (|)
        int byte{0};
        for (int j{0}; j < 8; ++j) {
            if (string[i + j] == '1') {
                // integer 1 represented as 00000001
                byte |= (1 << (7 - j));
            }
        }

        // write byte chunk into file
        char charByte{static_cast<char>(byte)};
        output.write(&charByte, 1);
    }
}

// decompress helper functions

void HuffmanTree::readSection(std::ifstream& input, std::string& section, uint32_t size) {
    section.clear();

    // read the section and instantiate the appropriate data member
    int bytesCount{(static_cast<int>(size) + 7) / 8};
    for (int i{0}; i < bytesCount; ++i) { // this ensures file pointer ends after the padding
        char byte{};
        input.read(&byte, 1);

        for (int j{0}; j < 8; ++j) {
            if (i * 8 + j < size) { // don't count padding bits
                section += (byte & (1 << 7 - j)) ? '1' : '0';
            }
        }
    }
}

void HuffmanTree::instantiateHuffmanFileInformation() {
    // use huffmanFileInfoEncoding to instantiate fileInformation's fileName and fileExtension

    bool isFileName{true};
    for (size_t i = 0; i < huffmanFileInfoEncoding.length(); i += 8) {
        // read byte
        int byte{0};
        for (size_t j = 0; j < 8; j++) {
            if (huffmanFileInfoEncoding[i + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }

        // check character
        char character{static_cast<char>(byte)};

        // when period is reached, change condition status so that fileExtension gets appended instead
        if (character == '.') {
            isFileName = false;
        }

        // append to fileName and fileExtension accordingly
        if (isFileName) {
            fileInformation.fileName += character;
        } else {
            fileInformation.fileExtension += character;
        }
    }
}

HuffmanNode* HuffmanTree::instantiateHuffmanTree(const std::string& representation, int& position) {
    if (position >= representation.length()) {
        return nullptr;
    }

    if (representation[position] == '0') {
        // leaf node - next 8 bits represent the character
        ++position;
        int byte{0};
        for (int i{0}; i < 8; ++i) {
            if (representation[position + i] == '1') {
                byte |= (1 << (7 - i));
            }
        }
        position += 8;
        return new HuffmanNode(static_cast<char>(byte), 0);  // Weight doesn't matter for decompression
    }

    // internal node
    ++position;
    auto* node = new HuffmanNode(0);  // Create internal node
    node->left = instantiateHuffmanTree(representation, position);
    node->right = instantiateHuffmanTree(representation, position);

    return node;
}

void HuffmanTree::writeDecompressedFile(const std::string& path) {
    std::ofstream output(path, std::ios::out | std::ios::binary);

    if (!output) {
        std::cout << "Write Decompressed File Error\n";
        return;
    }

    // traverse through Huffman Encoding and where a leaf node is reached, insert character
    HuffmanNode* currentPtr{huffmanTreeRoot};
    for (char bit : huffmanEncodingString) {
        if (bit == '0') {
            currentPtr = currentPtr->left;
        } else if (bit == '1') {
            currentPtr = currentPtr->right;
        }

        // when a leaf node is reached, write to file and reset to root
        if (!currentPtr->left && !currentPtr->right) {
            output.put(currentPtr->key.value());
            currentPtr = huffmanTreeRoot;
        }
    }

    output.close();
}
