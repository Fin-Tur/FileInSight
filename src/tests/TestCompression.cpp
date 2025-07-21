//
// Created by Admin on 07.07.2025.
//
#include "../compression/HuffmanCompressor.h"
#include "../thirdparty/testing/catch_amalgamated.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

TEST_CASE("HuffmanCompression test successfull!", "[compression]"){
    //Create files
    std::filesystem::path inputFile = "testCompression.bin";
    std::filesystem::path compressedFile = "temp_compressed.huff";
    std::filesystem::path decompressedFile = "temp_decompressed.txt";
    //Write testfile
    std::ofstream ofs(inputFile, std::ios::binary);
    std::vector<uint8_t> dummy(100, 0xAB);  // Dummy-Daten
    ofs.write(reinterpret_cast<char*>(&dummy), sizeof(dummy));
    ofs.close();

    //Handle Compressor
    HuffmanCompressor compressor;

    compressor.compress(inputFile, compressedFile);
    compressor.decompress(compressedFile, decompressedFile);

    //Check similarity
    std::ifstream original(inputFile, std::ios::binary);
    std::ifstream decompressed(decompressedFile, std::ios::binary);

    original.seekg(0, std::ios::end);
    size_t originalSize = original.tellg();
    original.seekg(0, std::ios::beg);

    decompressed.seekg(0, std::ios::end);
    size_t decompressedSize = decompressed.tellg();
    decompressed.seekg(0, std::ios::beg);

    std::vector<uint8_t> originalBuffer(originalSize);
    original.read(reinterpret_cast<char*>(originalBuffer.data()), originalBuffer.size());
    original.close();

    std::vector<uint8_t> decompressedBuffer(decompressedSize);
    decompressed.read(reinterpret_cast<char*>(decompressedBuffer.data()), decompressedBuffer.size());
    decompressed.close();

    original.close();
    decompressed.close();

    REQUIRE(originalSize == decompressedSize);
    REQUIRE(std::equal(originalBuffer.begin(), originalBuffer.end(), decompressedBuffer.begin()));

    //CleanUp
    std::filesystem::remove(compressedFile);
    std::filesystem::remove(decompressedFile);
    std::filesystem::remove(inputFile);


}
