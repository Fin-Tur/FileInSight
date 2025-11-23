

#include <filesystem>
#include <iostream>

#include "FileAnalyzer.h"
#include "EntropyAnalyzer.h"
#include "MagicByteAnalyzer.h"
#include "../core/FileCollecter.h"
#include "../UI/UI.h"


void FileAnalyzer::analyze(const std::string& src) {
    //gather File(Info)
    if (!std::filesystem::exists(src)) {
        std::cerr << "[Error] Unknown File!\n";
    }
    if (!std::filesystem::is_regular_file(src)) {
        std::cerr << "[Error] Not a regular file!\n";
    }

    FileInfo file = FileCollector::collect(src)[0];
    //Gather Information
    file.size = std::filesystem::file_size(src);

    file.entropy = EntropyAnalyzer::analyze(src);
    long actualInformationSize = (file.entropy/8) * file.size;

    std::string fileExtension = std::filesystem::path(src).extension().string();
    std::optional<std::pair<FileInfo, std::string>> byteFlag = MagicByteAnalyzer::analyzeFile(src);

    //Print Information
    std::cout << "[Info] Information for file @ " << src << "\n";
    std::cout << "  -> Size: " << UI::convertBytes(file.size) << "\n";
    std::cout << "  -> type: " << fileExtension << "\n";
    if (byteFlag.has_value()) {
        std::cout << "    -> Flagged as: " << byteFlag->second << "\n";
    }
    std::cout << "  -> Entropy: " << file.entropy << "\n";
    std::cout << "  -> Actual Amount of information: " << UI::convertBytes(actualInformationSize) << "\n";
    if (file.entropy < 6.5) {
        std::cout << "    -> Could free memory with compression!\n";
    }else if (file.entropy > 7.8) {
        std::cout << "    -> Could be encrypted file!\n";
    }
    std::cout << "  -> Last Used: " << file.lastUsed << " days ago!\n";


}
