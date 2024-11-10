#ifndef FREQUENCYHASHNODE_H
#define FREQUENCYHASHNODE_H



class FrequencyHashNode {
public:
    char key{};
    int frequency{1};
    FrequencyHashNode* left{nullptr};
    FrequencyHashNode* right{nullptr};
    explicit FrequencyHashNode(char value) : key(value) {}
};



#endif // FREQUENCYHASHNODE_H
