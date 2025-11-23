

#include "MagicByteAnalyzer.h"

#include <algorithm>
#include <iostream>
#include <fstream>

#include "../core/FileCollecter.h"


std::optional<std::pair<FileInfo, std::string>> MagicByteAnalyzer::analyzeFile(const std::filesystem::path& path) {
    //Check src
    if (!(std::filesystem::exists(path) && std::filesystem::is_regular_file(path))) {
        std::cerr << "[Error] " << path << " doesn't exist or isn't a regular file!\n" << std::endl;
        return std::nullopt;
    }
    //Open FIle
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "[Error] " << path << " could not be opened!\n" << std::endl;
        return std::nullopt;
    }
    //Read first 8 bytes
    unsigned char magicBytes[8];
    ifs.read(reinterpret_cast<char *>(magicBytes), 8);
    if (ifs.gcount() != 8) {
        std::cerr << "[Error] " << path << " could not be read!\n" << std::endl;
        return std::nullopt;
    }
    ifs.close();

    //Compare to byte signatures and flag files
    std::stringstream ss;
    for (unsigned char magicByte : magicBytes) {
        ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(magicByte);
    }
    for (auto& [signature, filetype] : byteSignatures) {
        if (ss.str().starts_with(signature)) {
            std::string extension = std::filesystem::path(path).extension().string();
            if (!extension.empty() && extension[0] == '.') {
                extension = extension.substr(1); // remove dot
            }
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
            if (filetype != extension) {
                return std::make_pair(FileInfo(path), filetype);
            }
        }
    }
    return std::nullopt;
}

std::vector<std::pair<FileInfo, std::string> > MagicByteAnalyzer::analyzePath(const std::string &path) {
    //Get Files
    std::vector<FileInfo> files = FileCollector::collect(path);
    if (files.empty()) {
        std::cerr << "[Error] " << path << " could not be collected!\n" << std::endl;
    }
    //Analyze Files
    std::vector<std::pair<FileInfo, std::string>> flaggedFiles;
    for (auto& file : files) {
        std::optional<std::pair<FileInfo, std::string>> res = analyzeFile(file.path);
        if (res) {
            flaggedFiles.push_back(*res);
        }
    }
    return flaggedFiles;
}
