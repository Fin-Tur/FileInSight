//
// Created by Admin on 19.07.2025.
//

#include <cstdint>
#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>

#include "../steganography/forensics/RawRecoveryScanner.h"

void testRecovery() {
    //Create dummy file
    std::ofstream ofs("testraw.bin", std::ios::binary);
    std::vector<uint8_t> pngHeader = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    std::vector<uint8_t> dummy(100, 0xAB);  // Dummy-Daten
    std::vector<uint8_t> pngFooter = {0x00, 0x00, 0x00, 0x00, 'I', 'E', 'N', 'D'};

    ofs.write(reinterpret_cast<char*>(pngHeader.data()), pngHeader.size());
    ofs.write(reinterpret_cast<char*>(dummy.data()), dummy.size());
    ofs.write(reinterpret_cast<char*>(pngFooter.data()), pngFooter.size());
    ofs.close();

    //Try to extract lost file
    std::ifstream ifs("testraw.bin", std::ios::binary);
    RawRecoveryScanner scanner;
    scanner.scan(ifs);

    // Wichtig: Danach zurückspulen, um mit demselben Stream extrahieren zu können
    ifs.clear();
    ifs.seekg(0);
    scanner.extractFiles(ifs);

    if (std::filesystem::exists("/recovered/extracted_file-1.png")) {
        std::cout << "[Info] Test succesful!\n";
    }else {
        std::cout << "[Info] Test failed!\n";
    }
}
