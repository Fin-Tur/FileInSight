#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "../models/FileInfo.h"




//Collects files from directory recursively
namespace FileCollector {

    [[nodiscard]] static std::vector<FileInfo> collect(const std::string& path);
    static bool isPathBlacklisted(const std::string& path);

    //BlackListing secured Paths
    const std::vector<std::string> blacklist = {
        "C:/Windows",
        "C:/ProgramData",
        "C:/Recovery"
    };
};


