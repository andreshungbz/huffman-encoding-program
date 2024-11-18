#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "huffman_tree/HuffmanTree.h"
#include "utils/file/file_utils.h"

// main driver functions
void driver();
void compress();
void decompress();
void displayAbout();
// helper functions for driver
void printMenu();
void printCompressionResult(const std::string& path, int oSize, int cSize);
int promptMenuResponse();
std::string promptFilePath();


int main() {
    driver();
    return 0;
}

// main driver functions

void driver() {
    while (true) {
        printMenu();
        int menuResponse(promptMenuResponse());

        switch (menuResponse) {
        case 1:
            compress();
            break;
        case 2:
            decompress();
            break;
        case 3:
            displayAbout();
            break;
        case 4:
            return;
        default: ;
        }
    }
}

void compress() {
    std::cout << "\n[Compress a File]\n";

    // get file path
    std::string filePath{promptFilePath()};
    // open the uncompressed file
    std::ifstream input{filePath, std::ios::in | std::ios::binary}; // read in binary mode
    if (!input) {
        std::cout << "\nError: Failed to read file. Recheck file name and path.\n";
        return;
    }

    // retrieve information about the file
    std::string fileName{getFileName(filePath)};
    std::string fileExtension{getFileExtension(filePath)};
    std::string directory{getDirectory(filePath)};

    // construct Huffman Tree, compress the file, and write .hzip file to the same directory as original file
    HuffmanTree huffmanTree{input, fileName, fileExtension, directory};
    std::string compressedFilePath{huffmanTree.compress(input, directory)};

    // retrieve size information about the original file and the compressed file
    int originalSize{static_cast<int>(getFileSize(filePath))};
    int compressedSize{static_cast<int>(getFileSize(compressedFilePath))};

    // print compression result
    printCompressionResult(compressedFilePath, originalSize, compressedSize);
}

void decompress() {
    std::cout << "\n[Decompress a .hzip File]\n";

    // get file path
    std::string filePath{promptFilePath()};

    // exit if provided file does not end with .hzip
    // use C++17 compatible method: https://stackoverflow.com/a/42844629
    std::string extension{".hzip"};
    if (!(filePath.size() >= extension.size() &&
        filePath.compare(filePath.size() - extension.size(), extension.size(), extension) == 0)) {
        std::cout << "Error: File provided does not have the .hzip extension.\n";
        }

    // open the uncompressed file
    std::ifstream input{filePath, std::ios::in | std::ios::binary}; // read in binary mode
    if (!input) {
        std::cout << "\nError: Failed to read compressed file. Recheck file name and path.\n";
        return;
    }

    // get directory of compressed file
    std::string directory{getDirectory(filePath)};

    // read the file, instantiate Huffman Tree, and write original file to the same directory as the .hzip file
    HuffmanTree decompressHuffmanTree{};
    std::string decompressedFilePath{decompressHuffmanTree.decompress(input, directory)};

    // retrieve size information about the compressed file and the decompressed file
    int compressedSize{static_cast<int>(getFileSize(filePath))};
    int decompressedSize{static_cast<int>(getFileSize(decompressedFilePath))};

    // print compression result
    printCompressionResult(decompressedFilePath, decompressedSize, compressedSize);
}

void displayAbout() {
    std::cout << std::endl;

    std::cout << "[About Huffman Encoding Compression Program]\n";
    std::cout << std::left << std::setw(24) << "[About Algorithm]" << "https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1234/resources/huffman.html\n";
    std::cout << std::left << std::setw(24) << "[Source Code]" << "https://github.com/andreshungbz/CMPS2131/tree/main/final-project/02-huffman-encoding\n";
    std::cout << std::left << std::setw(24) << "[Code Author]" << "Andres Hung\n";
    std::cout << std::left << std::setw(24) << "[Contact]" << "201811824@ub.edu.bz or andres.hung@outlook.com\n";
    std::cout << std::left << std::setw(24) << "[Course]" << "[CMPS2131] Data Structures @ University of Belize\n";
    std::cout << std::left << std::setw(24) << "[Code Completion Date]" << "November 18, 2024";

    std::cout << std::endl;
}

// helper functions for driver

void printMenu() {
    std::cout << std::endl;

    std::cout << "[Huffman Encoding Compression Program]\n";
    std::cout << "1) Compress a File\n";
    std::cout << "2) Decompress an .hzip File\n";
    std::cout << "3) About Program\n";
    std::cout << "4) Exit Program\n";

    std::cout << std::endl;
}

int promptMenuResponse() {
    while (true) {
        int response;
        std::cout << "Enter Menu Option Number: ";
        std::cin >> response;

        // basic input validation
        // https://www.learncpp.com/cpp-tutorial/stdcin-and-handling-invalid-input/

        // check failed extraction (e.g. user inputs a character instead of a number)
        if (!std::cin) {
            std::cin.clear(); // set back to normal mode
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
            std::cout << "\nError: Number was not entered. Try again.\n";
            continue;
        }

        // also clear buffer on successful extraction (e.g. user inputs something like "1b")
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // check valid menu option range
        if (response < 1 || response > 4) {
            std::cout << "\nError: Number is not in valid range of options. Try again\n";
            continue;
        }

        return response;
    }
}

std::string promptFilePath() {
    std::string response;
    std::cout << "Enter File Path: ";
    std::getline(std::cin >> std::ws, response); // std::ws ignores leading whitespace

    return response;
}

void printCompressionResult(const std::string& path, int oSize, int cSize) {
    // determine compression percentage, accounting for negative when compressed file ends up larger
    bool negative{cSize > oSize};
    double max{std::max(oSize, cSize)};
    double min{std::min(oSize, cSize)};
    double compressionPercentage{((max - min) / oSize * 100) * (negative ? -1 : 1)};

    // message snippet
    std::string message{negative ? "(size increase [!])" : "(size decrease ✅)"};

    std::cout << std::endl;

    std::cout << "[Compression Result]\n";
    std::cout << std::left << std::setw(20) << "[Location] " << path << '\n';
    std::cout << std::left << std::setw(20) << "[Original Size] " << oSize << " bytes\n";
    std::cout << std::left << std::setw(20) << "[Compressed Size] " << cSize << " bytes\n";
    std::cout << std::left << std::setw(20) << "[Compression %] " << std::fixed << std::setprecision(2)
        << compressionPercentage << "% " + message + '\n';
}
