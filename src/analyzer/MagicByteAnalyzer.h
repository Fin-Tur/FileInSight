//
// Created by Admin on 11.07.2025.
//

#ifndef MAGICBYTEANALYZER_H
#define MAGICBYTEANALYZER_H

#include <vector>

#include "../models/FileInfo.h"

const std::vector<std::pair<std::string, std::string>> byteSignatures = {
    {"89504E47", "png"},
    {"4D5A", "exe"},
    {"25504446", "pdf"},
    {"504B0304", "zip"},
    {"FFD8FF", "jpg"},
    {"47494638", "gif"},
    {"377ABCAF", "7z"},
    {"52617221", "rar"}
};

class MagicByteAnalyzer {
private:
    std::vector<FileInfo> files;
    std::vector<FileInfo> flaggedFiles;
public:
    explicit MagicByteAnalyzer(std::string& path);
    void analyze();
};



#endif //MAGICBYTEANALYZER_H
