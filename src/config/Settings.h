//
// Created by Admin on 24.07.2025.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include "../thirdparty/json/json.hpp"

class Settings {
public:

    Settings();
    void setConfig();

    [[nodiscard]] std::string getCompression() const {return this->compression;}
    [[nodiscard]] uint8_t getCompLevel() const {return this->compressionLevel;}
    [[nodiscard]] std::string getEncryption() const {return this->encryption;}
    [[nodiscard]] bool get_utf_16_le_enabled() const {return this->utf_16_le_enabled;}

private:

    std::string compression;
    uint8_t compressionLevel;
    std::string encryption;
    bool utf_16_le_enabled;

};



#endif //SETTINGS_H
