#pragma once

#include <vector>
#include "../models/FileInfo.h"

class AbstractFinder {
    public:
    virtual std::vector<std::vector<FileInfo>> find() = 0;
    virtual ~AbstractFinder() noexcept = default;
};


