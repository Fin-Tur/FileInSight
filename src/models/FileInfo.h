//
// Created by Admin on 05.07.2025.
//

#ifndef FILEINFO_H
#define FILEINFO_H
#include <cstdint>
#include <filesystem>

struct FileInfo {
    std::filesystem::path path;
    size_t size;
    std::string hash;
    int lastUsed = -1; // in days
    double entropy = -1;
};

#endif //FILEINFO_H
