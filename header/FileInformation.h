#ifndef FILE_INFORMATION_H
#define FILE_INFORMATION_H


#include <string>
#include <utility>

class FileInformation {
public:
    FileInformation(std::string name, std::string extension, const std::size_t size) : fileName(std::move(name)),
        fileExtension(std::move(extension)), fileSize(size) {}

private:
    std::string fileName{};
    std::string fileExtension{};
    std::size_t fileSize{};
};


#endif // FILE_INFORMATION_H
