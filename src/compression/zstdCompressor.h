//
// Created by Admin on 10.07.2025.
//

#ifndef ZSTDCOMPRESSOR_H
#define ZSTDCOMPRESSOR_H
#include <zstd.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>


class zstdCompressor {
public:
    static bool compress(std::filesystem::path src, std::filesystem::path dst,  int compressionLevel = 3);
    static bool decompress(std::filesystem::path src, std::filesystem::path dst);
};



#endif //ZSTDCOMPRESSOR_H
