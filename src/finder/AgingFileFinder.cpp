//
// Created by Admin on 05.07.2025.
//

#include "AgingFileFinder.h"

#include "../core/FileCollecter.h"

std::vector<std::vector<FileInfo> > AgingFileFinder::find() {
    std::vector<std::vector<FileInfo>> wrapped;
    std::vector<FileInfo> result;

    for (auto& file : this->files) {
        if (file.lastUsed > this->lastUsedCriteria) {
            result.emplace_back(file);
        }
    }

    wrapped.emplace_back(result);
    return wrapped;
}
