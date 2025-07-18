//
// Created by Admin on 03.07.2025.
//

#include "UI.h"

#include <algorithm>

#include "../analyzer/MagicByteAnalyzer.h"
#include "../compression/HuffmanCompressor.h"
#include "../compression/zstdCompression.h"
#include "../finder/AgingFileFinder.h"
#include "../hashing/picosha2.h"
#include "../finder/RegExFinder.h"
#include "../steganography/XOREncryptor.h"
#include "../analyzer/EntropyAnalyzer.h"

bool UI::verifyPassword() {
    std::string input;
    std::cin >> input;
    return picosha2::hash256_hex_string(input) == passwordHashed;
}

void UI::start_ui() {
    /*
    std::cout << "Please enter password to continue:\n";
    if (!verifyPassword()) return;
    */

    //main-loop

    while (true) {
        this->freedMemory = 0;
        this->duplicateDataAmount = 0;
        std::cout << "\nPress ENTER to return to main menu...";
        std::cin.ignore(); std::cin.get();
        std::system("cls");

        std::cout << "\nWhich Program would you like to run? \n[D : DuplicateSearch]\n[R : RegEx Search]\n[A : Aged Search]\n[C : Compression]\n[S : SimilaritySearch]\n[M : MagicByteAnalyzer]\n[E : En/De-cryption]\n[T : Entropy]\n[Q : Quit]\n";
        std::string input;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == "d") {
            start_duplicateSearch();
        }else if (input == "r") {
            start_regexSearch();
        }else if (input == "a") {
            start_agingSearch();
        }else if (input == "c") {
            start_compression();
        }else if (input == "s") {
            start_similaritySearch();
        }else if (input == "m") {
            start_magicByteAnalyzer();
        }else if (input == "e") {
            start_cryption();
        }else if (input == "t"){
            start_entropy();
        }else if (input == "q"){
            break;
        }
    }
}

void UI::start_duplicateSearch() {

    std::cout << "Please enter path for duplicate search: \n";
    std::string filePath;
    std::cin >> filePath;


    try {
        const std::vector<FileInfo> files = FileCollector::collect(filePath);
        DuplicateFinder finder(files);
        std::vector<std::vector<FileInfo>> duplicates = finder.find();

        std::system("cls");

        if (duplicates.empty()){std::cout <<"No duplicates found!\n";}

        for (auto& filesD : duplicates) {
            std::cout << "\nFile Duplicate found: \n";
            for (auto& file: filesD) {
                std::cout << file.path << " size: " << file.size/1024.0 << "KB" << std::endl;
            }
            this->duplicateDataAmount += filesD[0].size*(filesD.size()-1);
            std::cout << "\nWould you like to delete following file/s? [Y : Yes], [N: NO] \n";

            for (int i = 1; i < filesD.size(); i++) {
                std::cout << filesD[i].path << " size: " << filesD[i].size/1024.0 << "KB" << std::endl;
            }
            std::cout << "\n";
            std::string deleteAnswer;
            std::cin >> deleteAnswer;
            std::transform(deleteAnswer.begin(), deleteAnswer.end(), deleteAnswer.begin(), ::tolower);

            if (deleteAnswer == "y") {
                this->freedMemory += filesD[0].size*(filesD.size()-1);
                finder.deleteDuplicates(filesD);
            }

        }

        std::cout << "Total Duplicate Data: " << bytesToKB(this->duplicateDataAmount) << "kB" << std::endl;
        std::cout << "Freed Memory: " << bytesToKB(this->freedMemory) << "kB" << std::endl;


    }catch (std::exception& e) {
        std::cout << "\nError :" << e.what() << std::endl;
    }

}

void UI::start_regexSearch() {
    std::cout << "Please enter path for Regex-Title search: \n";
    std::string filePath;
    std::cin >> filePath;

    std::cout << "Please enter pattern to look out for:\n";
    std::string pattern;
    std::cin >> pattern;

    try {
        std::vector<FileInfo> files = FileCollector::collect(filePath);
        RegExFinder finder(files, pattern);


        std::vector<FileInfo> results = finder.find()[0];

        if (results.empty()) {std::cout <<"\nNo matches found!";}

        for (auto& file: results) {
            std::cout << "Match! : " << file.path.string() << "\n";
        }

    } catch (std::exception& e) {
        std::cout << "\nError :" << e.what() << std::endl;
    }

}

void UI::start_agingSearch() {
    std::cout << "Please enter path for AgingFilter search: \n";
    std::string filePath;
    std::cin >> filePath;

    std::cout <<"Please enter aging criteria (last used) in days: \n";
    int lastUsed;
    std::cin >> lastUsed;

    try {
        const std::vector<FileInfo> files = FileCollector::collect(filePath);
        AgingFileFinder finder(files, lastUsed);
        const std::vector<FileInfo> results = finder.find()[0];
        if (results.empty()) {std::cout <<"\nNo matches found!";}

        for (auto& file: results) {
            std::cout << "Match! : " << file.path.string() << " Last Used: " << file.lastUsed << " days ago!\n";
        }

    } catch (std::exception& e) {
        std::cout << "\nError :" << e.what() << std::endl;
    }
}

void UI::start_compression() {
    //Get path
    std::string stringPathToFile;
    std::cout << "Please enter path to compression: ";
    std::cin >> stringPathToFile;

    if (!std::filesystem::exists(stringPathToFile)) {
        std::cout << "\n[Error] File does not exist!\n";
        return;
    }
    //get compression alg
    std::string alg;
    std::cout << "Please enter algorithm: [H : Huffman] , [Z : zstd]";
    std::cin >> alg;
    std::unique_ptr<AbstractCompressor> compressor;
    if (alg == "H" or alg == "h") {
        compressor = std::make_unique<HuffmanCompressor>();
    }else {
        compressor = std::make_unique<zstdCompressor>();
    }

    //Get Action
    std::cout << "Do you want to compress or decompress file? \n[C : Compress]\n[D : Decompress]";
    std::string decision;
    std::cin >> decision;

    if (decision == "C" or decision == "c") {
        //Compromise
        auto originalSize = static_cast<std::intmax_t>(std::filesystem::file_size(stringPathToFile));
        //Check file for size and throw warning if < 1 kb
        if (bytesToKB(originalSize) < 1) {
            std::cout << "\n[Warning] File size is under 1 KB: Compromising might increase size! [Y:N]\n";
            std::cin >> decision;
            if (!(decision == "Y" or decision == "y")) {return;}
        }

        if (compressor->compress(stringPathToFile, stringPathToFile + ".fisc")) {
            auto compressedSize = static_cast<std::intmax_t>(std::filesystem::file_size(stringPathToFile + ".fisc"));
            std::cout << "Compression done: Saved Memory -> "<< bytesToKB(originalSize - compressedSize) << " KiloBytes!\n";
            std::filesystem::remove(stringPathToFile);
        }else {
            std::cout << "Comrpession failed!\n";
        }


    }else if (decision == "D" or decision == "d") {
        //Decompromise
        std::string outputFile = stringPathToFile.substr(0, stringPathToFile.size()-5);
        if (stringPathToFile.size() < 5 || stringPathToFile.substr(stringPathToFile.size() - 5) != ".fisc") {
            std::cout << "[Error] Expected .fisc file for decompression!\n";
            return;
        }

        if (compressor->decompress(stringPathToFile, outputFile)) {
            std::cout << "Decompression done!\n";
            std::filesystem::remove(stringPathToFile);
        }else {
            std::cout << "Decompression failed!\n";
        }


    }else {
        std::cout << "[Error] Invalid Rqeuest!\n";
        return;
    }

}

void UI::start_similaritySearch() {
    //Collect Reference Path
    std::cout << "Please enter reference-file(s) path: ";
    std::string refPath;
    std::cin >> refPath;

    if (!std::filesystem::exists(refPath)) {
        std::cout << "[Error] Reference path does not exist!\n";
        return;
    }
    //Collect Search Path
    std::cout << "Please enter path to look for similarities: ";
    std::string searchPath;
    std::cin >> searchPath;

    if (!std::filesystem::exists(searchPath)) {
        std::cout << "[Error] Search path does not exist!\n";
        return;
    }
    //Find similarities
    SimilarityFinder simFinder(refPath, searchPath);
    std::vector<std::vector<FileInfo>> res = simFinder.find();

    if (!res.empty()) {
        std::cout << "\n[Info] Found similarities!\n";
        for (auto& group : res) {
            std::cout << "Similarities for file: " << group[0].path.string() << "\n";
            group.erase(group.begin());
            for (auto& file : group) {
                std::cout << "  -> Match: " << file.path.string() << "\n";
            }
        }
    }else {
        std::cout << "\n[Info] No similarities!\n";
    }

}

void UI::start_magicByteAnalyzer() {
    std::cout << "Please enter search path: ";
    std::string searchPath;
    std::cin >> searchPath;

    if (!std::filesystem::exists(searchPath)) {
        std::cout << "[Error] Search path does not exist!\n";
    }

    std::vector<std::pair<FileInfo, std::string>> flaggedFiles = MagicByteAnalyzer::analyzePath(searchPath);

    if (flaggedFiles.empty()) {
        std::cout << "\n[Info] No suspicious files found!\n";
        return;
    }
    std::cout << "\n[Info] Found suspicious file/s!\n";
    for (const auto& [file, extension] : flaggedFiles) {
        std::cout << "  -> FlaggedFile: " << file.path.string() << " was flagged as a " << extension <<" file!\n";
    }
}

void UI::start_cryption() {
    std::cout << "Please enter file path: ";
    std::string searchPath;
    std::cin >> searchPath;

    if (!std::filesystem::exists(searchPath)) {
        std::cout << "[Error] Search path does not exist!\n";
        return;
    }

    std::cout << "Do you want to de or encrypt file? [D : Decryption], [E : Encryption]\n";
    std::string answer;
    std::cin >> answer;

    std::cout << "Please enter password for de/en-cryption :";
    std::string password;
    std::cin >> password;


    std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

    if (answer == "d") {
        XOREncryptor::decrypt(searchPath, password);
        std::cout << "\n[Info] Decryption succesfull!\n";
    }else if (answer == "e") {
        XOREncryptor::encrypt(searchPath, password);
        std::cout << "\n[Info] Encryption succesfull!\n";
    }else {
        std::cout << "[Error] Invalid action!\n";
    }

}


void UI::start_entropy() {
    std::cout << "Please enter file path: ";
    std::string searchPath;
    std::cin >> searchPath;

    if (!std::filesystem::exists(searchPath)) {
        std::cout << "[Error] Search path does not exist!\n";
        return;
    }
    const double entropy = EntropyAnalyzer::analyze(searchPath);
    std::cout << "[Info] Entropy of file: " << searchPath << " is ->" << entropy << "bits/byte !\n";
}

std::string UI::convertBytes(std::size_t bytes) {
    if (bytes < 1024) {
        return std::to_string(bytes) + " bytes";
    }else if (bytes < 1024 * 1024) {
        return std::to_string(bytes/1024) + "KB";
    }else if (bytes < 1024 * 1024 * 1024) {
        return std::to_string(bytes / (1024 * 1024)) + "MB";
    }else {
        return std::to_string(bytes/(1024 * 1024 * 1024)) + "GB";
    }
}
