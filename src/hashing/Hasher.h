#pragma once
#include <string>
#include "picosha2.h"

struct FileInfo; // Forward declaration

// Static SHA-256 hash generator
namespace Hasher {
    [[nodiscard]] static std::string computeHash(const FileInfo& file, bool trueHash);
};

