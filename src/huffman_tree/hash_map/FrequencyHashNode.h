// Frequency Hash Node Header and Implementation

// There are the nodes used for chaining in the Frequency Hash Map. Pointers to the left and right are provided
// so that a BST can be created.

#ifndef FREQUENCY_HASH_NODE_H
#define FREQUENCY_HASH_NODE_H


class FrequencyHashNode {
public:
    explicit FrequencyHashNode(char value) : key(value) {} // constructor

    // public data members are fine
    char key{}; // un-hashed key for comparisons
    int frequency{1};
    FrequencyHashNode* left{nullptr};
    FrequencyHashNode* right{nullptr};
};


#endif // FREQUENCY_HASH_NODE_H
