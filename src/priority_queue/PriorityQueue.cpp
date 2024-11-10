// Priority Queue Implementation

#include <algorithm>

#include "priority_queue/PriorityQueue.h"

void PriorityQueue::reHeapUp(std::size_t endIndex) {
    if (endIndex > 0) { // base case is at root node
        std::size_t parentIndex{getParent(endIndex)};

        // swap and continue reHeapUp as necessary
        if (queue[parentIndex]->weight > queue[endIndex]->weight) {
            std::swap(*queue[parentIndex], *queue[endIndex]);
            reHeapUp(parentIndex);
        }
    }
}

void PriorityQueue::enqueue(char key, int weight) {
    queue.push_back(new HuffmanNode(key, weight));
    reHeapUp(queue.size() - 1);
}

void PriorityQueue::reHeapDown(std::size_t startIndex, std::size_t endIndex) {
    std::size_t leftChildIndex{getLeftChild(startIndex)};
    std::size_t rightChildIndex{getRightChild(startIndex)};

    if (leftChildIndex <= endIndex) { // base case is when left index is past the last vector index
        std::size_t minChildIndex;

        if (leftChildIndex == endIndex) { // if left child is the last node, set it as the min
            minChildIndex = leftChildIndex;
        } else { // set min child accordingly
            minChildIndex = queue[rightChildIndex]->weight < queue[leftChildIndex]->weight
            ? rightChildIndex : leftChildIndex;
        }

        // swap and continue reHeapDown as necessary
        if (queue[startIndex]->weight > queue[minChildIndex]->weight) {
            std::swap(*queue[startIndex], *queue[minChildIndex]);
            reHeapDown(minChildIndex, endIndex);
        }
    }
}

void PriorityQueue::dequeue() {
    // check if empty
    if (queue.empty()) {
        return;
    }

    queue[0] = queue[queue.size() - 1]; // assign last node to root
    queue.pop_back(); // delete last node
    reHeapDown(0, queue.size() - 1);
}
