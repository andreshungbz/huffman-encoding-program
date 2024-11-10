// Frequency Hash Node Header and Implementation

// There are the nodes used for chaining in the Frequency Hash Map. Pointers to the left and right are provided
// so that a BST can be created.

#ifndef FREQUENCYHASHNODE_H
#define FREQUENCYHASHNODE_H



class FrequencyHashNode {
public:
    char key{}; // un-hashed key for comparisons
    int frequency{1};
    FrequencyHashNode* left{nullptr};
    FrequencyHashNode* right{nullptr};
    explicit FrequencyHashNode(char value) : key(value) {} // constructor
};



#endif // FREQUENCYHASHNODE_H
