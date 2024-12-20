// Priority Queue Implementation

#include <algorithm>

#include "PriorityQueue.h"

PriorityQueue::PriorityQueue(const FrequencyHashMap& hashMap) {
    for (FrequencyHashNode* tree : hashMap.buckets) {
        traverseBST(tree);
    }

    // construct Huffman Tree
    constructHuffmanTree();
}

// constructor helper functions

void PriorityQueue::traverseBST(const FrequencyHashNode* root) {
    // base case
    if (root == nullptr) {
        return;
    }

    // traverse preorder, going through every node and populating the queue
    enqueue(root->key, root->frequency);
    traverseBST(root->left);
    traverseBST(root->right);
}

void PriorityQueue::constructHuffmanTree() {
    // base case when only one pointer left in queue
    if (queue.size() == 1) {
        return;
    }

    // extract the two most minimum nodes from queue
    HuffmanNode* minimumA{dequeue()};
    HuffmanNode* minimumB{dequeue()};

    // create a non-leaf HuffmanNode with the combined weights
    HuffmanNode* newPtr{new HuffmanNode(minimumA->weight + minimumB->weight)};
    newPtr->left = minimumA;
    newPtr->right = minimumB;

    // add new HuffmanNode back to queue
    enqueue(newPtr);

    // recursively continue
    constructHuffmanTree();
}

// heap functions

void PriorityQueue::reHeapUp(std::size_t endIndex) {
    if (endIndex > 0) {
        // base case is at root node
        std::size_t parentIndex{getParent(endIndex)};

        // swap and continue reHeapUp as necessary
        if (queue[parentIndex]->weight > queue[endIndex]->weight) {
            std::swap(*queue[parentIndex], *queue[endIndex]);
            reHeapUp(parentIndex);
        }
    }
}

void PriorityQueue::reHeapDown(std::size_t startIndex, std::size_t endIndex) {
    std::size_t leftChildIndex{getLeftChild(startIndex)};
    std::size_t rightChildIndex{getRightChild(startIndex)};

    if (leftChildIndex <= endIndex) {
        // base case is when left index is past the last vector index
        std::size_t minChildIndex;

        if (leftChildIndex == endIndex) {
            // if left child is the last node, set it as the min
            minChildIndex = leftChildIndex;
        } else {
            // set min child accordingly
            minChildIndex = queue[rightChildIndex]->weight < queue[leftChildIndex]->weight
                                ? rightChildIndex
                                : leftChildIndex;
        }

        // swap and continue reHeapDown as necessary
        if (queue[startIndex]->weight > queue[minChildIndex]->weight) {
            std::swap(*queue[startIndex], *queue[minChildIndex]);
            reHeapDown(minChildIndex, endIndex);
        }
    }
}

void PriorityQueue::enqueue(char key, int weight) {
    queue.push_back(new HuffmanNode(key, weight));
    reHeapUp(queue.size() - 1);
}

void PriorityQueue::enqueue(HuffmanNode* node) {
    queue.push_back(node);
    reHeapUp(queue.size() - 1);
}

HuffmanNode* PriorityQueue::dequeue() {
    // check if empty
    if (queue.empty()) {
        return nullptr;
    }

    HuffmanNode* dequeuedPtr{queue.front()}; // get dequeued pointer
    queue[0] = queue[queue.size() - 1]; // assign last pointer to root
    queue.pop_back(); // delete last pointer

    std::size_t length{queue.size()}; // check for overflow
    reHeapDown(0, length == 0 ? 0 : length - 1);

    return dequeuedPtr;
}
