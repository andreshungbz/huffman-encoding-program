// Driver Program Function Declarations

// The driver program consolidates usage of the HuffmanTree class, providing a command line interface for compressing
// and decompressing files when the executable is run. It consists of a variety of helper functions for printing menus,
// getting input from the user, and displaying processed statistics.

// Functions in the File Utilities header are used. Additionally, basic input validation and error handling is
// implemented for user prompts and file opens.

// For the file path prompts, both relative and absolute file paths should work.

#ifndef DRIVER_H
#define DRIVER_H


#include <string>

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


#endif // DRIVER_H
