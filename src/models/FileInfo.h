//
// Created by Admin on 05.07.2025.
//

#ifndef FILEINFO_H
#define FILEINFO_H

struct FileInfo {
    std::filesystem::path path;
    std::uintmax_t size;
    std::string hash;
    int lastUsed = -1; // in days
};

#endif //FILEINFO_H
