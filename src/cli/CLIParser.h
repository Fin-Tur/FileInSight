//
// Created by Admin on 13.07.2025.
//

#ifndef CLIPARSER_H
#define CLIPARSER_H
#include <set>
#include <string>
#include <unordered_set>

const std::unordered_set<std::string> commands = {"-compress", "-decompress", "-find", "-define", "-duplicates", "-similar", "-aging"};
//zstd Comp, RegExFinder, MagicByteAnalyzer, DuplicateFinder, SimilarityFinder, AgingFinder

class CLIParser {
public:
    static int run(int argc, char* argv[]);
};



#endif //CLIPARSER_H
