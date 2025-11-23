#pragma once
#include <filesystem>
#include <fstream>

#include "AbstractCompressor.h"
#include "HuffmanTree.h"


class HuffmanCompressor final :public AbstractCompressor{
public:
    bool compress(const std::filesystem::path& src, const std::filesystem::path& dst, int compressionLevel) override;

    bool decompress(const std::filesystem::path& src, const std::filesystem::path& dst) override;

    static void parseHuffTree(HuffmanTree::Node* n, std::unordered_map<unsigned char, std::string>* mp, std::string path);
};


