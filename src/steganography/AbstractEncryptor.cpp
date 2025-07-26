//
// Created by Admin on 26.07.2025.
//
#include "AbstractEncryptor.h"

#include <algorithm>
#include <random>

#include "hashing/picosha2.h"

std::vector<unsigned char> AbstractEncryptor::generateSalt() {
    std::vector<unsigned char> salt(16);
    std::random_device rd;
    std::generate(salt.begin(), salt.end(), [&rd]() { return rd() % 256; });

    return salt;
}
