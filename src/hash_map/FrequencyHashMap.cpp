// Frequency Hash Map Implementation

#include "./hash_map/FrequencyHashMap.h"

// recursive helper function that inserts node in the manner of a BST
void insertBST(FrequencyHashNode*& root, char key) {
    // where node with key does not exist in the bucket, create new node
    if (root == nullptr) {
        FrequencyHashNode* newPtr{new FrequencyHashNode(key)};
        root = newPtr;
        return;
    }

    // where node with key already exists, just increment the frequency instead of creating new node
    // else traverse tree recursively
    if (root->key == key) {
        ++root->frequency;
    } else if (key < root->key) {
        insertBST(root->left, key);
    } else {
        insertBST(root->right, key);
    }
}

void FrequencyHashMap::insertHashNode(const char key) {
    std::size_t bucketIndex{hash(key) % buckets.size()}; // get index hash of the key
    insertBST(buckets[bucketIndex], key);
}
