#include "compression_utils.h"

#include <iostream>

// compress helper functions

void writeSection(std::ofstream& output, const std::string& section) {
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

void writeCompressedFile(const std::string& destination, const HuffmanHeader& header, const std::string& information, const std::string& representation, const std::string& encoding) {
    std::ofstream output{destination,std::ios::out | std::ios::binary};

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
    output.write(reinterpret_cast<const char*>(&header), sizeof(HuffmanHeader));

    // write Huffman File Information (always in byte (8-bit) chunks)
    writeSection(output, information);

    // write Huffman Tree Representation (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose tree representation is 69 bits, padding count is 3
    writeSection(output, representation);

    // write Huffman Encoding (may have incomplete byte so pad the end with 0s)
    // for happy_hip_hop whose encoding string is 34 bits, padding count is 6
    writeSection(output, encoding);

    output.close();
}

// decompress helper functions

void readSection(std::ifstream& input, std::string& section, uint32_t size) {
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

void writeDecompressedFile(const std::string& destination, HuffmanNode* root, const std::string& encodingString) {
    std::ofstream output(destination, std::ios::out | std::ios::binary);

    if (!output) {
        std::cout << "Write Decompressed File Error\n";
        return;
    }

    // traverse through Huffman Encoding and where a leaf node is reached, insert character
    HuffmanNode* currentPtr{root};
    for (char bit : encodingString) {
        if (bit == '0') {
            currentPtr = currentPtr->left;
        } else if (bit == '1') {
            currentPtr = currentPtr->right;
        }

        // when a leaf node is reached, write to file and reset to root
        if (!currentPtr->left && !currentPtr->right) {
            output.put(currentPtr->key.value());
            currentPtr = root;
        }
    }

    output.close();
}
