

#include "EntropyAnalyzer.h"

#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

double EntropyAnalyzer::analyze(const std::string& src) {
    if (!std::filesystem::exists(src)) {
        std::cerr << "[Error] File does not exist!" << std::endl;
        return -1;
    }
    
    try {
        std::ifstream ifs(src, std::ios::binary);
        if (!ifs.is_open()) {
            std::cerr << "[Error] Could not open file!" << std::endl;
            return -1;
        }

        std::unordered_map<unsigned char, size_t> frequencies;
        size_t totalbytes = 0;

        //buff reading : 64KB
        constexpr size_t BUFFER_SIZE = 64 * 1024;
        std::array<unsigned char, BUFFER_SIZE> buffer;

        while (ifs.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE) || ifs.gcount() > 0) {
            const std::streamsize bytesRead = ifs.gcount();
            
            for (std::streamsize i = 0; i < bytesRead; ++i) {
                frequencies[buffer[i]]++;
            }
            
            totalbytes += bytesRead;
        }

        if (totalbytes == 0) {
            std::cerr << "[Error] File is empty!" << std::endl;
            return -1;
        }

        //Calculate Shannon entropy
        double entropy = 0.0;
        for (const auto& [byte, freq] : frequencies) {
            const double chance = static_cast<double>(freq) / totalbytes;
            entropy += chance * std::log2(chance);
        }

        return -entropy;
        
    } catch (const std::exception& e) {
        std::cerr << "[Error] Exception: " << e.what() << std::endl;
        return -1;
    }
}
