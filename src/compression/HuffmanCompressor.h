//
// Created by Admin on 05.07.2025.
//

#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H
#include <filesystem>
#include <fstream>

#include "AbstractCompressor.h"
#include "HuffmanTree.h"


class HuffmanCompressor :public AbstractCompressor{
public:
    bool compress(const std::filesystem::path& src, const std::filesystem::path& dst) override;

    bool decompress(const std::filesystem::path& src, const std::filesystem::path& dst) override;

    static void parseHuffTree(HuffmanTree::Node* n, std::unordered_map<unsigned char, std::string>* mp, std::string path);
};



#endif //HUFFMANCOMPRESSOR_H
