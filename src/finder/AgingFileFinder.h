//
// Created by Admin on 05.07.2025.
//

#ifndef AGINGFILEFINDER_H
#define AGINGFILEFINDER_H
#include <string>
#include <utility>

#include "AbstractFinder.h"


class AgingFileFinder : public AbstractFinder{
public:
    explicit AgingFileFinder(std::vector<FileInfo> files, const int& lastUsedCriteria) : lastUsedCriteria(lastUsedCriteria) , files(std::move(files)){};
    std::vector<std::vector<FileInfo>> find() override;
private:
    const int lastUsedCriteria;
    std::vector<FileInfo> files;
};



#endif //AGINGFILEFINDER_H
