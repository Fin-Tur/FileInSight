//
// Created by Admin on 15.07.2025.
//

#include "XOREncryptor.h"
#include "../hashing/picosha2.h"
#include <algorithm>
#include <iostream>
#include <random>

std::vector<unsigned char> XOREncryptor::generateSalt() {
    std::vector<unsigned char> salt(16);
    std::random_device rd;
    std::generate(salt.begin(), salt.end(), [&rd]() { return rd() % 256; });
    return salt;
}

void XOREncryptor::encrypt(const std::filesystem::path& src, const std::string& password) {
    //Open file
    std::ifstream ifs(src, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open " << src << " for reading!" << std::endl;
        return;
    }
    //Read file
    ifs.seekg(0, std::ios::end);
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<unsigned char> data(size);
    if (!ifs.read(reinterpret_cast<char*>(data.data()), size)) {
        std::cerr << "Error reading file!" << std::endl;
        return;
    }

    ifs.close();

    //Generate hash
    std::vector<unsigned char> salt = generateSalt();
    std::vector<unsigned char> key(password.begin(), password.end());
    //salt + key = input
    std::vector<unsigned char> input;
    input.reserve(salt.size() + key.size());
    input.insert(input.end(), salt.begin(), salt.end());
    input.insert(input.end(), key.begin(), key.end());

    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(input.begin(), input.end(), hash.begin(), hash.end());

    //Encrypt file
    for (size_t i = 0; i < data.size(); i++) {
        data[i] ^= hash[i % hash.size()];
    }

    //del old file and write in new one
    std::filesystem::remove(src);
    std::ofstream ofs(src, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(salt.data()), salt.size());
    ofs.write(reinterpret_cast<char*>(data.data()), data.size());
    ofs.close();

}

void XOREncryptor::decrypt(const std::filesystem::path &src, const std::string& password) {
    //OPen file
    std::ifstream ifs(src, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open " << src << " for reading!" << std::endl;
        return;
    }
    //Read salt
    std::vector<unsigned char> salt(16);
    ifs.read(reinterpret_cast<char*>(salt.data()), salt.size());
    //determine data Size
    ifs.seekg(0, std::ios::end);
    std::streamsize fullSize = ifs.tellg();
    std::streamsize dataSize = fullSize - salt.size();
    ifs.seekg(salt.size(), std::ios::beg);
    //Read file
    std::vector<unsigned char> data(dataSize);
    if (!ifs.read(reinterpret_cast<char*>(data.data()), dataSize)) {
        std::cerr << "Error reading file!" << std::endl;
        return;
    }
    ifs.close();

    //Generate hash
    std::vector<unsigned char> key(password.begin(), password.end());
    //salt + key = input
    std::vector<unsigned char> input;
    input.reserve(salt.size() + key.size());
    input.insert(input.end(), salt.begin(), salt.end());
    input.insert(input.end(), key.begin(), key.end());

    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(input.begin(), input.end(), hash.begin(), hash.end());

    //Decrypt data
    for (size_t i = 0; i < data.size(); i++) {
        data[i] ^= hash[i % hash.size()];
    }

    //write file
    std::filesystem::remove(src);
    std::ofstream ofs(src, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(data.data()), data.size());

}
