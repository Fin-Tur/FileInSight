

#include <filesystem>
#include <iostream>

#include "FileAnalyzer.h"
#include "EntropyAnalyzer.h"
#include "MagicByteAnalyzer.h"
#include "../core/FileCollecter.h"
#include "../UI/UI.h"
#include <hashing/FuzzyHasher.h>


void FileAnalyzer::analyze(const std::string& src) {
    //gather File(Info)
    std::optional<ExtFileInfo> fileExtInfoOpt = analyzeExtended(src);
    if (!fileExtInfoOpt.has_value()) {
        std::cerr << "[Error] Failed to analyze file: " << src << "\n";
        return;
    }
    ExtFileInfo fileExtInfo = fileExtInfoOpt.value();

    long actualInformationSize = (fileExtInfo.entropy/8) * fileExtInfo.size;

    //Print Information
    std::cout << "[Info] Information for file @ " << src << "\n";
    std::cout << "  -> Size: " << UI::convertBytes(fileExtInfo.size) << "\n";
    std::cout << "  -> type: " << fileExtInfo.type << "\n";
    if (fileExtInfo.flagged) {
        std::cout << "    -> Flagged as: " << fileExtInfo.type << "\n";
    }
    std::cout << "  -> Entropy: " << fileExtInfo.entropy << "\n";
    std::cout << "  -> Actual Amount of information: " << UI::convertBytes(actualInformationSize) << "\n";
    if (fileExtInfo.entropy < 6.5) {
        std::cout << "    -> Could free memory with compression!\n";
    }else if (fileExtInfo.entropy > 7.8) {
        std::cout << "    -> Could be encrypted file!\n";
    }
    std::cout << "  -> Last Used: " << fileExtInfo.last_used_days << " days ago!\n";


}

std::optional<ExtFileInfo> FileAnalyzer::analyzeExtended(const std::string& src){
    //gather File(Info)
    if (!std::filesystem::exists(src)) {
        std::cerr << "[Error] Unknown File!\n";
        return std::nullopt;
    }
    if (!std::filesystem::is_regular_file(src)) {
        std::cerr << "[Error] Not a regular file!\n";
        return std::nullopt;
    }

    FileInfo file = FileCollector::collect(src)[0];
    //Gather Information
    file.size = std::filesystem::file_size(src);

    file.entropy = EntropyAnalyzer::analyze(src);

    std::string fileExtension = std::filesystem::path(src).extension().string();
    std::optional<std::pair<FileInfo, std::string>> byteFlag = MagicByteAnalyzer::analyzeFile(src);

    std::string tlsh_hash = FuzzyHasher::computeFuzzyHash(file);

    return ExtFileInfo{
        .size = file.size,
        .type = byteFlag.has_value() ? byteFlag->second : fileExtension,
        .entropy = file.entropy,
        .tlsh_hash = tlsh_hash,
        .flagged = byteFlag.has_value(),
        .last_used_days = file.lastUsed
    };

}