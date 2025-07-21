//
// Created by Admin on 13.07.2025.
//

#include "CLIParser.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>
#include <bits/ostream.tcc>
#include <fstream>

#include "../analyzer/EntropyAnalyzer.h"
#include "../compression/zstdCompression.h"
#include "../finder/RegExFinder.h"
#include "../models/FileInfo.h"
#include "../analyzer/MagicByteAnalyzer.h"
#include "../finder/DuplicateFinder.h"
#include "../finder/SimilarityFinder.h"
#include "../finder/AgingFileFinder.h"
#include "../steganography/XOREncryptor.h"
#include "../analyzer/FileAnalyzer.h"
#include "../analyzer/PathAnalyzer.h"
#include "../steganography/forensics/RawRecoveryScanner.h"


void CLIParser::printHelp() {
    std::cout << "Commands:\n"
            << "  FileInSight -compress <file> : Compresses file\n"
            << "  FileInSight -decompress <file> : Decompresses file\n"
            << "  FileInSight -find <path> <pattern> : RegEx search in <path> for <pattern>\n"
            << "  FileInSight -define <path> : Analyzes Magic Bytes to ensure file format\n"
            << "  FileInSight -duplicates <path> : Finds duplicates\n"
            << "  FileInSight -similar <file> <path> : finds similar files to <file> in <path>\n"
            << "  FileInSight -aging <path> <age cap> : finds files in <path>, last used more days ago then <age cap>\n"
            << "  FileInSight -encrypt <file> <password> : Encrypts file\n"
            << "  FileInSight -decrypt <file> <password> : Decrypts file\n"
            << "  FileInSight -entropy <file> : Displays Shannon Entropy\n"
            << "  FileInSight -info <file> : Displays File info\n"
            << "  FileInSight -path <path> : Displays Path info\n"
            << "  FileInSight -recover <raw dump> : Recovers files from Raw dump\n";
}


int CLIParser::run(int argc, char *argv[]) {

    //Define dispatches
    static const std::map<std::string, std::function<int(int, char*[])>> dispatchMap = {
        { "-compress", [](int argc, char* argv[]) -> int {
            if (argc != 3) {
                std::cerr << "[Error] Usage: -compress <file>\n";
                return 1;
            }
            return CLIParser::handleCompress(argv[2]);
        }},
        { "-decompress", [](int argc, char* argv[]) -> int {
            if (argc != 3) {
                std::cerr << "[Error] Usage: -decompress <file>\n";
                return 1;
            }
            return CLIParser::handleDecompress(argv[2]);
        }},
        { "-find", [](int argc, char* argv[]) -> int {
            if (argc != 4) {
                std::cerr << "[Error] Usage: -find <path> <pattern>\n";
                return 1;
            }
            return CLIParser::handleFind(argv[2], argv[3]);
        }},
        { "-define", [](int argc, char* argv[]) -> int {
            if (argc != 3) {
                std::cerr << "[Error] Usage: -define <path>\n";
                return 1;
            }
            return CLIParser::handleDefine(argv[2]);
        }},
        { "-duplicates", [](int argc, char* argv[]) -> int {
            if (argc != 3) {
                std::cerr << "[Error] Usage: -duplicates <path>\n";
                return 1;
            }
            return CLIParser::handleDuplicates(argv[2]);
        }},
        { "-similar", [](int argc, char* argv[]) -> int {
            if (argc != 4) {
                std::cerr << "[Error] Usage: -similar <file> <path>\n";
                return 1;
            }
            return CLIParser::handleSimilar(argv[2], argv[3]);
        }},
        { "-aging", [](int argc, char* argv[]) -> int {
            if (argc != 4) {
                std::cerr << "[Error] Usage: -aging <path> <age cap>\n";
                return 1;
            }
            int ageCap = std::atoi(argv[3]);
            return CLIParser::handleAging(argv[2], ageCap);
        }},
        { "-help", [](int, char*[]) -> int {
            CLIParser::printHelp();
            return 0;
        }},
        { "-encrypt", [](int argc, char* argv[]) -> int {
          if (argc != 4) {
              std::cerr << "[Error] Usage: -encrypt <path> <password>\n";
              return 1;
          }
            std::string password = argv[3];
            return CLIParser::handleXOREncrypt(argv[2], password);
        }},
        { "-decrypt", [](int argc, char* argv[]) -> int {
            if (argc != 4) {
                std::cerr << "[Error] Usage: -decrypt <path> <password>\n";
                return 1;
            }
            std::string password = argv[3];
            return CLIParser::handleXORDecrypt(argv[2], password);
        }},
        { "-entropy", [](int argc, char* argv[]) -> int {
            if (argc != 3) {
                std::cerr << "[Error] Usage: -entropy <file>\n";
                return 1;
            }
            return CLIParser::handleEntropy(argv[2]);
        }},
        { "-info", [](int argc, char* argv[]) -> int {
            if (argc != 3) {
                std::cerr << "[Error] Usage: -info <file>\n";
                return 1;
            }
            return CLIParser::handleFileInfo(argv[2]);
        }},
        {"-path", [](int argc, char* argv[]) -> int {
            if (argc != 3) {
                std::cerr << "[Error] Usage: -path <path>\n";
                return 1;
            }
            return CLIParser::handlePathInfo(argv[2]);
        }},
        {"-recover", [](int argc, char* argv[]) -> int {
            if (argc != 3) {
                std::cerr << "[Error] Usage: -recover <raw dump>\n";
                return 1;
            }
            return CLIParser::handleRecovery(argv[2]);
        }}

    };

    if (argc < 2) {std::cerr << "[Error] Usage: FileInSight -command ...\n"; printHelp(); return 1;}


    //Check for valid Command
    std::string command = argv[1];
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (dispatchMap.contains(command)) {
        return dispatchMap.at(command)(argc, argv);  // Run matching lambda
    } else {
        std::cerr << "[Error] Unknown command: " << command << std::endl;
        CLIParser::printHelp();
        return 1;
    }

}

int CLIParser::handleCompress(const std::string &path) {
    zstdCompressor compressor;
    compressor.compress(path, path+".fisc");
    std::filesystem::remove(path);
    std::cout << "[Info] Compression successful!\n";
    return 0;
}

int CLIParser::handleDecompress(const std::string &path) {
    std::string outputFile = path.substr(0, path.size()-5);
    if (path.size() < 5 || path.substr(path.size() - 5) != ".fisc") {
        std::cout << "[Error] Expected .fisc file for decompression!\n";
        return 1;
    }
    zstdCompressor compressor;
    compressor.decompress(path, outputFile);
    std::filesystem::remove(path);
    std::cout << "[Info] Decompression successful!\n";
    return 0;
}

int CLIParser::handleFind(const std::string &path, const std::string &pattern) {
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
    return 0;
}


int CLIParser::handleDefine(const std::string &path) {
    std::vector<std::pair<FileInfo, std::string>> flaggedFiles = MagicByteAnalyzer::analyzePath(path);
    if (flaggedFiles.empty()) {
        std::cout << "\n[Info] No suspicious files found!\n";
        return 0;
    }
    std::cout << "\n[Info] Found suspicious file/s!\n";
    for (const auto& [file, extension] : flaggedFiles) {
        std::cout << "  -> FlaggedFile: " << file.path.string() << " was flagged as a " << extension <<" file!\n";
    }
    return 0;
}

int CLIParser::handleDuplicates(const std::string &path) {
    DuplicateFinder finder(FileCollector::collect(path));
    std::vector<std::vector<FileInfo>> duplicates = finder.find();

    if (duplicates.empty()){std::cout <<"No duplicates found!\n";}

    for (auto& filesD : duplicates) {
        std::cout << "\nFile Duplicate found: \n";
        for (auto& file: filesD) {
            std::cout << file.path << " size: " << file.size/1024.0 << "KB" << std::endl;
        }
        std::cout << "\nWould you like to delete following file/s? [Y : Yes], [N: NO] \n";

        for (int i = 1; i < filesD.size(); i++) {
            std::cout << filesD[i].path << " size: " << filesD[i].size/1024.0 << "KB" << std::endl;
        }
        std::cout << "\n";
        std::string deleteAnswer;
        std::cin >> deleteAnswer;
        std::transform(deleteAnswer.begin(), deleteAnswer.end(), deleteAnswer.begin(), ::tolower);

        if (deleteAnswer == "y") {
            finder.deleteDuplicates(filesD);
        }
    }
    return 0;
}

int CLIParser::handleSimilar(const std::string &file, const std::string &dir) {
    SimilarityFinder simFinder(file, dir);
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
    return 0;
}

int CLIParser::handleAging(const std::string &path, int ageCap) {
    const std::vector<FileInfo> files = FileCollector::collect(path);
    AgingFileFinder finder(files, ageCap);
    const std::vector<FileInfo> results = finder.find()[0];
    if (results.empty()) {std::cout <<"\nNo matches found!";}

    for (auto& file: results) {
        std::cout << "Match! : " << file.path.string() << " Last Used: " << file.lastUsed << " days ago!\n";
    }
    return 0;
}

int CLIParser::handleXOREncrypt(const std::string &path, std::string &password) {
    XOREncryptor::encrypt(path, password);
    std::cout << "[Info] Encryption succesfull!\n";
    return 0;
}

int CLIParser::handleXORDecrypt(const std::string &path, std::string &password) {
    XOREncryptor::decrypt(path, password);
    std::cout << "[Info] Decryption succesfull!\n";
    return 0;
}

int CLIParser::handleEntropy(const std::string &path) {
    const double entropy = EntropyAnalyzer::analyze(path);
    std::cout << "[Info] Entropy of file: " << path << "is ->" << entropy << "bits/byte !\n";
    return 0;
}

int CLIParser::handleFileInfo(const std::string &path) {
    FileAnalyzer::analyze(path);
    return 0;
}

int CLIParser::handlePathInfo(const std::string &path) {
    PathAnalyzer analyzer(path);
    analyzer.analyze();
    analyzer.printAnalytics();
    return 0;
}

int CLIParser::handleRecovery(const std::string &path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "[Error] File not found!\n";
    }

    std::ifstream ifs(path, std::ios::binary);
    RawRecoveryScanner scanner(false);
    scanner.scan(ifs);
    scanner.extractFiles(ifs, "recovered");
    ifs.close();
    return 0;
}



