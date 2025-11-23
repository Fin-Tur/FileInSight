#pragma once
#include <string>
#include <utility>

#include "AbstractFinder.h"


class AgingFileFinder : public AbstractFinder{
public:
    explicit AgingFileFinder(std::vector<FileInfo> files, const int& lastUsedCriteria) : lastUsedCriteria(lastUsedCriteria) , files(std::move(files)){};
    [[nodiscard]] std::vector<std::vector<FileInfo>> find() override;
private:
    const int lastUsedCriteria;
    std::vector<FileInfo> files;
};
