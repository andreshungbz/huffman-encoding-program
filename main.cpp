// https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1234/resources/huffman.html

// read on little-endian format later for 32-bit unsigned integers
// file named input.txt with content "happy_hip_hop" gives the following output in raw bits for the header
// 01001000 00000000 00000000 00000000 for 72
// 01000101 00000000 00000000 00000000 for 69
// 00100010 00000000 00000000 00000000 for 34

#include <string>

#include "huffman_tree/HuffmanTree.h"

int main() {
    // define slash delimiter for Windows and Unix/Linux
#if defined(_WIN32)
    char slash = "\\";
#else
    std::string slash = "/";
#endif

    std::string path{".." + slash + "test" + slash + "input.txt"};

    // testing compress
    HuffmanTree huffmanTree{path};
    huffmanTree.compress();

    std::string dPath{".." + slash + "test" + slash + "input.hzip"};

    // testing decompress
    HuffmanTree decompressHuffmanTree{};
    decompressHuffmanTree.decompress(dPath);

    return 0;
}
