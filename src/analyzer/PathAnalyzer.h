//
// Created by Admin on 16.07.2025.
//

#ifndef PATHANALYZER_H
#define PATHANALYZER_H
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "../core/FileCollecter.h"

struct FileInfo;

class PathAnalyzer {
public:
    explicit PathAnalyzer(const std::string& path) : path(path), files(FileCollector::collect(path)) {};
    void analyze();
    void printAnalytics() const;

private:
    std::filesystem::path path;
    std::vector<FileInfo> files;
    std::size_t fullSize = 0;
    std::unordered_map<std::filesystem::path, std::size_t> fileTypeSizes;
    std::unordered_map<std::filesystem::path, std::size_t> subfolderSizes;
    std::size_t averageFileSize = 0;
};



#endif //PATHANALYZER_H
