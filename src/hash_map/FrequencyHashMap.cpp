// Frequency Hash Map Implementation

#include "./hash_map/FrequencyHashMap.h"

FrequencyHashMap::FrequencyHashMap(std::ifstream& input, int bucketsCount) : buckets(bucketsCount) {
    input.clear(); // check if error state
    input.seekg(0, std::ios::beg);

    // read every character and insert into frequency hash map
    char character;
    while (input.get(character)) {
        // .get() evaluates false after reading invalid character
        insertHashNode(character);
    }
}

void FrequencyHashMap::insertHashNode(const char key) {
    std::size_t bucketIndex{hash(key) % buckets.size()}; // get index hash of the key
    insertBST(buckets[bucketIndex], key);
}

// recursive helper function that inserts node in the manner of a BST
void FrequencyHashMap::insertBST(FrequencyHashNode*& root, const char key) {
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