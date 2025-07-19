//
// Created by Admin on 18.07.2025.
//

#include "RawRecoveryScanner.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

void RawRecoveryScanner::scan(std::ifstream& ifs) {
    //check for valid stream
    if (!ifs.is_open()) {
        std::cout << "[Error] Invalid File stream!\n";
    }
    //Create recFile instance
    std::unique_ptr<RecoveredFile> recFile;
    bool inRecovery = false;
    //Check for header
    std::vector<uint8_t> window(maxWindowLength);
    while (ifs.read(reinterpret_cast<char *>(window.data()), window.size())) {

        std::streampos pos = ifs.tellg();

        for (const auto& signature : knownSignatures) {
            if (std::equal(signature.header.begin(), signature.header.end(), window.begin())) {
                //header found -> if recovery in action extract file and instanciate new one
                //if not create instance right away
                if (inRecovery && recFile) {
                    recFile->end = pos - static_cast<std::streamoff>(signature.header.size());
                    this->extractedFiles.push_back(*recFile);
                }
                inRecovery = true;
                recFile = std::make_unique<RecoveredFile>();
                recFile->offset = pos - static_cast<std::streamoff>(signature.header.size());
                recFile->type = signature.name;
                continue;
            }
            //Check for footers
            if (std::equal(signature.footer.begin(), signature.footer.end(), window.begin())) {
                if (inRecovery) {
                    //extract file
                    recFile->end = pos - static_cast<std::streamoff>(window.size()-signature.footer.size());
                    this->extractedFiles.push_back(*recFile);
                    recFile.reset();
                    inRecovery = false;
                }
            }
        }
        //Move window by 1 byte
        ifs.seekg(-static_cast<int>(window.size()+1), std::ios::cur);
    }

    //IF File Recovery is active, extract file
    if (recFile && inRecovery) {
        ifs.clear();  //if eofbit is set
        ifs.seekg(0, std::ios::end);
        recFile->end = ifs.tellg();
        this->extractedFiles.push_back(*recFile);
        recFile.release();
        inRecovery = false;

    }
}

void RawRecoveryScanner::extractFiles(std::ifstream& ifs) const {
    //Check for valid fstream
    if (!ifs.is_open()) {
        std::cout << "[Error] Invalid File stream!\n";
    }
    //Create names for files
    std::vector<std::string> fileNames(this->extractedFiles.size());
    for (int i = 0; i < this->extractedFiles.size(); i++) {
        fileNames[i]=("extracted_file-"+std::to_string(this->extractedFiles.size()-i));
    }
    //Extract files
    for (const auto& file : this->extractedFiles) {
        std::streamsize dataLength = file.end - file.offset;
        std::vector<uint8_t> data(dataLength);
        ifs.seekg(file.offset);
        ifs.read(reinterpret_cast<char *>(data.data()), dataLength);

        std::ofstream outFile;
        if (!std::filesystem::exists("recovered")) {
            std::filesystem::create_directory("recovered");
        }
        outFile.open("recovered/"+fileNames.back()+"."+file.type, std::ios::binary);
        outFile.write(reinterpret_cast<char *>(data.data()), dataLength);
        outFile.close();
        fileNames.pop_back();

    }
}
