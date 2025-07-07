//
// Created by Admin on 03.07.2025.
//

#include <iostream>
#include "../finder/DuplicateFinder.h"
#include "../core/FileCollecter.h"

#ifndef UI_H
#define UI_H

const std::string passwordHashed = "245ed3bcc0f0652aa9d4a3b2098a5878c1e4745ff8b01a5efbe11adbe93291ab";

class UI {
    long duplicateDataAmount = 0;
    long freedMemory = 0;

public:
    UI() = default;
    void start_ui();
    void start_duplicateSearch();
    static void start_regexSearch();
    static void start_agingSearch();
    static bool verifyPassword();
};



#endif //UI_H
