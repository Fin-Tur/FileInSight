//
// Created by Admin on 18.07.2025.
//

#ifndef FILESIGNATURE_H
#define FILESIGNATURE_H
#include <cstdint>
#include <string>
#include <vector>

//Data container for file signatures
struct FileSignature {

    std::vector<uint8_t> header;
    std::vector<uint8_t> footer;
    std::string name;

};

#endif //FILESIGNATURE_H
