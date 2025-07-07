//
// Created by Admin on 05.07.2025.
//

#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H
#include <filesystem>
#include <fstream>
#include "HuffmanTree.h"


class HuffmanCompressor {
public:
    static void compress(const std::filesystem::path& inputPath,
                  const std::filesystem::path& outputPath);

    void decompress(const std::filesystem::path& compressedPath,
                    const std::filesystem::path& outputPath);

    static void parseHuffTree(HuffmanTree::Node* n, std::unordered_map<unsigned char, std::string>* mp, std::string path);
};



#endif //HUFFMANCOMPRESSOR_H
