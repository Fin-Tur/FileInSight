#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct RecoveredFile {
    std::string type;
    std::streampos offset;
    std::streampos end;

};

