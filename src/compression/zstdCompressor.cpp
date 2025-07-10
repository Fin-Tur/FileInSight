//
// Created by Admin on 10.07.2025.
//

#include "zstdCompressor.h"

bool zstdCompressor::compress(std::filesystem::path src, std::filesystem::path dst,  int compressionLevel) {
    //Read input
    std::ifstream ifs(src, std::ios::binary);
    std::vector<char> inputData((std::istreambuf_iterator<char>(ifs)), {});
    ifs.close();
    //Allocate buffer for max size compressed data
    size_t bound = ZSTD_compressBound(inputData.size());
    std::vector<char> compressedData(bound);
    //compress (dst, dstCapacity, src, srcCapacity, compression level)
    size_t compressedSize = ZSTD_compress(
        compressedData.data(),
        bound,
        inputData.data(),
        inputData.size(),
        compressionLevel
    );
    //Check for Errors
    if (ZSTD_isError(compressedSize)) {
        std::cerr << "Compression failed: " << ZSTD_getErrorName(compressedSize) << '\n';
        return false;
    }
    //Write size of original data plus compressed data
    std::ofstream ofs(dst, std::ios::binary);
    size_t originalSize = inputData.size();
    ofs.write(reinterpret_cast<const char*>(&originalSize), sizeof(size_t)); // Originalgröße speichern
    ofs.write(compressedData.data(), compressedSize);
    ofs.close();

    return true;
}

bool zstdCompressor::decompress(std::filesystem::path src, std::filesystem::path dst) {
    //Open file
    std::ifstream ifs(src, std::ios::binary);
    if (!ifs){std::cerr <<"Error opening file!\n";return false;}
    //Read Original (uncompressed size)
    size_t originalSize = 0;
    ifs.read(reinterpret_cast<char*>(&originalSize), sizeof(size_t));
    if (!ifs.good()) {std::cerr << "Error occured while reading file!\n"; return false;}
    //read file (compressed data)
    std::vector<char> inputData((std::istreambuf_iterator<char>(ifs)), {});
    ifs.close();
    //Create char vector for decompressed Data
    std::vector<char> outputData(originalSize);
    //Decompress data (dst, dstCapacity, src, srcCapacity)
    size_t result = ZSTD_decompress(
        outputData.data(),
        originalSize,
        inputData.data(),
        inputData.size()
        );
    //check for errors
    if (ZSTD_isError(result)) {
        std::cerr << "Decompression failed: " << ZSTD_getErrorName(result) << '\n';
        return false;
    }
    //write in output
    std::ofstream ofs(dst, std::ios::binary);
    if (!ofs) {
        std::cerr << "Error opening output file!\n";
        return false;
    }
    ofs.write(outputData.data(), result);
    if (!ofs.good()) {
        std::cerr << "Error writing output file!\n";
        return false;
    }

    return true;

}

