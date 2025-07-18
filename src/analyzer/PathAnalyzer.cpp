//
// Created by Admin on 16.07.2025.
//

#include "PathAnalyzer.h"

#include <algorithm>

#include "../UI/UI.h"
#include <iostream>


void PathAnalyzer::analyze() {
    for (auto& file : this->files) {
        std::filesystem::path extension = file.path.extension();
        this->fullSize += file.size;
        this->fileTypeSizes[extension] += file.size;

        auto relative = std::filesystem::relative(file.path, this->path);
        if (relative.has_parent_path()) {
            auto topLevel = this->path / *relative.begin();  // eg /path/subdir
            subfolderSizes[topLevel] += file.size;
        }

    }
    this->averageFileSize = this->files.empty() ? 0 : this->fullSize / this->files.size();

}

void PathAnalyzer::printAnalytics() const {

    std::vector<std::pair<std::filesystem::path, size_t>> sortedFileTypSizes(this->fileTypeSizes.begin(), this->fileTypeSizes.end());
    std::sort(sortedFileTypSizes.begin(), sortedFileTypSizes.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::cout << "\n[Info] Analytics for " << this->path.string() <<"\n";
    std::cout << "  -> Number of files: " << this->files.size() << "\n";
    std::cout << "  -> Total Memory allocated: " << UI::convertBytes(this->fullSize) << "\n";
    std::cout << "  -> Average file size: " << UI::convertBytes(this->averageFileSize) << "\n";
    std::cout << "  -> Subfolder memory occupation: \n";
    for (auto& [subdir, size] : this->subfolderSizes) {
        std::cout << "   -> Subfolder " << subdir << " size: " << UI::convertBytes(size) << "\n";
    }
    std::cout << "  -> Memory occupation by file types:\n";
    for (auto& [filetype, size] : sortedFileTypSizes) {
        std::cout << "    -> File type: " << filetype << " size:" << UI::convertBytes(size) << "\n";
    }
}
