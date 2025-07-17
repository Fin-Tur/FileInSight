//
// Created by Admin on 17.07.2025.
//

#include "EntropyAnalyzer.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

double EntropyAnalyzer::analyze(const std::string& src) {
    if (!std::filesystem::exists(src)) {
        std::cout << "[Error] File does not exist!" << std::endl;
    }

    std::ifstream ifs(src, std::ios::binary);
    std::unordered_map<unsigned char, size_t> frequencies;
    size_t totalbytes = 0;

    unsigned char byte;
    while (ifs.read((char*)&byte, sizeof(byte))) {
        frequencies[byte]++;
        totalbytes++;
    }
    ifs.close();

    double entropy = 0;

    for (auto& [byte, freq] : frequencies) {
        const double chance = static_cast<double>(freq) / totalbytes;
        entropy += chance * std::log2(chance);
    }

    return -entropy;

}
