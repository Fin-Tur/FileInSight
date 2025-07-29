//
// Created by Admin on 28.07.2025.
//

#ifndef VAULTMANAGER_H
#define VAULTMANAGER_H
#include <string>
#include <vector>

#include "models/FileInfo.h"

const std::vector<uint8_t> vaultSignature = {'F', 'I', 'S', 'V'};

class VaultManager {
public:
    //Creates Vault
    static bool createVault(const std::string& dir, const std::string& name);
    //Dissolves vault
    static bool dissolveVault(const std::string& path, const std::string &dst);

private:
    //writes File into vault
    static bool writeFile(const FileInfo& fi, std::ofstream& ofs);
    //Gathers file out of Vault
    static bool readFile(std::ifstream& ifs, const std::string &dst);
};



#endif //VAULTMANAGER_H
