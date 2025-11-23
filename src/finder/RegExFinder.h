
#pragma once

#include <utility>

#include "AbstractFinder.h"
#include "../core/FileCollecter.h"

class RegExFinder final : public AbstractFinder{
    std::vector<FileInfo> files;
    std::string pattern;
public:
    explicit RegExFinder(const std::vector<FileInfo>& files, std::string  pattern) : files(files), pattern(std::move(pattern)) {};
    [[nodiscard]] std::vector<std::vector<FileInfo>> find() override;
};

