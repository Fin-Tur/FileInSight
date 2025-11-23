
#include "VaultManager.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "core/FileCollecter.h"
#include "thirdparty/testing/catch_amalgamated.hpp"

bool VaultManager::detail::writeFile(const FileInfo &fi, std::ofstream &ofs) {
    //Look for errors
    if (!fi.size or fi.path.empty()) {
        return false;
    }

    //Gather filename & size
    std::string filenameStr = std::filesystem::path(fi.path).filename().string();
    std::vector<uint8_t> fileName(filenameStr.begin(), filenameStr.end());
    uint16_t fileNameSize = fileName.size();

    //Gather data
    std::ifstream ifs(fi.path, std::ios::binary);
    if (!ifs.is_open()) {
        return false;
    }

    std::vector<uint8_t> fileData(fi.size);
    if (!ifs.read(reinterpret_cast<char*>(fileData.data()), fileData.size())) {
        return false;
    }
    ifs.close();

    //Write filename size & filename
    ofs.write(reinterpret_cast<char*>(&fileNameSize), sizeof(fileNameSize));
    ofs.write(reinterpret_cast<const char*>(fileName.data()), fileName.size());
    //write data size & data
    uint64_t dataSize = fileData.size();
    ofs.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
    ofs.write(reinterpret_cast<char*>(fileData.data()), fileData.size());

    return ofs.good();
}

bool VaultManager::detail::readFile(std::ifstream &ifs, const std::string &dst) {
    //Prepare variables
    uint16_t fileNameSize;
    std::vector<uint8_t> fileName;
    uint64_t dataSize;
    std::vector<uint8_t> fileData;

    //Read info
    if (!ifs.is_open()) {
        std::cerr << "[Error] Could not open file!" << std::endl;
        return false;
    }
    ifs.read(reinterpret_cast<char*>(&fileNameSize), sizeof(fileNameSize));
    fileName.resize(fileNameSize);
    ifs.read(reinterpret_cast<char*>(fileName.data()), fileNameSize);
    ifs.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
    fileData.resize(dataSize);
    ifs.read(reinterpret_cast<char*>(fileData.data()), dataSize);

    //Write
    std::string fileNameStr(fileName.begin(), fileName.end());
    std::ofstream ofs(dst+"/"+fileNameStr, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(fileData.data()), dataSize);
    ofs.close();
    return true;


}


bool VaultManager::createVault(const std::string &dir, const std::string& name) {
    //Gather  files
    auto files = FileCollector::collect(dir);
    if (files.empty()) {
        std::cout << "[Error] Could not extract files!\n";
        return false;
    }
    //Prepare ofs
    std::filesystem::path vaultPath = std::filesystem::path(dir) / (name + ".vault");
    std::ofstream ofs(vaultPath, std::ios::binary);
    if (!ofs.is_open()) {
        std::cout << "[Error] Could not create Vault file!\n";
        return false;
    }
    //Write magic bytes
    ofs.write(reinterpret_cast<const char*>(vaultSignature.data()), vaultSignature.size());

    //Write number of files
    uint16_t fileCount = files.size();
    ofs.write(reinterpret_cast<const char*>(&fileCount), sizeof(fileCount));

    //Write file contents
    for (auto& file : files) {
        std::cout << "Writing file @ " << file.path << "With size " << file.size<<"\n";
        if (!VaultManager::detail::writeFile(file, ofs)) {
            std::cout << "[Error] Could not write file @ "<< file.path <<"\n";
        }
    }

    ofs.close();
    return true;

}

bool VaultManager::dissolveVault(const std::string &path, const std::string &dst) {
    //Open fstrean
    if (!std::filesystem::exists(path)) {
        std::cerr << "[Error] Could not find Vault file!\n";
        return false;
    }
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "[Error] Could not open Vault file!\n";
        return false;
    }
    //Gather files count
    std::vector<uint8_t> header(vaultSignature.size());
    ifs.read(reinterpret_cast<char*>(header.data()), header.size());
    if (!std::equal(header.begin(), header.end(), vaultSignature.begin())) {
        std::cerr << "[Error] Invalid File signature!\n";
        return false;
    }
    uint16_t fileCount;
    ifs.read(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));

    //Read Files
    for (uint16_t i = 0; i < fileCount; i++) {
        if (!VaultManager::detail::readFile(ifs, dst)) {
            std::cerr << "[Error] Could not read embedded file!\n";
        }

    }

    ifs.close();
    std::filesystem::remove(path);
    return true;


}
