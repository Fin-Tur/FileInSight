//
// Created by Admin on 05.07.2025.
//
#include <vector>
#include "../models/FileInfo.h"

#ifndef ABSTRACTFINDER_H
#define ABSTRACTFINDER_H

class AbstractFinder {
    public:
    virtual std::vector<std::vector<FileInfo>> find() = 0;
    virtual ~AbstractFinder() = default;
};

#endif //ABSTRACTFINDER_H
