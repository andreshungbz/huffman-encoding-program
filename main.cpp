#include <fstream>
#include <iostream>

#include "./hash_map/FrequencyHashMap.h"

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

    // read every character
    char character;
    while (input.get(character)) { // .get() evaluates false after reading invalid character
        hashMap.insertHashNode(character);
    }

    return 0;
}
