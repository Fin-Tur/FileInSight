#pragma once
#include <string>
#include <vector>

#include "../models/FileInfo.h"

const std::vector<uint8_t> vaultSignature = {'F', 'I', 'S', 'V'};

namespace VaultManager {

    //Creates Vault
     bool createVault(const std::string& dir, const std::string& name);
    //Dissolves vault
     bool dissolveVault(const std::string& path, const std::string &dst);

namespace detail {
    //writes File into vault
     bool writeFile(const FileInfo& fi, std::ofstream& ofs);
    //Gathers file out of Vault
     bool readFile(std::ifstream& ifs, const std::string &dst);
    }
};
