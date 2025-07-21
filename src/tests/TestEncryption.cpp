//
// Created by Admin on 18.07.2025.
//

#include <fstream>
#include <iostream>
#include "../thirdparty/testing/catch_amalgamated.hpp"

#include "../steganography/XOREncryptor.h"

TEST_CASE("XOR En/De-cryption test succesful", "[encryption]") {
    //create testfile
    const std::filesystem::path src= "testFile.txt";
    if (src == "testFile.txt") {
        std::ofstream createTestFile("testFile.txt");
        std::string testData = "test123blablablaabcdefghijklmnopqrstuvw\nwasmacheichhier";
        createTestFile.write(testData.data(), testData.size());
        createTestFile.close();
    }
    //Open file
    std::ifstream ifs(src, std::ios::binary);
    //Gather size
    ifs.seekg(0, std::ios::end);
    std::streamsize originalSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    //read file
    std::vector<unsigned char> bufferBefore(originalSize);  //Datencontainer mit keiner festen grösse da size laufzeit bestimmt!
    ifs.read(reinterpret_cast<char*>(bufferBefore.data()), originalSize);
    ifs.close();
    //encrypt file
    XOREncryptor::encrypt(src, "test");
    //Open encrypted file
    std::ifstream ifsEncrypted(src, std::ios::binary);
    //gather encrypted file size
    ifsEncrypted.seekg(0, std::ios::end);
    std::streamsize encryptedSize = ifsEncrypted.tellg();
    ifsEncrypted.seekg(0, std::ios::beg);
    //read encrypted file
    std::vector<unsigned char> encryptedBuffer(encryptedSize);
    ifsEncrypted.read(reinterpret_cast<char*>(encryptedBuffer.data()), encryptedSize);
    ifsEncrypted.close();
    //Decrypt file
    XOREncryptor::decrypt(src, "test");
    //Read aftermath
    std::ifstream ifsDecrypted(src, std::ios::binary);
    //gather after size
    ifsDecrypted.seekg(0, std::ios::end);
    std::streamsize afterSize = ifsDecrypted.tellg();
    ifsDecrypted.seekg(0, std::ios::beg);
    //read file again
    std::vector<unsigned char> bufferAfter(afterSize);
    ifsDecrypted.read(reinterpret_cast<char*>(bufferAfter.data()), afterSize);
    ifsDecrypted.close();

    //Check Similarity
    bool decryptedCorrectly = (originalSize == afterSize && std::equal(bufferBefore.begin(), bufferBefore.end(), bufferAfter.begin()));

    bool encrypted = !std::equal(
        bufferBefore.begin(),
        bufferBefore.end(),
        encryptedBuffer.begin() + 16 //skipping Salt
    );

    std::filesystem::remove(src);

    REQUIRE(encrypted);
    REQUIRE(decryptedCorrectly);



}
