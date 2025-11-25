#pragma once
#include <cstdint>
#include <optional>

#include "../thirdparty/fuzzyhashLib/tlsh.h"
#include <string>
#include <vector>

struct FileInfo;

namespace FuzzyHasher {
    //Computes Fuzzy Hash
    [[nodiscard]] std::string computeFuzzyHash(const FileInfo& file);
    //Checks for similarities in two Files
    bool areFilesSimilar(const FileInfo& file1, const FileInfo& file2, const std::uint8_t& cutOff = 45);
    //Checks group of files for similarities to a file
    [[nodiscard]] std::optional<std::vector<FileInfo>> findSimilarities(const FileInfo& referenceFile, const std::vector<FileInfo>& files, const std::uint8_t& cutOff = 35);


};

