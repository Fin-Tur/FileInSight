//
// Created by Admin on 10.07.2025.
//

#include "zstdCompression.h"
#include <zstd.h>
#include <fstream>
#include <vector>
#include <iostream>

bool zstdCompressor::compress(const std::filesystem::path& src, const std::filesystem::path& dst) {
    //Define compressionLevel (1-22)
    int compressionLevel = 10;

    // Read the input file into a buffer
    std::ifstream ifs(src, std::ios::binary);
    std::vector<char> inputData((std::istreambuf_iterator<char>(ifs)), {});
    ifs.close();

    // Allocate buffer for compressed data (maximum size)
    size_t bound = ZSTD_compressBound(inputData.size());
    std::vector<char> compressedData(bound);

    // Perform compression
    size_t compressedSize = ZSTD_compress(
        compressedData.data(),    // destination buffer
        bound,                    // max size of destination buffer
        inputData.data(),         // source data
        inputData.size(),         // size of source data
        compressionLevel          // compression level (1 = fast, 22 = strong)
    );

    // Check for compression errors
    if (ZSTD_isError(compressedSize)) {
        std::cerr << "Compression failed: " << ZSTD_getErrorName(compressedSize) << '\n';
        return false;
    }

    // Open output file and write original size + compressed data
    std::ofstream ofs(dst, std::ios::binary);
    size_t originalSize = inputData.size();
    ofs.write(reinterpret_cast<const char*>(&originalSize), sizeof(size_t)); // write original size first
    ofs.write(compressedData.data(), compressedSize);                        // then write compressed data
    ofs.close();

    return true;
}

bool zstdCompressor::decompress(const std::filesystem::path& src, const std::filesystem::path& dst) {
    // Open the compressed file
    std::ifstream ifs(src, std::ios::binary);
    if (!ifs) {
        std::cerr << "Error opening file!\n";
        return false;
    }

    // Read original uncompressed size
    size_t originalSize = 0;
    ifs.read(reinterpret_cast<char*>(&originalSize), sizeof(size_t));
    if (!ifs.good()) {
        std::cerr << "Error occurred while reading file!\n";
        return false;
    }

    // Read the rest of the file as compressed data
    std::vector<char> inputData((std::istreambuf_iterator<char>(ifs)), {});
    ifs.close();

    // Allocate buffer for decompressed data
    std::vector<char> outputData(originalSize);

    // Decompress
    size_t result = ZSTD_decompress(
        outputData.data(),
        originalSize,
        inputData.data(),
        inputData.size()
    );

    if (ZSTD_isError(result)) {
        std::cerr << "Decompression failed: " << ZSTD_getErrorName(result) << '\n';
        return false;
    }

    // Write the decompressed data to the output file
    std::ofstream ofs(dst, std::ios::binary);
    if (!ofs) {
        std::cerr << "Error opening output file!\n";
        return false;
    }

    ofs.write(outputData.data(), result);
    if (!ofs.good()) {
        std::cerr << "Error writing output file!\n";
    }
    return true;
}