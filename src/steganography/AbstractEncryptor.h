#pragma once
#include <string>
#include <vector>

class AbstractEncryptor {
    public:

    virtual ~AbstractEncryptor() = default;

    virtual void encrypt(const std::string& src, const std::string& password, const size_t& iterations) = 0;
    virtual void decrypt(const std::string& src, const std::string& password, const size_t& iterations) = 0;

    [[nodiscard]] static std::vector<unsigned char> generateSalt();

    bool handleEncryption(const std::string &src, const std::string &password, const int iterations);
    bool handleDecryption(const std::string &src, const std::string &password, const int iterations);
};

