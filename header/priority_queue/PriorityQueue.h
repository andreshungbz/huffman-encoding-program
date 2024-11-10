// Priority Queue Header

// The second step of Huffman Coding algorithm is to arrange the nodes such that every successive pair of nodes
// with the lowest weights become the children of a new node with the combined weight, which is added back to the queue.
// The Huffman Tree is created when all nodes are combined and the priority queue only has one node at the end. The
// pointer to the root of the Huffman Tree is later used in another class.

// In this implementation, a priority queue would be appropriate, with the nodes having the minimum weights being
// dequeued first for the construction of the Huffman Tree. A minimum heap is used to implement the priority queue,
// as it offers a guaranteed O(log base 2 of N) for enqueue and dequeue operations. (For a BST, it is the same when
// the tree is balanced but O(N) for both when skewed; for a linked list enqueue is O(N) and dequeue is O(1).)

// The minimum heap itself is implemented as an array (vector). The vector STL class provides a number of useful
// methods for the implementation of the heap. Pointers to HuffmanNode objects are stored in the vector so that
// the final pointer with the Huffman Tree can be copied while keeping the objects intact.

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H


#include <vector>

#include "huffman_tree/HuffmanNode.h"

class PriorityQueue {
public:
    void enqueue(char key, int weight);
    HuffmanNode* dequeue();
    void enqueue(HuffmanNode* node); // overload function for constructHuffmanTree
    void constructHuffmanTree(); // post-condition: single pointer in queue representing root of Huffman Tree
    [[nodiscard]] HuffmanNode* getHuffmanTree() const { return queue[0]; } // called after construction of Huffman Tree
private:
    std::vector<HuffmanNode*> queue{};

    // these are helper functions not meant to be used outside the class, so they are private
    // use implicit links to get appropriate indexes
    static std::size_t getParent(std::size_t index) { return (index - 1) / 2; }
    static std::size_t getLeftChild(std::size_t index) { return (index * 2) + 1; }
    static std::size_t getRightChild(std::size_t index) { return (index * 2) + 2; }
    // reHeap functions
    void reHeapUp(std::size_t endIndex);
    void reHeapDown(std::size_t startIndex, std::size_t endIndex);
};


#endif // PRIORITY_QUEUE_H
