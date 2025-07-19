//
// Created by Admin on 19.07.2025.
//

#ifndef RECOVEREDFILE_H
#define RECOVEREDFILE_H
#include <cstdint>
#include <string>
#include <vector>

struct RecoveredFile {
    std::string type;
    std::streampos offset;
    std::streampos end;

};

#endif //RECOVEREDFILE_H
