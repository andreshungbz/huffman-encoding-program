#include <fstream>
#include <iostream>

#include "hash_map/FrequencyHashMap.h"
#include "priority_queue/PriorityQueue.h"

// helper functions

void traverseBST(PriorityQueue& queue, const FrequencyHashNode* root) {
    // base case
    if (root == nullptr) {
        return;
    }

    // traverse inorder, going through every node and populating the queue
    queue.enqueue(root->key, root->frequency);
    traverseBST(queue, root->left);
    traverseBST(queue, root->right);
}

void populateQueue(PriorityQueue& queue, const FrequencyHashMap& hashMap) {
    for (FrequencyHashNode* tree : hashMap.buckets) {
        traverseBST(queue, tree);
    }
}

int main() {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{"../test/input.txt", std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "File Open Error\n";
        return 1;
    }

    // create frequency hash map with specified number of buckets
    FrequencyHashMap hashMap{10};

    // read every character and insert into frequency hash map
    char character;
    while (input.get(character)) { // .get() evaluates false after reading invalid character
        hashMap.insertHashNode(character);
    }

    // create priority queue
    PriorityQueue queue{};

    // populate queue with every key in hash map
    populateQueue(queue, hashMap);

    return 0;
}
