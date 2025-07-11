//
// Created by Admin on 11.07.2025.
//

#ifndef SIMILARITYFINDER_H
#define SIMILARITYFINDER_H
#include <string>

#include "AbstractFinder.h"
#include "../models/FileInfo.h"


class SimilarityFinder : public AbstractFinder{
private:
    std::vector<FileInfo> referenceFiles;
    std::vector<FileInfo> files;
public:
    SimilarityFinder(const std::string& referencePath, const std::string& searchPath);
    std::vector<std::vector<FileInfo>> find() override;
};



#endif //SIMILARITYFINDER_H
