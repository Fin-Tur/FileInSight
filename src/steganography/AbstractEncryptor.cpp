
#include "AbstractEncryptor.h"

#include <algorithm>
#include <deque>
#include <exception>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

#include "core/FileCollecter.h"
#include "hashing/picosha2.h"

std::vector<unsigned char> AbstractEncryptor::generateSalt() {
    std::vector<unsigned char> salt(16);
    std::random_device rd;
    std::generate(salt.begin(), salt.end(), [&rd]() { return rd() % 256; });

    return salt;
}

bool AbstractEncryptor::handleEncryption(const std::string &src, const std::string &password, const int iterations){
    //Collect Filest
    std::vector<FileInfo> files = FileCollector::collect(src);
    if (files.empty()) {
        std::cerr << "[Error] Invalid source!\n";
        return false;
    }

    //create thread enviromen
    std::deque<std::thread> threads;
    const int maxThreads = std::max(1u, std::thread::hardware_concurrency());
    std::exception_ptr workerException;
    std::mutex exceptionMutex;

    //Create lamda worker
    auto encryptWorker = [&](const FileInfo& fi) {
        try {
            this->encrypt(fi.path.string(), password, iterations);
        } catch (...) {
            std::lock_guard<std::mutex> lock(exceptionMutex);
            if (!workerException) {
                workerException = std::current_exception();
            }
        }
    };

    //Start threads
    for (auto& f : files) {
       threads.emplace_back(encryptWorker, f);
        if (threads.size() >= maxThreads) {
            threads[0].join();
            threads.pop_front();

        }
    }

    //Wait for threads to finish
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    if (workerException) {
        std::rethrow_exception(workerException);
    }

    return true;

}

bool AbstractEncryptor::handleDecryption(const std::string &src, const std::string &password, const int iterations) {
    //Collect Filest
    std::vector<FileInfo> files = FileCollector::collect(src);
    if (files.empty()) {
        std::cerr << "[Error] Invalid source!\n";
        return false;
    }

    //create thread enviromen
    std::deque<std::thread> threads;
    const int maxThreads = std::max(1u, std::thread::hardware_concurrency());
    std::exception_ptr workerException;
    std::mutex exceptionMutex;

    //Create lamda worker
    auto decryptWorker = [&](const FileInfo& fi) {
        try {
            this->decrypt(fi.path.string(), password, iterations);
        } catch (...) {
            std::lock_guard<std::mutex> lock(exceptionMutex);
            if (!workerException) {
                workerException = std::current_exception();
            }
        }
    };

    //Start threads
    for (auto& f : files) {
        threads.emplace_back(decryptWorker, f);
        if (threads.size() >= maxThreads) {
            threads[0].join();
            threads.pop_front();

        }
    }

    //Wait for threads to finish
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    if (workerException) {
        std::rethrow_exception(workerException);
    }

    return true;

}
