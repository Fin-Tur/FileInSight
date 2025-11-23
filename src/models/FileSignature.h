#pragma once
#include <cstdint>
#include <string>
#include <vector>

//Data container for file signatures
struct FileSignature {

    std::vector<uint8_t> header;
    std::vector<uint8_t> footer;
    std::string name;

};

