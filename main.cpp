// https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1234/resources/huffman.html

// read on little-endian format later for 32-bit unsigned integers
// file named input.txt with content "happy_hip_hop" gives the following output in raw bits for the header
// 01001000 00000000 00000000 00000000 for 72
// 01000101 00000000 00000000 00000000 for 69
// 00100010 00000000 00000000 00000000 for 34

#include <fstream>
#include <iostream>
#include <string>

#include "huffman_tree/HuffmanTree.h"
#include "utils/file/file_utils.h"

int main() {
    // define slash delimiter for Windows and Unix/Linux
#if defined(_WIN32)
    std::string slash = "\\";
#else
    std::string slash = "/";
#endif

    // COMPRESSION

    std::string path{".." + slash + "test" + slash + "input.txt"};
    std::ifstream input{path, std::ios::in | std::ios::binary}; // read in binary mode
    if (!input) {
        std::cout << "File Open Error\n";
        return 1;
    }

    // RETRIEVE DIRECTORY + FILE NAME + FILE EXTENSION
    std::string directory{getDirectory(path)};
    std::string fileName{getFileName(path)};
    std::string fileExtension{getFileExtension(path)};

    // testing compress
    HuffmanTree huffmanTree{input, fileName, fileExtension, directory};
    huffmanTree.compress(input, directory);

    input.close();


    // DECOMPRESSION

    std::string dPath{".." + slash + "test" + slash + "input.hzip"};
    std::ifstream input2{dPath, std::ios::in | std::ios::binary}; // read in binary mode
    if (!input2) {
        std::cout << "Compressed File Open Error\n";
        return 2;
    }

    // testing decompress
    HuffmanTree decompressHuffmanTree{};
    decompressHuffmanTree.decompress(input2, directory);

    input2.close();

    // COMPARE ORIGINAL AND COMPRESSED SIZES
    std::size_t originalSize{getFileSize(path)};
    std::size_t compressedSize{getFileSize(dPath)};

    return 0;
}
