//
// Created by Admin on 15.07.2025.
//

#ifndef XORENCRYPTOR_H
#define XORENCRYPTOR_H
#include <filesystem>
#include <vector>

#include "AbstractEncryptor.h"


class XOREncryptor : public AbstractEncryptor{
public:
    void encrypt(const std::string& src, const std::string& password, const size_t& iterations) override;
    void decrypt(const std::string& src, const std::string& password, const size_t& iterations) override;
};



#endif //XORENCRYPTOR_H
