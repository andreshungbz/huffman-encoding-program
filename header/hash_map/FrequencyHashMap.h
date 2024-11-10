// Frequency Hash Map Header

// The first step of the Huffman Coding algorithm is to create a frequency table of every unique character.
// The hash map data structure is used to record these values, as they can dynamically resize and offer
// an average O(1) time complexity for insertion/deletion/search. The C++ STL <unordered_map> can be used instead,
// but the hash map is only used for insertion, therefore it has been implemented accordingly.

// In this implementation, buckets and chaining are used to account for collisions. An array (vector) of
// pointers to FrequencyHashNode are stored. The chaining of each node is implemented as a Binary Search Tree (BST)
// rather than a standard Linked List. This makes the insertions on the chain O(log base 2 of N).

#ifndef FREQUENCY_HASHMAP_H
#define FREQUENCY_HASHMAP_H



#include <functional> // std::hash object already provides a rather performant hash function to use
#include <iostream>
#include <vector>

#include "FrequencyHashNode.h"

class FrequencyHashMap {
public:
    explicit FrequencyHashMap(int bucketsNumber) : buckets(bucketsNumber) {} // constructor
    void insertHashNode(char key);
private:
    std::vector<FrequencyHashNode*> buckets;
    std::hash<char> hash; // hash object
};



#endif // FREQUENCY_HASHMAP_H
