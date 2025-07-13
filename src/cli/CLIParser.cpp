//
// Created by Admin on 13.07.2025.
//

#include "CLIParser.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>
#include <bits/ostream.tcc>
#include "../compression/zstdCompression.h"
#include "../finder/RegExFinder.h"
#include "../models/FileInfo.h"

int CLIParser::run(int argc, char *argv[]) {

    //Check for valid Command
    std::string command = argv[1];
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (!commands.contains(command)) {
        std::cout << "[Error] Command not found! Type {FileInSight -help} for help-\n";
        return 1;
    }

    //Command Declaration
    if (command == "-compress" && argc == 3) {
        std::string inputFile = argv[2];
        if (!std::filesystem::exists(inputFile)) {
            std::cout << "\n[Error] File does not exist!\n";
            return 1;
        }
        zstdCompressor compressor;
        compressor.compress(inputFile, inputFile+".fisc");
        std::filesystem::remove(inputFile);
        std::cout << "[Info] Compression successful!\n";

    }else if (command == "-decompress" and argc == 3) {
        std::string inputFile = argv[2];
        if (!std::filesystem::exists(inputFile)) {
            std::cout << "\n[Error] File does not exist!\n";
            return 1;
        }
        std::string outputFile = inputFile.substr(0, inputFile.size()-5);
        if (inputFile.size() < 5 || inputFile.substr(inputFile.size() - 5) != ".fisc") {
            std::cout << "[Error] Expected .fisc file for decompression!\n";
            return 1;
        }
        zstdCompressor compressor;
        compressor.decompress(inputFile, outputFile);
        std::filesystem::remove(inputFile);
        std::cout << "[Info] Decompression successful!\n";

    }else if (command == "-find" && argc == 4) {
        std::string path = argv[2];
        if (!std::filesystem::exists(path)) {
            std::cout << "\n[Error] File does not exist!\n";
            return 1;
        }
        std::string pattern = argv[3];
        RegExFinder finder(FileCollector::collect(path), pattern);
        std::vector<FileInfo> matches = finder.find()[0];

        if (matches.size() < 1) {
            std::cout << "\n[Info] No Matches!\n";
        }else {
            std::cout << "\n[Info] Matches for pattern {" <<pattern<< "} in path {" <<path<< "} :\n";
        }

        for (auto& match : matches) {
            std::cout << "  -> " << path << ":\n";
        }

    }else if (command == "-define" && argc == 3) {
        std::string path = argv[2];
        if (!std::filesystem::exists(path)) {
            std::cout << "\n[Error] File does not exist!\n";
            return 1;
        }
        //TODO : path declaration before if statements
    }



    return 0;
}
