//
// Created by Admin on 11.07.2025.
//

#ifndef FUZZYHASHER_H
#define FUZZYHASHER_H

#include <cstdint>

#include "tlsh.h"
#include <string>

struct FileInfo;

class FuzzyHasher {
    private:
    //Computes Fuzzy Hash
    static std::string computeFuzzyHash(const FileInfo& file);
    public:
    //Looks for similarities in Files
    static bool areFilesSimilar(const FileInfo& file1, const FileInfo& file2, const std::uint8_t& cutOff = 35);

};



#endif //FUZZYHASHER_H
