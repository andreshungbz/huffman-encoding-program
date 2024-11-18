// File Information Header and Implementation

// This class consolidates information about the processed file and provides a simple constructor for
// instantiating an object. The data members written to file will be the byte size of the ASCII representation
// of the file name and file extension inclusive of the period.

#ifndef FILE_INFORMATION_H
#define FILE_INFORMATION_H


#include <string>
#include <utility>

class FileInformation {
public:
    // constructor
    FileInformation(std::string name, std::string extension)
    : fileName(std::move(name)), fileExtension(std::move(extension)) {}

    // public data members
    std::string fileName{};
    std::string fileExtension{};
};


#endif // FILE_INFORMATION_H
