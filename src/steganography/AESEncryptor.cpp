
#include "AESEncryptor.h"

#include <array>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <random>

#include "../hashing/picosha2.h"

namespace {

std::array<uint8_t, 32> hmacSha256(const std::array<uint8_t, 32>& key, const std::vector<uint8_t>& message) {
    constexpr size_t blockSize = 64;
    std::array<uint8_t, blockSize> keyBlock{};
    std::array<uint8_t, blockSize> iPad{};
    std::array<uint8_t, blockSize> oPad{};

    std::memcpy(keyBlock.data(), key.data(), key.size());
    for (size_t i = 0; i < blockSize; ++i) {
        iPad[i] = static_cast<uint8_t>(keyBlock[i] ^ 0x36);
        oPad[i] = static_cast<uint8_t>(keyBlock[i] ^ 0x5c);
    }

    std::vector<uint8_t> inner;
    inner.reserve(blockSize + message.size());
    inner.insert(inner.end(), iPad.begin(), iPad.end());
    inner.insert(inner.end(), message.begin(), message.end());

    std::array<uint8_t, 32> innerHash{};
    picosha2::hash256(inner.begin(), inner.end(), innerHash.begin(), innerHash.end());

    std::vector<uint8_t> outer;
    outer.reserve(blockSize + innerHash.size());
    outer.insert(outer.end(), oPad.begin(), oPad.end());
    outer.insert(outer.end(), innerHash.begin(), innerHash.end());

    std::array<uint8_t, 32> mac{};
    picosha2::hash256(outer.begin(), outer.end(), mac.begin(), mac.end());
    return mac;
}

std::vector<uint8_t> buildHeaderMacInput(const EncryptedFileHeader& header) {
    std::vector<uint8_t> headerMacInput;
    headerMacInput.reserve(4 + 2 + 16 + 16 + 1);
    headerMacInput.insert(headerMacInput.end(), header.magic, header.magic + sizeof(header.magic));
    headerMacInput.push_back(static_cast<uint8_t>((header.header_length >> 8) & 0xFF));
    headerMacInput.push_back(static_cast<uint8_t>(header.header_length & 0xFF));
    headerMacInput.insert(headerMacInput.end(), header.salt, header.salt + sizeof(header.salt));
    headerMacInput.insert(headerMacInput.end(), header.iv, header.iv + sizeof(header.iv));
    headerMacInput.push_back(header.enc_iter_expo);
    return headerMacInput;
}

bool constantTimeEqual(const uint8_t* lhs, const uint8_t* rhs, const size_t size) {
    uint8_t diff = 0;
    for (size_t i = 0; i < size; ++i) {
        diff |= static_cast<uint8_t>(lhs[i] ^ rhs[i]);
    }
    return diff == 0;
}

} // namespace

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

bool AESEncryptor::validateHeader(const EncryptedFileHeader &header, const std::string& password, std::array<uint8_t, 32> *encKeyOut) {
    if (std::memcmp(header.magic, MAGIC, sizeof(MAGIC)) != 0) {
        throw std::runtime_error("File is not encrypted with this program");
    }

    std::vector<uint8_t> fullPassword;
    fullPassword.reserve(password.size()+sizeof(header.salt));
    fullPassword.insert(fullPassword.end(), header.salt, header.salt + sizeof(header.salt));
    fullPassword.insert(fullPassword.end(), password.begin(), password.end());
    deriveKeyFromPassword(fullPassword, *encKeyOut, std::pow(10, header.enc_iter_expo));

    std::array<uint8_t, 32> authKey{};
    std::memcpy(authKey.data(), encKeyOut->data(), authKey.size());
    picosha2::hash256(authKey.begin(), authKey.end(), authKey.begin(), authKey.end());
    const auto headerMacInput = buildHeaderMacInput(header);
    const auto expectedMac = hmacSha256(authKey, headerMacInput);
    if (!constantTimeEqual(header.auth_hmac, expectedMac.data(), sizeof(header.auth_hmac))) {
        throw std::runtime_error("Header authentication failed");
    }

    return true;

}

//TODO HMAC over whole payloud for file integrity check
EncryptedFileHeader AESEncryptor::createHeader(const size_t& iterations, const std::string& password, std::array<uint8_t, 32>& encKey) {
    EncryptedFileHeader header{};
    std::memcpy(header.magic, MAGIC, sizeof(MAGIC));
    auto salt = AbstractEncryptor::generateSalt();
    std::memcpy(header.salt, salt.data(), salt.size());
    std::random_device rd;
    std::mt19937 gen(rd());
    for (auto& byte : header.iv) {
        byte = gen() % 255;
    }

    header.enc_iter_expo = static_cast<uint8_t>(std::log10(iterations));

    std::vector<uint8_t> fullPassword;
    fullPassword.reserve(password.size()+sizeof(header.salt));
    fullPassword.insert(fullPassword.end(), header.salt, header.salt + sizeof(header.salt));
    fullPassword.insert(fullPassword.end(), password.begin(), password.end());
    deriveKeyFromPassword(fullPassword, encKey, iterations);

    std::array<uint8_t, 32> authKey{};
    std::memcpy(authKey.data(), encKey.data(), authKey.size());
    picosha2::hash256(authKey.begin(), authKey.end(), authKey.begin(), authKey.end());
    const auto headerMacInput = buildHeaderMacInput(header);
    const auto expectedMac = hmacSha256(authKey, headerMacInput);
    std::memcpy(header.auth_hmac, expectedMac.data(), sizeof(header.auth_hmac));

    return header;
}

void AESEncryptor::encrypt(const std::string &src, const std::string &password, const size_t& iterations) {

    //Initilize AES struct
    AES_ctx ctx{};
   
    std::array<uint8_t, 32> encKey{};
    EncryptedFileHeader header = createHeader(iterations, password, encKey);


    AES_init_ctx_iv(&ctx, encKey.data(), header.iv);

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
    header.writeEncHeader(ofs);
    ofs.write(reinterpret_cast<char*>(data.data()), data.size());
    ofs.close();
}

void AESEncryptor::decrypt(const std::string &src, const std::string &password, const size_t& iterations) {
    //Initialize ifs and AES struct
    AES_ctx ctx;
    std::ifstream ifs(src, std::ios::binary);

    //Gather key and IV
    EncryptedFileHeader header;
    if (!header.getEncHeader(ifs)) {
        throw std::runtime_error("Failed to read encrypted file header");
    }
    
    std::array<uint8_t, 32> key{};
    if (!validateHeader(header, password, &key)) {
        throw std::runtime_error("Invalid password");
    }

    //Read data
    size_t sizeEncryptedFile = std::filesystem::file_size(src) - sizeof(EncryptedFileHeader);
    std::vector<uint8_t> data(sizeEncryptedFile);
    ifs.read(reinterpret_cast<char*>(data.data()), data.size());
    ifs.close();

    //Decrypt data
    AES_init_ctx_iv(&ctx, key.data(), header.iv);
    AES_CBC_decrypt_buffer(&ctx, data.data(), data.size());

    //Remove padding
    if (!data.empty()) {
        size_t paddingSize = data.back();
        if (paddingSize > 16 || paddingSize == 0 || paddingSize > data.size()) {
            throw std::runtime_error("Invalid padding in decrypted data");
        }

        for (size_t i = data.size() - paddingSize; i < data.size(); ++i) {
            if (data[i] != paddingSize) {
                throw std::runtime_error("Invalid padding in decrypted data");
            }
        }
        
        data.resize(data.size() - paddingSize);
    }

    //Write decrypted file
    std::ofstream ofs(src, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(data.data()), data.size());
    ofs.close();
}
