// https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1234/resources/huffman.html

#include <string>

#include "huffman_tree/HuffmanTree.h"

int main() {
    std::string path{".."};

    // create relative path for Windows and Unix/Linux
    #if defined(_WIN32)
        path += "\\test\\";
    #else
        path +="/test/";
    #endif

    // append word list file
    path += "input.txt";

    // create Huffman Tree
    HuffmanTree huffmanTree{path};

    return 0;
}
