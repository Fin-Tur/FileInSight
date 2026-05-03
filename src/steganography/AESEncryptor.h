#pragma once
#include <fstream>
#include <string>

#include "AbstractEncryptor.h"
#include "../thirdparty/aesEnc/aes.hpp"
#include "../hashing/picosha2.h"


static constexpr uint8_t MAGIC[4] = { 0x45, 0x4E, 0x43, 0x31 }; // "ENC1"

struct EncryptedFileHeader {
    
    uint8_t enc_iter_expo;
    uint16_t header_length = sizeof(EncryptedFileHeader);
    
    uint8_t magic[4];
    
    uint8_t salt[16];
    uint8_t iv[16];
    
    uint8_t auth_hmac[32];

    //TODO unstable over diffrent compilers and build systems, should read params one by one

    bool getEncHeader(std::ifstream& ifs) {
        ifs.seekg(0, std::ios::end);
        size_t fileSize = ifs.tellg();
        if (fileSize < sizeof(EncryptedFileHeader)) {
            return false;
        }
        ifs.seekg(0, std::ios::beg);
        ifs.read(reinterpret_cast<char*>(this), sizeof(EncryptedFileHeader));
        return static_cast<bool>(ifs);
    }

    bool writeEncHeader(std::ofstream& ofs) const {
        ofs.write(reinterpret_cast<const char*>(this), sizeof(EncryptedFileHeader));
        return static_cast<bool>(ofs);
    }
};

class AESEncryptor : public AbstractEncryptor{
public:
    void encrypt(const std::string& src, const std::string& password, const size_t& iterations) override;
    void decrypt(const std::string& src, const std::string& password, const size_t& iterations) override;

private:
   static EncryptedFileHeader createHeader(const size_t& iterations, const std::string& password,  std::array<uint8_t, 32>& encKey);
   bool validateHeader(const EncryptedFileHeader &header, const std::string& password, std::array<uint8_t, 32> *encKeyOut);
   static void deriveKeyFromPassword(const std::vector<uint8_t>& password, std::array<uint8_t, 32> &key, const size_t& iterations);
};


