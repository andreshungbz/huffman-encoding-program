// File Utilities Header

// This module defines a number of functions for getting information about a file on the system. Two implementations
// are accounted for depending on whether the system from which this project is compiled on supports the <filesystem>
// library in the C++ 17 standard. The MinGW compiler on the school computers support the C++ 17 standard, but not
// <filesystem>, so POSIX functions were found as a fallback alternative. Platform-specific code is used: on Windows
// systems _fullpath() and on Linux/macOS realpath().

// Preprocessor directives are used to manage this code, with use of __has_defined to check for <filesystem> and
// _WIN32 to check for compilation on Windows systems.

// These utilities can process both relative and absolute file paths.

#ifndef FILE_UTILS_H
#define FILE_UTILS_H


#include <string>

std::string getFileName(const std::string& path);
std::string getFileExtension(const std::string& path);
std::size_t getFileSize(const std::string& path);
std::string getDirectory(const std::string& path);


#endif // FILE_UTILS_H
