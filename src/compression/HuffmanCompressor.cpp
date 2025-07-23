#include "HuffmanCompressor.h"

#include <iostream>
#include <unordered_map>
#include <bitset>
#include <sstream>

bool HuffmanCompressor::compress(const std::filesystem::path& src, const std::filesystem::path& dst) {
    //Create FreqMap
    std::unordered_map<unsigned char, int> freqMap;
    std::ifstream ifs(src, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "[Error] Could not open file!" << src << std::endl;
        return false;
    }
    //gather original Datasize to store in file for lateron padding
    std::string originalData;
    unsigned char byte;
    while (ifs.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
        freqMap[byte]++;
        originalData += byte;
    }
    ifs.close();
    //Build hufftree
    HuffmanTree huffmanTree;
    huffmanTree.build(freqMap);
    //Gather size and serialize tree
    std::ostringstream temp;
    huffmanTree.serialize(temp);
    std::uint16_t huffSize = temp.str().size();
    //create code to compromise file
    std::unordered_map<unsigned char, std::string> hufftreeCode;
    parseHuffTree(huffmanTree.root, &hufftreeCode, "");
    //Compromise Data
    std::string bitBuffer;
    for (unsigned char c : originalData) {
        bitBuffer += hufftreeCode[c];
    }
    //Turn Data to Bits and fill in padding
    std::string byteStream;
    for (size_t i = 0; i < bitBuffer.size(); i += 8) {
        std::string byteStr = bitBuffer.substr(i, 8);
        while (byteStr.size() < 8) byteStr += "0"; // Padding
        std::bitset<8> bits(byteStr);
        byteStream += static_cast<unsigned char>(bits.to_ulong());
    }
    //Open output file
    std::ofstream ofs(dst, std::ios::binary);
    std::uint32_t originalSize = originalData.size();
    //Write size of tree, and size of data without padding
    ofs.write(reinterpret_cast<const char *>(&huffSize), sizeof(huffSize));
    ofs.write(temp.str().data(), temp.str().size());
    ofs.write(reinterpret_cast<const char *>(&originalSize), sizeof(originalSize)); // Originalgröße
    ofs.write(byteStream.data(), byteStream.size());
    ofs.close();

    return true;
}
//Create Huff-Code Map
void HuffmanCompressor::parseHuffTree(HuffmanTree::Node *n, std::unordered_map<unsigned char, std::string>* mp, std::string path) {
    if (!n) return;
    if (n->isLeaf()) {
        (*mp)[n->value] = path;
    } else {
        parseHuffTree(n->left, mp, path + "0");
        parseHuffTree(n->right, mp, path + "1");
    }
}

bool HuffmanCompressor::decompress(const std::filesystem::path& src, const std::filesystem::path& dst) {
    std::ifstream ifs(src, std::ios::binary);
    //Read size of huff-tree
    std::uint16_t hufftreeSize;
    ifs.read(reinterpret_cast<char *>(&hufftreeSize), sizeof(hufftreeSize));
    //read tree-data
    std::string treeData(hufftreeSize, '\0');
    ifs.read(&treeData[0], hufftreeSize);
    //Read size of data
    std::uint32_t originalSize;
    ifs.read(reinterpret_cast<char *>(&originalSize), sizeof(originalSize)); // Originalgröße lesen
    //Create huff-tree
    std::stringstream treeStream(treeData);
    HuffmanTree huffTree;
    huffTree.deserialize(treeStream);
    //Read rest of file
    std::ostringstream compressedBuffer;
    compressedBuffer << ifs.rdbuf();
    std::string compressedData = compressedBuffer.str();
    ifs.close();

    std::ofstream ofs(dst, std::ios::binary);
    HuffmanTree::Node* node = huffTree.root;
    //decompress data using huff tree
    std::uint32_t written = 0;
    for (unsigned char byte : compressedData) {
        std::bitset<8> bits(byte);
        for (int i = 7; i >= 0; --i) {
            node = (bits[i] == 0) ? node->left : node->right;
            if (node->isLeaf()) {
                if (written >= originalSize) break;
                ofs.put(node->value);
                node = huffTree.root;
                ++written;
            }
        }
        if (written >= originalSize) break;
    }

    ofs.close();
    return true;
}
