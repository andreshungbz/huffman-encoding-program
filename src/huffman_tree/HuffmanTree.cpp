#include <fstream>
#include <iostream>

#include "huffman_tree/HuffmanTree.h"
#include "FileUtils.h"

HuffmanTree::HuffmanTree(const std::string& path) {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{path, std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "File Open Error\n";
        return;
    }

    // get information about file
    std::string directory = getDirectory(path);
    std::string fileName = getFileName(path);
    std::string fileExtension = getFileExtension(path);
    std::size_t fileSize = getFileSize(path);

    // initialize members
    fileInformation = FileInformation(fileName, fileExtension, fileSize, directory);

    // create frequency hash map with specified number of buckets
    FrequencyHashMap hashMap{10};
    // read every character and insert into frequency hash map
    char character;
    while (input.get(character)) {
        // .get() evaluates false after reading invalid character
        hashMap.insertHashNode(character);
    }

    // create priority queue
    PriorityQueue queue{};
    // populate queue with every key in hash map
    populateQueue(queue, hashMap);

    // construct Huffman Tree
    queue.constructHuffmanTree();
    // assign constructed Huffman Tree to this object's root
    huffmanTreeRoot = queue.getHuffmanTree();

    // generate other structures
    generateEncodingTable();
    generateEncodingString(input);
    generateHuffmanTreeRepresentation();
    generateFileInfoEncoding();
    generateHuffmanFileHeader();

    // close file
    input.close();
}

void HuffmanTree::generateEncodingTable() {
    // clear encoding table
    huffmanEncodingTable.clear();

    generateEncodingTableHelper(huffmanTreeRoot, "");
}

void HuffmanTree::generateEncodingString(std::ifstream& input) {
    // clear string and move file pointer back to beginning
    huffmanEncodingString.clear();
    input.clear(); // check if error state
    input.seekg(0, std::ios::beg);

    char character;
    while (input.get(character)) {
        insertEncodedCharacter(character);
    }
}

void HuffmanTree::generateHuffmanTreeRepresentation() {
    huffmanTreeRepresentation.clear();
    generateHuffmanTreeRepresentationHelper(huffmanTreeRoot);
}

void HuffmanTree::generateFileInfoEncoding() {
    for (char c : fileInformation.fileName) {
        for (int i{7}; i >= 0; --i) {
            bool result{static_cast<bool>((c >> i) & 1)};
            huffmanFileInfoEncoding += result ? '1' : '0';
        }
    }

    for (char c : fileInformation.fileExtension) {
        for (int i{7}; i >= 0; --i) {
            bool result{static_cast<bool>((c >> i) & 1)};
            huffmanFileInfoEncoding += result ? '1' : '0';
        }
    }
}

void HuffmanTree::generateHuffmanFileHeader() {
    uint32_t infoLength{static_cast<uint32_t>(huffmanFileInfoEncoding.length())};
    uint32_t treeLength{static_cast<uint32_t>(huffmanTreeRepresentation.length())};
    uint32_t encodingLength{static_cast<uint32_t>(huffmanEncodingString.length())};

    huffmanFileHeader = HuffmanFileHeader{infoLength, treeLength, encodingLength};
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
    writeHuffmanFileInfo(output);

    // write Huffman Tree Representation (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose tree representation is 69 bits, padding count is 3
    writeHuffmanTreeRepresentation(output);

    // write Huffman Encoding (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose encoding string is 34 bits, padding count is 6
    writeHuffmanEncoding(output);

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
    readHuffmanFileInfo(input);

    // read and instantiate Huffman Tree Representation and build huffmanTreeRoot
    readHuffmanTreeRepresentation(input);
    int position{0};
    huffmanTreeRoot = reconstructHuffmanTree(huffmanTreeRepresentation, position);

    // read and instantiate Huffman Encoding String
    readHuffmanEncoding(input);

    // construct original file path
    std::string directory = getDirectory(path);
    std::string decompressedFilePath = directory + '/' + fileInformation.fileName + "-decompressed" + fileInformation.fileExtension;

    // write decompressed file
    writeDecompressedFile(decompressedFilePath);

    input.close();
}

// helper functions

void HuffmanTree::traverseBST(PriorityQueue& queue, const FrequencyHashNode* root) {
    // base case
    if (root == nullptr) {
        return;
    }

    // traverse inorder, going through every node and populating the queue
    queue.enqueue(root->key, root->frequency);
    traverseBST(queue, root->left);
    traverseBST(queue, root->right);
}

void HuffmanTree::populateQueue(PriorityQueue& queue, const FrequencyHashMap& hashMap) {
    for (FrequencyHashNode* tree : hashMap.buckets) {
        traverseBST(queue, tree);
    }
}

void HuffmanTree::generateEncodingTableHelper(const HuffmanNode* root, const std::string& code) {
    // base case: past leaf node nullptr
    if (root == nullptr) {
        return;
    }

    // add encoding for only leaf nodes
    if (root->left == nullptr && root->right == nullptr) {
        // special case: tree with only one node
        if (code.empty()) {
            huffmanEncodingTable[root->key] = "0";
        } else {
            huffmanEncodingTable[root->key] = code;
        }

        return; // short-circuit on successful addition
    }

    // for non-leaf nodes, recursively continue
    // here is where the Huffman Coding algorithm comes into play with 0 going left and 1 going right
    generateEncodingTableHelper(root->left, code + "0");
    generateEncodingTableHelper(root->right, code + "1");
}

void HuffmanTree::insertEncodedCharacter(const char character) {
    auto encoding{huffmanEncodingTable.find(character)};

    if (encoding != huffmanEncodingTable.end()) {
        huffmanEncodingString += encoding->second;
    } else {
        std::cout << "Character not found in encoding table.\n";
    }
}

void HuffmanTree::generateHuffmanTreeRepresentationHelper(const HuffmanNode* root) {
    // base case
    if (root == nullptr) return;

    // preorder traversal is used to record the tree representation

    // if the key has a value, encode 0 and then the 8-bit representation (9 bits total)
    if (root->key.has_value()) {
        huffmanTreeRepresentation += '0';

        // get the character value and loop through every bit from left to right.
        // for example, 'h' has the ASCII representation of '01101000'.
        // loop down from 7 to 0 inclusive, using the right-shift operator (>>) based on index
        // and using the bitwise AND operator (&) to evaluate the moved bit.

        // https://www.geeksforgeeks.org/cpp-bitwise-operators/

        char character{root->key.value()};
        for (int i{7}; i >= 0; --i) {
            bool result{static_cast<bool>((character >> i) & 1)};
            huffmanTreeRepresentation += result ? '1' : '0';
        }
    } else {
        // leaf node
        huffmanTreeRepresentation += '1';
    }

    // recursively continue
    generateHuffmanTreeRepresentationHelper(root->left);
    generateHuffmanTreeRepresentationHelper(root->right);
}

// compress helper functions

void HuffmanTree::writeHuffmanFileInfo(std::ofstream& output) const {
    // write Huffman File Information (always in byte (8-bit) chunks)
    // the file name with extension will be visible in a hex editor

    for (std::size_t i{0}; i < huffmanFileInfoEncoding.length(); i += 8) {
        int byte{0};
        // construct the 8-bit byte chunk by setting the appropriate bit
        // this time use the left shift operator (<<) to set the appropriate bit which gets set to byte
        // via the bitwise OR operator (|)
        for (int j{0}; j < 8; ++j) {
            if (huffmanFileInfoEncoding[i + j] == '1') {
                // integer 1 represented as 00000001
                byte |= (1 << (7 - j));
            }
        }

        // write byte chunk into file
        char charByte{static_cast<char>(byte)};
        output.write(&charByte, 1);
    }
}

void HuffmanTree::writeHuffmanTreeRepresentation(std::ofstream& output) const {
    // write Huffman Tree Representation (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose tree representation is 69 bits, padding count is 3

    std::string paddedTreeString{huffmanTreeRepresentation}; // copy over string
    // the calculation in parentheses gives us number of 0s to pad; extra % 8 ensures that
    // if the string is already divisible by 8, then the padCount becomes 0
    std::size_t padCountTree{(8 - (huffmanTreeRepresentation.length() % 8)) % 8};
    paddedTreeString.append(padCountTree, '0');

    // follow previous loop to write byte chunks
    for (std::size_t i{0}; i < paddedTreeString.length(); i += 8) {
        int byte{0};
        for (int j{0}; j < 8; ++j) {
            if (paddedTreeString[i + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }

        char charByte{static_cast<char>(byte)};
        output.write(&charByte, 1);
    }
}

void HuffmanTree::writeHuffmanEncoding(std::ofstream& output) const {
    // write Huffman Encoding (may have incomplete byte so pad the end with 0s)
    // follows same algorithm as writing huffmanTreeRepresentation
    // for happy_hip_hop whose encoding string is 34 bits, padding count is 6

    std::string paddedEncodingString{huffmanEncodingString};
    std::size_t padCountEncoding{(8 - (huffmanEncodingString.length() % 8)) % 8};
    paddedEncodingString.append(padCountEncoding, '0');

    // follow previous loop to write byte chunks
    for (std::size_t i{0}; i < paddedEncodingString.length(); i += 8) {
        int byte{0};
        for (int j{0}; j < 8; ++j) {
            if (paddedEncodingString[i + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }

        char charByte{static_cast<char>(byte)};
        output.write(&charByte, 1);
    }
}

// decompress helper functions

void HuffmanTree::readHuffmanFileInfo(std::ifstream& input) {
    huffmanFileInfoEncoding.clear();

    // read the file info section of the input and instantiate huffmanFileInfoEncoding

    int bytesCount{(static_cast<int>(huffmanFileHeader.fileInfoLength) + 7) / 8}; // main counter that will read all bytes
    for (int i{0}; i < bytesCount; ++i) {
        char byte{};
        input.read(&byte, 1);

        // because the File Info Encoding is guaranteed to be in byte chunks, it is safe to append all 8 bits
        for (int j{0}; j < 8; ++j) {
            huffmanFileInfoEncoding += (byte & (1 << 7 - j)) ? '1' : '0';
        }
    }

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

void HuffmanTree::readHuffmanTreeRepresentation(std::ifstream& input) {
    huffmanTreeRepresentation.clear();

    // read the tree representation section of the input and instantiate huffmanTreeRepresentation

    int bytesCount{(static_cast<int>(huffmanFileHeader.treeRepresentationLength) + 7) / 8};
    for (int i{0}; i < bytesCount; ++i) { // this ensures file pointer ends after the padding
        char byte{};
        input.read(&byte, 1);

        for (int j{0}; j < 8; ++j) {
            if (i * 8 + j < huffmanFileHeader.treeRepresentationLength) { // don't count padding bits
                huffmanTreeRepresentation += (byte & (1 << 7 - j)) ? '1' : '0';
            }
        }
    }
}

void HuffmanTree::readHuffmanEncoding(std::ifstream& input) {
    huffmanEncodingString.clear();

    // read the huffman encoding section of the input and instantiate huffmanEncodingString

    int bytesCount{(static_cast<int>(huffmanFileHeader.huffmanEncodingLength) + 7) / 8};
    for (int i{0}; i < bytesCount; ++i) { // this ensures file pointer ends after the padding
        char byte{};
        input.read(&byte, 1);

        for (int j{0}; j < 8; ++j) {
            if (i * 8 + j < huffmanFileHeader.huffmanEncodingLength) { // don't count padding bits
                huffmanEncodingString += (byte & (1 << 7 - j)) ? '1' : '0';
            }
        }
    }
}


HuffmanNode* HuffmanTree::reconstructHuffmanTree(const std::string& representation, int& position) {
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
    node->left = reconstructHuffmanTree(representation, position);
    node->right = reconstructHuffmanTree(representation, position);

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
