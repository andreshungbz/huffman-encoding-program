#ifndef FILE_UTILS_H
#define FILE_UTILS_H


#include <string>

std::size_t getFileSize(const std::string& path);
std::string getDirectory(const std::string& path);
std::string getFileName(const std::string& path);
std::string getFileExtension(const std::string& path);


#endif //FILE_UTILS_H
