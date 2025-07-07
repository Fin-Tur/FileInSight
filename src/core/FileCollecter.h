#ifndef FILECOLLECTER_H
#define FILECOLLECTER_H

#include <filesystem>
#include <string>
#include <vector>
#include "../models/FileInfo.h"
//BlackListing secured Paths
const std::vector<std::string> blacklist = {
    "C:/Windows",
    "C:/ProgramData",
    "C:/Recovery"
};


// Collects files from directory recursively
class FileCollector {
public:
    static std::vector<FileInfo> collect(const std::string& path);
    static bool isPathBlacklisted(const std::string& path);
};

#endif // FILECOLLECTER_H

