//
// Created by Admin on 13.07.2025.
//

#ifndef CLIPARSER_H
#define CLIPARSER_H
#include <functional>
#include <map>
#include <set>
#include <string>


class CLIParser {
public:
    static void printHelp();
    static int run(int argc, char* argv[]);
private:
    static int handleCompress(const std::string& path);
    static int handleDecompress(const std::string& path);
    static int handleFind(const std::string& path, const std::string& pattern);
    static int handleDefine(const std::string& path);
    static int handleDuplicates(const std::string& path);
    static int handleSimilar(const std::string& file, const std::string& dir);
    static int handleAging(const std::string& path, int ageCap);
};



#endif //CLIPARSER_H
