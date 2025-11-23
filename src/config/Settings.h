#pragma once

#include "../thirdparty/json/json.hpp"

class Settings {
public:

    Settings();
    void setConfig();
    void printSettings() const;

    [[nodiscard]] std::string getCompression() const noexcept {return this->compression;}
    [[nodiscard]] uint8_t getCompLevel() const noexcept {return this->compressionLevel;}
    [[nodiscard]] std::string getEncryption() const noexcept {return this->encryption;}
    [[nodiscard]] int getkeyDerivation() const noexcept {return this->keyDerivationIterations;}
    [[nodiscard]] bool get_utf_16_le_enabled() const noexcept {return this->utf_16_le_enabled;}
    void setCompression(const std::string& compression) {this->compression = compression;}
    void setCompLevel(uint8_t compLevel) {this->compressionLevel = compLevel;}
    void setEncryption(const std::string& encryption) {this->encryption = encryption;}
    void setkeyDerivation(int keyIts) {this->keyDerivationIterations = keyIts;}
    void set_utf_16_le_enabled(bool utf16) {this->utf_16_le_enabled = utf16;}

    std::unordered_map<std::string, std::vector<std::string>> validArguments = {
        {"compression", {"zstd", "huffman"}},
        {"compLevel", {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22"}},
        {"encryption", {"xor", "aes"}},
        {"keyDerivation", {"0", "1", "2", "3", "4", "5", "6"}},
        {"utf16_enable", {"true", "false"}}
    };

private:

    std::string compression;
    uint8_t compressionLevel;
    std::string encryption;
    int keyDerivationIterations;
    bool utf_16_le_enabled;

};


