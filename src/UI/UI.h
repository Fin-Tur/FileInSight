//
// Created by Admin on 03.07.2025.
//

#include <iostream>
#include "../finder/DuplicateFinder.h"
#include "../core/FileCollecter.h"
#include "../finder/SimilarityFinder.h"

#ifndef UI_H
#define UI_H

const std::string passwordHashed = "245ed3bcc0f0652aa9d4a3b2098a5878c1e4745ff8b01a5efbe11adbe93291ab";

class UI {

public:
    UI() = default;
    void start_ui();
    void start_duplicateSearch();
    static void start_regexSearch();
    static void start_agingSearch();
    static void start_compression();
    static void start_similaritySearch();
    static bool verifyPassword();

private:
    long duplicateDataAmount = 0;
    long freedMemory = 0;

    static std::intmax_t bytesToKB(std::intmax_t bytes){return bytes/1024;}
    static std::intmax_t bytesToMB(std::intmax_t bytes){return (bytes/1024)/1024;}
};



#endif //UI_H
