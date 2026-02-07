#include "Hasher.h"

#include <iostream>

#include "../finder/DuplicateFinder.h"
#include "../core/FileCollecter.h"

// Compute SHA-256 hash of a file
std::string Hasher::computeHash(const FileInfo& file, bool trueHash) {

    // Security check (DLLs etc.)
    if (DuplicateFinder::isProtected(file.path)) {
        return "";
    }

    std::ifstream ifs(file.path, std::ios::binary);
    if (!ifs.is_open()) {
        throw std::runtime_error("Cannot open file!");
    }

    if (trueHash) {
        return picosha2::hash256_hex_string(std::istreambuf_iterator<char>(ifs), {});
    } else {
        std::vector<char> buffer(16 * 1024 * 1024);  // 16 MB
        ifs.read(buffer.data(), buffer.size());
        std::streamsize readBytes = ifs.gcount();
        if (readBytes <= 0) {
            throw std::runtime_error("File read failed.");
        }
        return picosha2::hash256_hex_string(buffer.begin(), buffer.begin() + readBytes);
    }
}

std::string Hasher::computeHashNonFile(const std::string& chars) {
    return picosha2::hash256_hex_string(chars.begin(), chars.end());
}

