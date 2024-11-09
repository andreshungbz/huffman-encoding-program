#include <fstream>
#include <iostream>

int main() {
    // open test file in binary mode to read file exactly as is stored
    std::ifstream input{"../test/input.txt", std::ios::in | std::ios::binary};
    // handle file open error
    if (!input.is_open()) {
        std::cout << "File Open Error\n";
        return 1;
    }

    // read every character and print it
    char character;
    while (input.get(character)) { // .get() evaluates false after reading invalid character
        std::cout << character;
    }

    return 0;
}
