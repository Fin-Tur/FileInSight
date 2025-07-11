//
// Created by Admin on 11.07.2025.
//

#ifndef FUZZYHASHER_H
#define FUZZYHASHER_H

#include <cstdint>
#include <optional>

#include "tlsh.h"
#include <string>
#include <vector>

struct FileInfo;

class FuzzyHasher {
    private:
    //Computes Fuzzy Hash
    static std::string computeFuzzyHash(const FileInfo& file);
    public:
    //Checks for similarities in two Files
    static bool areFilesSimilar(const FileInfo& file1, const FileInfo& file2, const std::uint8_t& cutOff = 35);
    //Checks group of files for similarities to a file
    static std::optional<std::vector<FileInfo>> findSimilarities(const FileInfo& referenceFile, const std::vector<FileInfo>& files, const std::uint8_t& cutOff = 35);


};



#endif //FUZZYHASHER_H
