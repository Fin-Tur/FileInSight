#pragma once
#include <string>

#include "AbstractFinder.h"
#include "../models/FileInfo.h"


class SimilarityFinder final : public AbstractFinder{
private:
    std::vector<FileInfo> referenceFiles;
    std::vector<FileInfo> files;
public:
    SimilarityFinder(const std::string& referencePath, const std::string& searchPath);
    [[nodiscard]] std::vector<std::vector<FileInfo>> find() override;
};


