//
// Created by Admin on 15.07.2025.
//

#ifndef XORENCRYPTOR_H
#define XORENCRYPTOR_H
#include <filesystem>
#include <vector>


class XOREncryptor {
public:
    static void encrypt(const std::filesystem::path& src, const std::string& password);
    static void decrypt(const std::filesystem::path& src, const std::string& password);
    static std::vector<uint8_t> generateSalt();
};



#endif //XORENCRYPTOR_H
