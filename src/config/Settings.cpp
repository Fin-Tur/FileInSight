//
// Created by Admin on 24.07.2025.
//

#include "Settings.h"
#include <fstream>
#include <iostream>

Settings::Settings() {
    //Check for exiting config or else create
    if (!std::filesystem::exists("config.json")) {
        nlohmann::json j;
        j["compression"] = "zstd";
        j["compLevel"] = 15;
        j["encryption"] = "xor";
        j["enable_utf_16_le"] = false;

        std::ofstream ofs("config.json");
        ofs << j.dump();
        ofs.close();
    }

    //Open config file
    std::ifstream ifs("config.json");

    if (!ifs.is_open()) {
        std::cerr << "[Error] Could not open config file!" << std::endl;
    }
    //Read config
    nlohmann::json j;
    ifs >> j;
    //Set values
    this->compression = j.value("compression", this->compression);
    this->compressionLevel = j.value("compLevel", this->compressionLevel);
    this->encryption = j.value("encryption", this->encryption);
    this->utf_16_le_enabled = j.value("enable_utf_16_le", this->utf_16_le_enabled);

    ifs.close();
}

void Settings::setConfig() {

}

