// Huffman Tree Header

// The third and final step of the Huffman Coding algorithm is to create the Huffman Code by using the Huffman Tree.
// Once the tree is constructed via the priority queue, the resulting binary tree is such that every leaf node contains
// a character value. To obtain the individual code for a character, the Huffman Tree is traversed until the corresponding
// leaf node is reached. For every left turn, 0 is added to its code; for every right turn 1 is added to its code. This
// is done for every character, and the resulting table of keys is used to encode the original file to create the
// Huffman Code. By having the minimal number of bits used for the most frequent characters, space is considerably saved.

// However, in the practical application of compressing a file on a computer using this algorithm, more consideration
// is to be had. The Huffman Code itself is not enough were we interested in decompressing or decoding the file later.
// A representation of the Huffman Tree must also be stored and reconstructed as it is only through that tree that we
// are able to interpret the Huffman Code. Additionally, we should also store the file name and extension, and
// implement a strategy for delimiting sections.

/* Compressed File Structure */

// In this implementation, a total is of 4 sections is written to file in the following order and read later in the
// same order. Each section is elaborated on subsequently.

// [Header] > [File Information Code] > [Tree Representation] > [Huffman Code]

// First is a Header section containing three 32-bit unsigned integer values
// corresponding to the true byte lengths of the other three sections. Other strategies for section delimiting
// include tagging and using special character sequences, but a header section is more convenient.

// Second is the File Information Code section which is simply the ASCII byte sequence for the file name and extension
// inclusive of the period. This can be seen using a hex dump tool such as xxd on Linux/macOS. This section will
// always be in byte chunks so no padding is necessary.

// Third is the Tree Representation section. To represent the tree, the following algorithm is used: traverse the
// tree in a preorder manner and for every non-leaf node (with no value), record a 1; for every leaf node (which has
// a value), record a 0 then the 8-bit representation of node's value. Weight is not needed in the reconstruction of
// the Huffman Tree, therefore it is not stored. Because this section can result in a count of bits not divisible by 8
// (as computers typically read), padding of 0s may be added at the end.

// Last is the Huffman Code section. This section may also have a padding of 0s at the end due to possible count of
// bits not divisible by 8.

/* Main Program Loop */

// When compressing a file, the constructor with parameters is called. details in fileInformation and the Huffman Tree
// is created. Afterward, compress is called, which generates the rest of data members (using generate) and writes
// those data members to file.

// When decompressing a file, the default constructor is called to instantiate an initial object. The decompress
// function is then manually called, which first reads from the compressed file to populate data members, and then
// instantiates fileInformation and the Huffman Tree. Lastly, written to file is the original file.

/* Other Implementation Notes */

// The unused .hzip extension is used for the compressed file, and when decompressed, the original file name is appended
// with "-decompressed".

// The Header and File Information Code sections are sectioned into auxiliary classes, HuffmanHeader and FileInformation
// respectively.

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include <string>

#include "HuffmanNode.h"
#include "huffman_tree/components/FileInformation.h"
#include "huffman_tree/components/HuffmanHeader.h"

class HuffmanTree {
public:
    // constructors
    HuffmanTree(std::ifstream& input, const std::string& name, const std::string& extension, const std::string& destination);
    HuffmanTree() = default;

    // main program loop public functions
    std::string compress(std::ifstream& input, const std::string& destination);
    std::string decompress(std::ifstream& input, const std::string& destination);

private:
    // instantiated data members
    HuffmanNode* huffmanTreeRoot{nullptr};
    FileInformation fileInformation{"", ""};

    // data members which are written and read to file
    HuffmanHeader huffmanHeader{0, 0, 0};
    std::string huffmanFileInfoCode{};
    std::string huffmanTreeRepresentation{};
    std::string huffmanCode{};

    // main program loop private functions
    void generate(std::ifstream& input);
    void instantiate();
};


#endif // HUFFMAN_TREE_H
