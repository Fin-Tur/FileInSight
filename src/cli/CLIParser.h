//
// Created by Admin on 13.07.2025.
//

#ifndef CLIPARSER_H
#define CLIPARSER_H
#include <functional>
#include <map>
#include <set>
#include <string>

#include "config/Settings.h"


class CLIParser {
public:
    static void printHelp();
    static int run(int argc, char* argv[]);
private:
    static Settings config;

    static int handleCompress(const std::string& path);
    static int handleDecompress(const std::string& path);
    static int handleFind(const std::string& path, const std::string& pattern);
    static int handleDefine(const std::string& path);
    static int handleDuplicates(const std::string& path);
    static int handleSimilar(const std::string& file, const std::string& dir);
    static int handleAging(const std::string& path, int ageCap);
    static int handleXOREncrypt(const std::string& path, std::string& password);
    static int handleXORDecrypt(const std::string& path, std::string& password);
    static int handleEntropy(const std::string& path);
    static int handleFileInfo(const std::string& path);
    static int handlePathInfo(const std::string& path);
    static int handleRecovery(const std::string& path);

    static int handleSettings(int argc, char* arg[]);
};



#endif //CLIPARSER_H
