//
// Created by Admin on 11.07.2025.
//

#include "SimilarityFinder.h"

#include <iostream>

#include "../core/FileCollecter.h"
#include "../hashing/FuzzyHasher.h"

SimilarityFinder::SimilarityFinder(const std::string &referencePath, const std::string &searchPath) {
    this->referenceFiles =  FileCollector::collect(referencePath);
    this->files = FileCollector::collect(searchPath);

}

std::vector<std::vector<FileInfo>> SimilarityFinder::find() {
    /*
    std::cout << "[Debug] Called findSimilarities for: " << this->referenceFiles[0].path << "\n";
    */
    std::vector<std::vector<FileInfo>> res;

    for (const auto& ref : this->referenceFiles) {
        std::vector<FileInfo> group;
        group.push_back(ref);

        auto matches = FuzzyHasher::findSimilarities(ref, files);
        if (matches.has_value()) {
            group.insert(group.end(), matches->begin(), matches->end());
        }

        if (group.size() > 1) res.push_back(group);
    }


    return res;
}


