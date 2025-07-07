//
// Created by Admin on 04.07.2025.
//

#include <utility>

#include "AbstractFinder.h"
#include "../core/FileCollecter.h"

#ifndef REGEXFINDER_H
#define REGEXFINDER_H



class RegExFinder : public AbstractFinder{
    std::vector<FileInfo> files;
    std::string pattern;
public:
    explicit RegExFinder(const std::vector<FileInfo>& files, std::string  pattern) : files(files), pattern(std::move(pattern)) {};
    std::vector<std::vector<FileInfo>> find() override;
};



#endif //REGEXFINDER_H
