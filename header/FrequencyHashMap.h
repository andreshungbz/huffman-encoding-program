#ifndef FREQUENCYHASHMAP_H
#define FREQUENCYHASHMAP_H



#include <functional>
#include <iostream>
#include <list>
#include <vector>

#include "FrequencyHashNode.h"

class FrequencyHashMap {
public:
    explicit FrequencyHashMap(int bucketsNumber) : buckets(bucketsNumber) {}
    void insertHashNode(char key);
private:
    std::vector<FrequencyHashNode*> buckets;
    std::hash<char> hash;
};



#endif // FREQUENCYHASHMAP_H
