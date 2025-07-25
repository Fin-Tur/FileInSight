//
// Created by Admin on 25.07.2025.
//

#ifndef AESENCRYPTOR_H
#define AESENCRYPTOR_H
#include <string>
#include "../thirdparty/aesEnc/aes.hpp"


class AESEncryptor {
public:
    static void encrypt(const std::string& src, std::string& password);
    static void decrypt(const std::string& src, std::string& password);

private:
   static void deriveKeyFromPassword(const std::string& password, std::array<uint8_t, 32> &key);
};



#endif //AESENCRYPTOR_H
