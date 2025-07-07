#ifndef HASHER_H
#define HASHER_H

#include <string>
#include "picosha2.h"

struct FileInfo; // Forward declaration

// Static SHA-256 hash generator
class Hasher {
public:
    static std::string computeHash(const FileInfo& file, bool trueHash);
};

#endif // HASHER_H
