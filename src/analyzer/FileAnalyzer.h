
#pragma once
#include <optional>
#include <string>

#include "../models/ExtFileInfo.h"

namespace FileAnalyzer {

    void analyze(const std::string& src);
    [[nodiscard]] std::optional<ExtFileInfo> analyzeExtended(const std::string& src);

};


