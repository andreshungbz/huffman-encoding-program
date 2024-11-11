#include "FileUtils.h"

#if __has_include(<filesystem>)
    #include <filesystem>
    #define USE_FILESYSTEM 1
#else
    #define USE_FILESYSTEM 0
#endif

#if USE_FILESYSTEM // functions using <filesystem>

std::size_t getFileSize(const std::string& path) {
    return std::filesystem::file_size(path);
}

std::string getDirectory(const std::string& path) {
    std::filesystem::path filePath{path};
    return std::filesystem::canonical(filePath.parent_path()).string();
}

std::string getFileName(const std::string& path) {
    std::filesystem::path filePath{path};
    return filePath.stem().string();
}

std::string getFileExtension(const std::string& path) {
    std::filesystem::path filePath{path};
    return filePath.extension().string();
}

#else // functions using POSIX

std::size_t getFileSize(const std::string& path) {
    struct stat statBuf{};
    if (stat(path.c_str(), &statBuf) != 0) {
        return 0;
    }
    return statBuf.st_size;
}

std::string getDirectory(const std::string& path) {
    char absPath[PATH_MAX];
    if (realpath(path.c_str(), absPath) == nullptr) {
        return "";
    }
    std::string fullPath(absPath);
    size_t pos = fullPath.find_last_of("/\\");
    return (std::string::npos == pos) ? "" : fullPath.substr(0, pos);
}

std::string getFileName(const std::string& path) {
    size_t start = path.find_last_of("/\\") + 1;
    size_t end = path.find_last_of('.');
    return path.substr(start, end - start);
}

std::string getFileExtension(const std::string& path) {
    size_t pos = path.find_last_of('.');
    return (std::string::npos == pos) ? "" : path.substr(pos);
}

#endif