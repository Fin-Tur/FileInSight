#pragma once
#include <string>


struct ExtFileInfo {
    size_t size;
    std::string type;
    double entropy;
    std::string tlsh_hash;
    bool flagged;
    int last_used_days;

};