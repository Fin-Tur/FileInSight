#pragma once

#include <optional>
#include <vector>

#include "../models/FileInfo.h"


namespace MagicByteAnalyzer {

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


//Analyze Path for flaggedFiles
[[nodiscard]] std::vector<std::pair<FileInfo, std::string>> analyzePath(const std::string& path);
//Analyze File
[[nodiscard]] std::optional<std::pair<FileInfo, std::string>> analyzeFile(const std::filesystem::path& path);
//Check raw bytes for flagged signatures
[[nodiscard]] bool checkFlagBytes(const unsigned char* data, const size_t data_size, std::string expected_type) noexcept;

};



