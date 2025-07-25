//
// Created by Admin on 11.07.2025.
//

#include "FuzzyHasher.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>

#include "../models/FileInfo.h"

std::string FuzzyHasher::computeFuzzyHash(const FileInfo &file) {
    //Open File
    std::ifstream ifs(file.path, std::ios::binary);
    if (!ifs){std::cerr << "Error opening file " << file.path << std::endl; return std::string("");}
    //Read file
    std::vector<unsigned char> buffer(file.size);
    ifs.read(reinterpret_cast<char *>(buffer.data()), file.size);
    //Check actual read size
    std::streamsize bytesRead = ifs.gcount();
    if (bytesRead < 50) {
        //std::cerr << "[Error] Fuzzy Hashing needs at least 50 bytes of data!" <<std::endl;
        return std::string("");
    }
    ifs.close();
    //compute fuzzy Hash
    Tlsh hasher;
    hasher.update(buffer.data(), bytesRead);
    hasher.final();
    return hasher.getHash();

}

bool FuzzyHasher::areFilesSimilar(const FileInfo &file1, const FileInfo &file2, const std::uint8_t &cutOff) {
    const std::string fuzzyhashF1 = computeFuzzyHash(file1);
    const std::string fuzzyhashF2 = computeFuzzyHash(file2);

    //Create Objects and check for valid hashes!
    Tlsh hashObj1;
    if (hashObj1.fromTlshStr(fuzzyhashF1.c_str()) != 0) {
        std::cerr << "[Error] Invalid TlSH Hash for File 1!\n";
        return false;
    }

    Tlsh hashObj2;
    if (hashObj2.fromTlshStr(fuzzyhashF2.c_str()) != 0) {
        std::cerr << "[Error] Invalid TlSH Hash for File 2!\n";
        return false;
    }

    //Calcutate diff
    const int diff = hashObj1.totalDiff(&hashObj2);

    return diff <= cutOff;
}

std::optional<std::vector<FileInfo> > FuzzyHasher::findSimilarities(const FileInfo &referenceFile, const std::vector<FileInfo> &files, const std::uint8_t& cutOff) {
    //Instanciates Reference Obj
    const std::string fuzzyhashReferenceFile = computeFuzzyHash(referenceFile);
    Tlsh referenceObj;
    if (referenceObj.fromTlshStr(fuzzyhashReferenceFile.c_str()) != 0) {
        std::cerr << "[Error] Invalid TlSH Hash for Reference-File!\n";
        return std::nullopt;
    }
    //Create resolution and loop through files
    std::vector<FileInfo> matches;

    for (const FileInfo &file : files) {
        //Check if file is not refFile
        if (std::filesystem::equivalent(file.path, referenceFile.path)) {
            continue;
        }

        const std::string fuzzyhashFile = computeFuzzyHash(file);
        Tlsh fileObj;
        if (fileObj.fromTlshStr(fuzzyhashFile.c_str()) != 0) {
            //std::cerr << "[Error] Invalid Tlsh Hash for file!\n";
            continue;
        }
        /*
        std::cout << "Hash1: " << fuzzyhashReferenceFile << "\n";
        std::cout << "Hash2: " <<  fuzzyhashFile << "\n";
        std::cout << "Diff: " << referenceObj.totalDiff(&fileObj) << "\n";
        */
        if (referenceObj.totalDiff(&fileObj) <= cutOff) {
            matches.push_back(file);
        }
    }

    if (matches.empty()){ return std::nullopt;}
    return matches;


}


