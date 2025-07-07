//
// Created by Admin on 05.07.2025.
//

#include "HuffmanCompressor.h"

#include <bitset>
#include <iostream>
#include <unordered_map>


void HuffmanCompressor::compress(const std::filesystem::path &inputPath, const std::filesystem::path &outputPath) {
    //Create FreqMap
    std::unordered_map<unsigned char, int> freqMap;
    std::ifstream ifs(inputPath, std::ios::binary);
    if (!ifs.is_open()) {std::cerr << "Error opening file " << inputPath << std::endl;}
    unsigned char byte;
    while (ifs.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
        freqMap[byte]++;
    }
    ifs.close();

    //Build HuffTree
    HuffmanTree huffmanTree;
    huffmanTree.build(freqMap);
    //Gather Size and serialize tree
    std::ostringstream temp;
    huffmanTree.serialize(temp);

    std::uint8_t huffSize = temp.str().size();

    //create code to compromise file
    std::unordered_map<unsigned char, std::string> hufftreeCode;
    parseHuffTree(huffmanTree.root, &hufftreeCode, "");

    //Read File to compress
    std::ifstream ifs2(inputPath, std::ios::binary);
    std::string bitBuffer;
    unsigned char byte2;
    if (!ifs2.is_open()) {std::cerr << "Error opening file " << inputPath << std::endl;}
    while (ifs2.read(reinterpret_cast<char *>(&byte2), sizeof(byte2))) {
        bitBuffer += hufftreeCode[byte2];
    }
    ifs2.close();

    //Open output stream and write Size plus HuffmanTree for decompressing later on
    std::ofstream ofs(outputPath, std::ios::binary);
    std::string treeData = temp.str();
    ofs.write(reinterpret_cast<const char*>(&huffSize), sizeof(huffSize));
    ofs.write(treeData.data(), treeData.size());

    //Convert bitBuffer to actual Bits and write in output path
    while (bitBuffer.size() >= 8) {
        std::bitset<8> bits(bitBuffer.substr(0, 8));
        unsigned char byte = static_cast<unsigned char>(bits.to_ulong());
        ofs.write(reinterpret_cast<const char*>(&byte), 1);
        bitBuffer.erase(0, 8);
    }

    //Fill in Rest of last byte
    if (!bitBuffer.empty()) {
        bitBuffer.append(8 - bitBuffer.size(), '0'); // mit '0' auffüllen
        std::bitset<8> bits(bitBuffer);
        unsigned char byte = static_cast<unsigned char>(bits.to_ulong());
        ofs.write(reinterpret_cast<const char*>(&byte), 1);
    }

    ofs.close();

}

void HuffmanCompressor::parseHuffTree(HuffmanTree::Node *n, std::unordered_map<unsigned char, std::string>* mp, std::string path) {
    if (!n) return;
    if (n->isLeaf()) {
        (*mp)[n->value] = path;
    }else {
        parseHuffTree(n->left, mp, path+"0");
        parseHuffTree(n->right, mp, path+"1");
    }
}
