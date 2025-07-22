//
// Created by Admin on 19.07.2025.
//

#include <cstdint>
#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include "../thirdparty/testing/catch_amalgamated.hpp"

#include "../steganography/forensics/RawRecoveryScanner.h"

TEST_CASE("Recovery test succesfull!", "[revocery]"){
    //Create dummy file
    std::ofstream ofs("testraw.bin", std::ios::binary);
    std::vector<uint8_t> jpegHeader = {0xFF, 0xD8};
    std::vector<uint8_t> dummy(100, 0xAB);  // Dummy-Daten
    std::vector<uint8_t> jpegFooter = {0xFF, 0xD9};


    //ofs.write(reinterpret_cast<char*>(dummy.data()), dummy.size());
    ofs.write(reinterpret_cast<char*>(dummy.data()), dummy.size());
    ofs.write(reinterpret_cast<char*>(jpegHeader.data()), jpegHeader.size());
    ofs.write(reinterpret_cast<char*>(dummy.data()), dummy.size());
    ofs.write(reinterpret_cast<char*>(jpegFooter.data()), jpegFooter.size());
    ofs.write(reinterpret_cast<char*>(dummy.data()), dummy.size());
    ofs.close();

    //Try to extract lost file
    std::ifstream ifs("testraw.bin", std::ios::binary);
    RawRecoveryScanner scanner(false);
    scanner.scan(ifs);

    ifs.clear();
    ifs.seekg(0);
    scanner.extractFiles(ifs, "recovered");
    ifs.close();



    REQUIRE(std::filesystem::exists("extracted_file_1.jpeg"));
    std::filesystem::remove("extracted_file_1.jpeg");
    std::filesystem::remove("testraw.bin");
}
