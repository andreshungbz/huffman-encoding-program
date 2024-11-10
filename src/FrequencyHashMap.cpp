#include "FrequencyHashMap.h"

void insertBST(FrequencyHashNode*& root, char key) {
    // where node with key does not exist in the bucket, create new node
    if (root == nullptr) {
        FrequencyHashNode* newPtr{new FrequencyHashNode(key)};
        root = newPtr;
        return;
    }

    // where node with key already exists, just increment the frequency
    if (root->key == key) {
        ++root->frequency;
    } else if (key < root->key) {
        insertBST(root->left, key);
    } else {
        insertBST(root->right, key);
    }
}

void FrequencyHashMap::insertHashNode(const char key) {
    // std::hash object already provides a rather performant hash function
    std::size_t bucketIndex{hash(key) % buckets.size()};
    insertBST(buckets[bucketIndex], key);
}
