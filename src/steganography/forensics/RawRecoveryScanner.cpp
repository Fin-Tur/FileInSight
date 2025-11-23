
#include "RawRecoveryScanner.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

RawRecoveryScanner::RawRecoveryScanner(const bool& enable_UTF16LE) {
    if (enable_UTF16LE) addUTF16LESignatures();
}


void RawRecoveryScanner::scan(std::ifstream& ifs) {
    //check for valid stream
    if (!ifs.is_open()) {
        std::cerr << "[Error] Invalid File stream!\n";
    }
    //Create recFile instance
    std::unique_ptr<RecoveredFile> recFile;
    bool inRecovery = false;
    //Check for header
    std::vector<uint8_t> window(maxWindowLength);
    while (true) {
        std::streampos pos = ifs.tellg();
        if (!ifs.read(reinterpret_cast<char *>(window.data()), window.size())) break;

        for (const auto& signature : knownSignatures) {
            if (std::equal(signature.header.begin(), signature.header.end(), window.begin())) {
                //header found -> if recovery in action extract file and instanciate new one
                //if not create instance right away
                if (inRecovery && recFile) {
                    recFile->end = pos;
                    this->extractedFiles.push_back(*recFile);
                }
                inRecovery = true;
                recFile = std::make_unique<RecoveredFile>();
                recFile->offset = pos;
                recFile->type = signature.name;
                continue;
            }
            //Check for footers
            if (!signature.footer.empty() and std::equal(signature.footer.begin(), signature.footer.end(), window.begin())) {
                if (inRecovery) {
                    //extract file
                    recFile->end = pos + static_cast<std::streamoff>(signature.footer.size());
                    this->extractedFiles.push_back(*recFile);
                    recFile.reset();
                    inRecovery = false;
                }
            }
        }
        //Move window by 1 byte
        ifs.seekg(-static_cast<int>(window.size()-1), std::ios::cur);
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

void RawRecoveryScanner::extractFiles(std::ifstream& ifs, const std::string& outputDir) const {
    //Check for valid fstream
    if (!ifs.is_open()) {
        std::cerr << "[Error] Invalid File stream!\n";
    }
    //Create names for files
    std::vector<std::string> fileNames(this->extractedFiles.size());
    for (int i = 0; i < this->extractedFiles.size(); i++) {
        fileNames[i]=("extracted_file_"+std::to_string(this->extractedFiles.size()-i));
    }
    //Extract files
    if (this->extractedFiles.empty()) {
        std::cout << "[Info] Could not recover files!\n";
    }

    for (const auto& file : this->extractedFiles) {
        std::streamsize dataLength = file.end - file.offset;
        std::vector<uint8_t> data(dataLength);
        ifs.seekg(file.offset);
        ifs.read(reinterpret_cast<char *>(data.data()), dataLength);

        std::ofstream outFile;
        outFile.open(fileNames.back()+"."+file.type, std::ios::binary);
        outFile.write(reinterpret_cast<char *>(data.data()), dataLength);
        outFile.close();
        std::cout << "[Info] Recovered: " << fileNames.back() << "("<< dataLength <<" bytes) succesfully!\n";
        fileNames.pop_back();

    }
}

void RawRecoveryScanner::addUTF16LESignatures() {
    std::vector<FileSignature> newSignatures;
    std::vector<uint8_t> UTF16signaturHeader;
    std::vector<uint8_t> UTF16signaturFooter;
    //Iterate through UTF-8 sigs
    for (auto& signature : knownSignatures) {
        //Convert Header
        for (uint8_t byte : signature.header) {
            UTF16signaturHeader.push_back(byte);
            UTF16signaturHeader.push_back(0x00);
        }
        //ComvertFooter
        for (uint8_t byte : signature.footer) {
            UTF16signaturFooter.push_back(byte);
            UTF16signaturFooter.push_back(0x00);
        }
        //Create new Signature
        newSignatures.emplace_back(UTF16signaturHeader, UTF16signaturFooter, signature.name+"_utf16le");
        UTF16signaturHeader.clear();
        UTF16signaturFooter.clear();
    }
    //Add signatures
    knownSignatures.insert(knownSignatures.end(), newSignatures.begin(), newSignatures.end());
}
