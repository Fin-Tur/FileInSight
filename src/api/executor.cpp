
#include "executor.h"
#include "../compression/zstdCompression.h"
#include "../steganography/AESEncryptor.h"
#include "../steganography/AbstractEncryptor.h"
#include "../core/FileCollecter.h"
#include "../finder/DuplicateFinder.h"
#include "../models/FileInfo.h"
#include "analyzer/EntropyAnalyzer.h"
#include "API.h"
#include <analyzer/FileAnalyzer.h>
#include <cstring>

//0 -> Sucess, 1 -> Failure
int executor::compress(std::string &path, int compLvl) {
    auto comp = zstdCompressor();
    if (comp.compress(path, path, compLvl)) return 0;
    return 1;
}

//0 -> Sucess, 1 -> Failure
int executor::decompress(std::string &path) {
    auto comp = zstdCompressor();
    if (comp.decompress(path, path)) return 0;
    return 1;
}

//0 -> Sucess, 1 -> Failure
int executor::encrypt(const std::string &path, std::string& password, int iterations) {
    std::unique_ptr<AbstractEncryptor> enc = std::make_unique<AESEncryptor>();
    if (enc->handleEncryption(path, password, iterations)) return 0;
    return 1;
}

//0 -> Sucess, 1 -> Failure
int executor::decrypt(const std::string &path, std::string& password, int iterations) {
    std::unique_ptr<AbstractEncryptor> enc = std::make_unique<AESEncryptor>();
    if (enc->handleDecryption(path, password, iterations)) return 0;
    return 1;
}

//0->No Doplicates, 1->Dupes existing to file, 2->Dupes existing in general 3-> Failure
int executor::dupes_existing_for_file(const std::string &path, const std::string &file) {
    DuplicateFinder finder(FileCollector::collect(path));
    std::error_code ec;
    auto groups = finder.find();
    for (auto& group : groups) {
        for (auto& f : group) {
            ec.clear();
            if (std::filesystem::equivalent(file, f.path, ec)) {
                return 1;
            }

        }
    }
    if (!groups.empty()) return 2;
    return 0;
}

// return -> Entropy
double executor::entropy_for_file(const std::string &path) {
    return EntropyAnalyzer::analyze(path);
}




