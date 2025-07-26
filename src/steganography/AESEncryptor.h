//
// Created by Admin on 25.07.2025.
//

#ifndef AESENCRYPTOR_H
#define AESENCRYPTOR_H
#include <string>

#include "AbstractEncryptor.h"
#include "../thirdparty/aesEnc/aes.hpp"


class AESEncryptor : public AbstractEncryptor{
public:
    void encrypt(const std::string& src, const std::string& password, const size_t& iterations) override;
    void decrypt(const std::string& src, const std::string& password, const size_t& iterations) override;

private:
   static void deriveKeyFromPassword(const std::vector<uint8_t>& password, std::array<uint8_t, 32> &key, const size_t& iterations);
};



#endif //AESENCRYPTOR_H
