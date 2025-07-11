//
// Created by Admin on 11.07.2025.
//

#include "MagicByteAnalyzer.h"

#include <iostream>

#include "../core/FileCollecter.h"

MagicByteAnalyzer::MagicByteAnalyzer(std::string &path) {
    //get Files
    if (!std::filesystem::exists(path)) {
        std::cerr << "Path does not exists" << std::endl;
    }
    this->files = FileCollector::collect(path);

}

//TODO : analyze()