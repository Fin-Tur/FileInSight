#pragma once
#include <cstdint>
#include <filesystem>

struct FileInfo {
    std::filesystem::path path;
    size_t size;
    std::string hash;
    int lastUsed = -1; // in days
    double entropy = -1;
};

