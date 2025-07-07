//
// Created by Admin on 04.07.2025.
//

#include "RegExFinder.h"

#include <iostream>
#include <regex>

std::vector<std::vector<FileInfo>> RegExFinder::find() {
    std::vector<std::vector<FileInfo>> wrapped;
    std::vector<FileInfo> matches;

    try {
        std::regex re(pattern, std::regex::icase);
        for (auto& file : this->files) {
            if (std::regex_search(file.path.filename().string(), re)) {
                matches.emplace_back(file);
            }
        }
    }catch (const std::regex_error& e) {
        std::cout << "[RegEx Error] Invalid Pattern: " << e.what() << std::endl;
    }

    wrapped.emplace_back(matches);
    return wrapped;
}
