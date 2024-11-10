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
