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

// [Header] > [File Information] > [Tree Representation] > [Huffman Code]

// First is a Header section containing three 32-bit unsigned integer values
// corresponding to the true byte lengths of the other three sections. Other strategies for section delimiting
// include tagging and using special character sequences but a header section is more convenient.

// Second is the File Information section which is simply the ASCII byte sequence for the file name and extension
// inclusive of the period. This can be seen using a hex dump tool such as xxd on Linux/macOS. This section will
// always be in byte chunks so no padding is necessary.

// Third is the Tree Representation section. To represent the tree, the following algorithm is used: traverse the
// tree in a preorder manner and for every non-leaf node (with no value), record a 1; for every leaf node (which has
// a value), record a 0 then the 8-bit representation of node's value. Weight is not needed in the reconstruction of
// the Huffman Tree, therefore it is not stored. Because this section can result in a count of bits not divisible by 8
// (as computers typically read), padding of 0s may be added at the end.

// Last is the Huffman Code section. This section may also have a padding of 0s at the end due to possible count of
// bits not divisible by 8.

/* Program Loop */

// This implementation has 4 main functions, represented in the public methods in this class.

// 1) generate takes a path to a file and constructs the Huffman Tree and other necessary data members.
// 2) compress writes the file according to the 4 sections above.
// 3) decompress reads a compressed file according to the 4 sections above.
// 4) instantiate reconstructs the Huffman Tree, from which the original file can then be recreated.

/* Other Implementation Notes */

// The encoding table used to create the Huffman Code utilizes a hash map. The C++ STL unordered_map is used for
// convenience. Two separate classes are utilized to group related data members: FileInformation and HuffmanHeader,
// which correspond to sections 2 and 1 of the written file respectively.

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include <string>

#include "HuffmanNode.h"
#include "huffman_tree/components/FileInformation.h"
#include "huffman_tree/components/HuffmanHeader.h"

class HuffmanTree {
public:
    // constructors
    explicit HuffmanTree(std::ifstream& input, const std::string& source); // used when creating an instance from which to compress
    HuffmanTree() = default; // default constructor used when creating an empty object from which to decompress

    // main functions, they are in order of the program loop
    void generate(std::ifstream& input);
    void compress(const std::string& destination) const;
    void decompress(std::ifstream& input, const std::string& destination);
    void instantiate();

private:
    // instantiated data members
    HuffmanNode* huffmanTreeRoot{nullptr};
    FileInformation fileInformation{"", ""};

    // data members which are written and read to file
    HuffmanHeader huffmanHeader{0, 0, 0};
    std::string huffmanFileInfoCode{};
    std::string huffmanTreeRepresentation{};
    std::string huffmanCode{};
};


#endif // HUFFMAN_TREE_H
