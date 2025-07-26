//
// Created by Admin on 25.07.2025.
//

#include "AESEncryptor.h"

#include <array>
#include <cstring>
#include <filesystem>
#include <random>

#include "../hashing/picosha2.h"

//Creates 16 byte key from hashed password
void AESEncryptor::deriveKeyFromPassword(const std::vector<uint8_t>& password, std::array<uint8_t, 32> &key, const size_t& iterations) {
    key.fill(0);
    std::vector<uint8_t> hashedPassword(32);
    picosha2::hash256(password.begin(), password.end(), hashedPassword.begin(), hashedPassword.end());

    //Derivate key iteration times
    for (size_t i = 1; i < iterations; i++) {
        picosha2::hash256(hashedPassword.begin(), hashedPassword.end(), hashedPassword.begin(), hashedPassword.end());
    }

    std::memcpy(key.data(), hashedPassword.data(), key.size());
}

void AESEncryptor::encrypt(const std::string &src, const std::string &password, const size_t& iterations) {

    //Initilize AES struct
    AES_ctx ctx;
    std::array<uint8_t, 32> key;

    //Create random IV
    std::array<uint8_t, 16> iv;
    std::random_device rd;
    std::mt19937 gen(rd());
    for (auto& byte : iv) {
        byte = gen() % 255;
    }
    //create salt and derive key
    const auto salt = AbstractEncryptor::generateSalt();
    std::vector<unsigned char> pw(password.begin(), password.end());
    std::vector<unsigned char> saltedPassword;
    saltedPassword.reserve(salt.size() + pw.size());
    saltedPassword.insert(saltedPassword.end(), salt.begin(), salt.end());
    saltedPassword.insert(saltedPassword.end(), pw.begin(), pw.end());

    deriveKeyFromPassword(saltedPassword, key, iterations);

    AES_init_ctx_iv(&ctx, key.data(), iv.data());

    //Get Data
    std::ifstream ifs(src, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    size_t sizeOriginalFile = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(sizeOriginalFile);
    ifs.read(reinterpret_cast<char*>(data.data()), data.size());
    ifs.close();

    //Create buffer to 16 byte blocks
    size_t padding = 16 - (data.size() % 16);
    data.insert(data.end(), padding, static_cast<uint8_t>(padding));

    //Encrypt data
    AES_CBC_encrypt_buffer(&ctx, data.data(), data.size());

    //Write iv + encrypted data
    std::ofstream ofs(src, std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(salt.data()), salt.size());
    ofs.write(reinterpret_cast<char*>(iv.data()), iv.size());
    ofs.write(reinterpret_cast<char*>(data.data()), data.size());
    ofs.close();
}

void AESEncryptor::decrypt(const std::string &src, const std::string &password, const size_t& iterations) {
    //Initialize ifs and AES struct
    AES_ctx ctx;
    std::ifstream ifs(src, std::ios::binary);

    //Gather key and IV
    std::array<uint8_t, 32> key;
    std::array<uint8_t, 16> salt;
    ifs.read(reinterpret_cast<char*>(salt.data()), salt.size());
    std::vector<uint8_t> fullPassword;
    fullPassword.reserve(password.size()+salt.size());
    fullPassword.insert(fullPassword.end(), salt.begin(), salt.end());
    fullPassword.insert(fullPassword.end(), password.begin(), password.end());
    deriveKeyFromPassword(fullPassword, key, iterations);

    std::array<uint8_t, 16> iv;
    ifs.read(reinterpret_cast<char*>(iv.data()), iv.size());

    //Read data
    size_t sizeEncryptedFile = std::filesystem::file_size(src) - iv.size()- salt.size();
    std::vector<uint8_t> data(sizeEncryptedFile);
    ifs.read(reinterpret_cast<char*>(data.data()), data.size());
    ifs.close();

    //Decrypt data
    AES_init_ctx_iv(&ctx, key.data(), iv.data());
    AES_CBC_decrypt_buffer(&ctx, data.data(), data.size());

    //Remove padding
    if (!data.empty()) {
        size_t paddingSize = data.back();
        data.resize(data.size() - paddingSize);
    }

    //Write decrypted file
    std::ofstream ofs(src, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(data.data()), data.size());
    ofs.close();
}
